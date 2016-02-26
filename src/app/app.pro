QT += core widgets gui sql network

DESTDIR = ../build
OBJECTS_DIR += ../build

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
	globalcontext.h
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
	globalcontext.cpp

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
