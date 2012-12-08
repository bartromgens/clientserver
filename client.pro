TEMPLATE = app
CONFIG += console
CONFIG -= qt

#INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread

SOURCES += ./src/main/mainclient.cpp \
    ./src/client/client.cpp

HEADERS += \
    ./src/client/client.h \

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileClient

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall
QMAKE_LFLAGS += -static
