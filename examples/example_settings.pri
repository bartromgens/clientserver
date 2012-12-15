# add relative path of client and server sources
INCLUDEPATH += ../../src/
DEPENDPATH = $$INCLUDEPATH

# boost libraries
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0
LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread
LIBS += -lpthread

# compiler settings
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

# linker settings
