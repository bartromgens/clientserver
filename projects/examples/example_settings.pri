# add relative path of client and server sources
INCLUDEPATH += ../../../src/
DEPENDPATH = $$INCLUDEPATH

# boost libraries
LIBS += -lboost_system
LIBS += -lpthread

CONFIG += silent

# compiler settings
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

CONFIG(debug, debug|release) {
  OBJECTS_DIR = ./tmp/debug
} else {
  OBJECTS_DIR = ./tmp/release
}

UI_DIR += ./generated
MOC_DIR += ./generated
