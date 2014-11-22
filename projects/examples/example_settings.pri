# add relative path of client and server sources
INCLUDEPATH += ../../../src/
DEPENDPATH = $$INCLUDEPATH

# client library
LIBS += -L../../../lib
LIBS += -lclient
LIBS += -lserver

# boost libraries
LIBS += -lboost_system
LIBS += -lpthread

# compiler settings
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

CONFIG(debug, debug|release) {
  OBJECTS_DIR = ./tmp/debug
} else {
  OBJECTS_DIR = ./tmp/release
}

UI_DIR += ./generated
MOC_DIR += ./generated

