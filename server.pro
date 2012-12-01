TEMPLATE = app
CONFIG += console
CONFIG -= qt

#INCLUDEPATH += /usr/include/boost
# use boost 1.52
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread


SOURCES += ./src/main/mainserver.cpp \
    ./src/server/server.cpp

HEADERS += \
    ./src/server/server.h

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileServer

QMAKE_CXXFLAGS += -std=c++0x
