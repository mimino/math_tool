QT += network xml webkitwidgets

TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH     += ../../base ../../html-qt

HEADERS         = msas_plugin.h \
    msaspublication.h \
    msaspublicationsrefresher.h \
    msasrefresherbase.h \
    msasauthorsrefresher.h \
    msasauthor.h \
    msaskeywordsrefresher.h

SOURCES         = msas_plugin.cpp \
    msaspublication.cpp \
    msaspublicationsrefresher.cpp \
    msasrefresherbase.cpp \
    msasauthorsrefresher.cpp \
    msasauthor.cpp \
    msaskeywordsrefresher.cpp

OTHER_FILES += msas.json

QMAKE_RPATHDIR += ../../build
QMAKE_LIBDIR += ../../build

LIBS += -lbase

TARGET          = $$qtLibraryTarget(msas-plugin)
DESTDIR         = ../../build/plugins

target.path = /usr/lib/math_tool/plugins

INSTALLS += target
