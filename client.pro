TEMPLATE = app
CONFIG += console
CONFIG -= qt

# add directory of client and server sources
INCLUDEPATH += ./src/
DEPENDPATH = $$INCLUDEPATH

#INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread
LIBS += -lpthread

SOURCES += main/mainclient.cpp \
    client/client.cpp \
    client/clienttestgroup.cpp \

HEADERS += \
    client/client.h \
    client/clienttestgroup.h \

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileClient

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

