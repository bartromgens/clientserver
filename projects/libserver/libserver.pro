TEMPLATE = lib
CONFIG += console
CONFIG -= qt

TARGET = server
DESTDIR = ../../lib

INCLUDEPATH += ../../src/
DEPENDPATH = $$INCLUDEPATH
VPATH = $$INCLUDEPATH

LIBS += -lboost_system
LIBS += -lpthread


SOURCES += \
    server/server.cpp \
    server/connectionstatus.cpp \

HEADERS += \
    src/server/server.h \
    src/server/connectionstatus.h \
    src/shared/clientserverdata.h \

OBJECTS_DIR += ./tmp/objects/

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall
