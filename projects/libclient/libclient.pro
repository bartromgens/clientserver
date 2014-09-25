TEMPLATE = lib
CONFIG += console
CONFIG -= qt

TARGET = client
DESTDIR = ../../lib

INCLUDEPATH += ../../src/
DEPENDPATH = $$INCLUDEPATH
VPATH = $$INCLUDEPATH

LIBS += -lboost_system
LIBS += -lpthread


SOURCES += \
    client/client.cpp \

HEADERS += \
    client/client.h \
    shared/clientserverdata.h \

OBJECTS_DIR += ./tmp/objects/


QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

