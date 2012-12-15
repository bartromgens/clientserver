TEMPLATE = app
CONFIG += console
CONFIG -= qt

# add directory of client and server sources
INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

#INCLUDEPATH += /usr/include/boost
# use boost 1.52, see ./boost_1_52_0/stage/lib/dir for boost static and shared build libs
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread


SOURCES += main/mainserver.cpp \
    server/connectionstatus.cpp \
    server/dummyapplication.cpp \
    server/server.cpp \
    server/serverobserver.cpp \

HEADERS += \
    server/server.h \
    server/connectionstatus.h \
    server/dummyapplication.h \
    server/serverobserver.h \
    shared/clientserverdata.h \

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileServer

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall
