# add relative path of client and server sources
INCLUDEPATH += ../../src/
DEPENDPATH += $$INCLUDEPATH

# boost libraries
LIBS += -lboost_system
LIBS += -lpthread

# compiler settings
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

