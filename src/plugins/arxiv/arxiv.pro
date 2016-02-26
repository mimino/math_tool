QT += network
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH     += ../../base

HEADERS         = arxiv_plugin.h

SOURCES         = arxiv_plugin.cpp

OTHER_FILES += arxiv.json

LIBS += -lbase

QMAKE_RPATHDIR += ../../build
QMAKE_LIBDIR += ../../build

TARGET          = $$qtLibraryTarget(arxiv-plugin)
DESTDIR         = ../../build/plugins

target.path = /usr/lib/math_tool/plugins

INSTALLS += target

