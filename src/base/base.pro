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
    downloadmanager.h
SOURCES = \
	httprequest.cpp \
    downloadmanager.cpp

HEADERS_EXPORT += httprequest.h

INSTALLS += target \
			sources 
