QT += sql network
TARGET = base
TEMPLATE = lib
DESTDIR = ../build
DEFINES += MATHTOOL_LIBRARY
QMAKE_CXXFLAGS += -D_FILE_OFFSET_BITS=64

sources.files += $$HEADERS_EXPORT
sources.path = /usr/include/math_tool

HEADERS = \
	common_stuff.h \
	httprequest.h \
    downloadmanager.h \
    datamodel.h \
    author.h \
    publication.h \
    organization.h \
    keyword.h \
    journal.h \
    plugin_interface.h \
    reg_exp.h \
    types.h
SOURCES = \
	httprequest.cpp \
    downloadmanager.cpp \
    datamodel.cpp \
    author.cpp \
    publication.cpp \
    organization.cpp \
    keyword.cpp \
    journal.cpp \
    reg_exp.cpp

HEADERS_EXPORT += httprequest.h

INSTALLS += target \
			sources 
