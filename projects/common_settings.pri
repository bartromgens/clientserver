# add relative path of client and server sources
INCLUDEPATH += ../../src/
DEPENDPATH += $$INCLUDEPATH

# boost libraries
LIBS += -lboost_system
LIBS += -lboost_thread # needed for BOOST_SPIRIT_THREADSAFE define that makes json parser thread safe
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
