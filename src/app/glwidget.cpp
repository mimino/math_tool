#include <QtMath>
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glwidget.h"
#include "globalcontext.h"
#include "input.h"

GLWidget::GLWidget(DataModel& model, int entityType) : _dataModel(model)
{
	m_transform.translate(0.0f, 0.0f, -5.0f);

	_visualization = new DataVisualization(this, _dataModel, entityType, GlobalContext::getCurrentSubjectId());
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
	connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

	// Set global information
	//glEnable(GL_CULL_FACE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Create Shader (Do not release until VAO is created)
	m_program = new QOpenGLShaderProgram();
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vert");
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.frag");
	m_program->link();
	m_program->bind();

	// Cache Uniform Locations
	u_modelToWorld = m_program->uniformLocation("modelToWorld");
	u_worldToCamera = m_program->uniformLocation("worldToCamera");
	u_cameraToView = m_program->uniformLocation("cameraToView");

	// Create Buffer (Do not release until VAO is created)
	m_vertex.create();
	m_vertex.bind();
	m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);

	const int verticesCount = _visualization->graph().count()*2;
	QVector<GLfloat> buf;
	buf.resize(verticesCount*7);
	GLfloat *p = buf.data();
	for (int i = 0; i < verticesCount/2; ++i) {
		Edge edge(_visualization->graph()[i]);
		*p++ = _visualization->coordinates()[edge[0]].x();
		*p++ = _visualization->coordinates()[edge[0]].y();
		*p++ = _visualization->coordinates()[edge[0]].z();
		*p++ = 0.f;
		*p++ = 1.f;
		*p++ = 0.f;
		*p++ = _visualization->sizes()[edge[0]];

		*p++ = _visualization->coordinates()[edge[1]].x();
		*p++ = _visualization->coordinates()[edge[1]].y();
		*p++ = _visualization->coordinates()[edge[1]].z();
		*p++ = 0.f;
		*p++ = 1.f;
		*p++ = 0.f;
		*p++ = _visualization->sizes()[edge[1]];
	}
	m_vertex.allocate(buf.constData(), buf.count()*sizeof(GLfloat));

	// Create Vertex Array Object
	m_object.create();
	m_object.bind();
	m_program->enableAttributeArray(0);
	m_program->enableAttributeArray(1);
	m_program->enableAttributeArray(2);

	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Point)*2 + sizeof(GLfloat));
	m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(Point), 3, sizeof(Point)*2 + sizeof(GLfloat));
	m_program->setAttributeBuffer(2, GL_FLOAT, sizeof(Point)*2, 1, sizeof(Point)*2 + sizeof(GLfloat));

	// Release (unbind) all
	m_object.release();
	m_vertex.release();
	m_program->release();
}

void GLWidget::resizeGL(int width, int height)
{
	m_projection.setToIdentity();
	m_projection.perspective(60.0f, width / float(height), 0.1f, 1000.0f);
}

void GLWidget::paintGL()
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPointSize(10.f);

	// Render using our shader
	m_program->bind();
	m_program->setUniformValue(u_worldToCamera, m_camera.toMatrix());
	m_program->setUniformValue(u_cameraToView, m_projection);
	{
		m_object.bind();
		m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
		glDrawArrays(GL_POINTS, 0, _visualization->graph().count()*2);
		m_object.release();
	}
	m_program->release();
}

void GLWidget::teardownGL()
{
	// Actually destroy our OpenGL information
	m_object.destroy();
	m_vertex.destroy();
	delete m_program;
	delete _visualization;
}

void GLWidget::update()
{
	// Update input
	Input::update();

	// Camera Transformation
	if (Input::buttonPressed(Qt::RightButton))
	{
		static const float transSpeed = 0.5f;
		static const float rotSpeed   = 0.5f;

		// Handle rotations
		m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
		m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

		// Handle translations
		QVector3D translation;
		if (Input::keyPressed(Qt::Key_W))
		{
			translation += m_camera.forward();
		}
		if (Input::keyPressed(Qt::Key_S))
		{
			translation -= m_camera.forward();
		}
		if (Input::keyPressed(Qt::Key_A))
		{
			translation -= m_camera.right();
		}
		if (Input::keyPressed(Qt::Key_D))
		{
			translation += m_camera.right();
		}
		if (Input::keyPressed(Qt::Key_Q))
		{
			translation -= m_camera.up();
		}
		if (Input::keyPressed(Qt::Key_E))
		{
			translation += m_camera.up();
		}
		m_camera.translate(transSpeed * translation);
	}
	else if(Input::buttonPressed(Qt::LeftButton))
	{
		int dX = Input::mouseDelta().x();
		int dY = Input::mouseDelta().y();

		// Update instance information
		m_transform.rotate(3.0f, QVector3D(dY, dX, 0.f));
	}
	else if(Input::wheelTriggered())
	{
		static const float transSpeed = 0.5f;
		int dY = Input::wheelDelta().y();
		Input::reset();

		qreal sss = -dY/qFabs(dY);
		m_transform.translate(0, 0, sss*0.1f);
	}

	// Schedule a redraw
	QOpenGLWidget::update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyPress(event->key());
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyRelease(event->key());
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	Input::registerMousePress(event->button());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Input::registerMouseRelease(event->button());
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	Input::registerWheelEvent(event);
}
