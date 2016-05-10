QT += core widgets gui sql network

DESTDIR = ../build
OBJECTS_DIR += ../build
INCLUDEPATH     += ../base
QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64

HEADERS = \
	pages.h \
	addresswidget.h \
	tablemodel.h \
	authorswidget.h \
	extendedtabwidget.h \
	database.h \
	articleswidget.h \
	organizationswidget.h \
	mainwindow.h \
	subjectswidget.h \
	contentwidget.h \
	journalswidget.h \
	globalcontext.h \
    glwidget.h \
    camera3d.h \
    input.h \
    transform3d.h
SOURCES = \
	main.cpp \
	pages.cpp \
	addresswidget.cpp \
	tablemodel.cpp \
	authorswidget.cpp \
	extendedtabwidget.cpp \
	articleswidget.cpp \
	organizationswidget.cpp \
	mainwindow.cpp \
	subjectswidget.cpp \
	contentwidget.cpp \
	journalswidget.cpp \
	globalcontext.cpp \
    glwidget.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp

RESOURCES += \
    app.qrc

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg

QMAKE_RPATHDIR += ../build
QMAKE_LIBDIR += ../build

LIBS += -lbase
