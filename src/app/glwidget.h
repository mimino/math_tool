#ifndef __GLWIDGET__
#define __GLWIDGET__

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "types.h"
#include "transform3d.h"
#include "camera3d.h"
#include "datamodel.h"
#include "datavisualization.h"

class QOpenGLShaderProgram;

class GLWidget : public QOpenGLWidget,
	protected QOpenGLFunctions
{
	Q_OBJECT

		// OpenGL Events
public:
	GLWidget(DataModel& model, int entityType);
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	protected slots:
		void teardownGL();
		void update();

protected:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	DataModel& _dataModel;
	// OpenGL State Information
	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;

	// Shader Information
	int u_modelToWorld;
	int u_worldToCamera;
	int u_cameraToView;
	QMatrix4x4 m_projection;
	Camera3D m_camera;
	Transform3D m_transform;
	DataVisualization* _visualization;

	// Private Helpers
	void printVersionInformation();
};

#endif // __GLWIDGET__
