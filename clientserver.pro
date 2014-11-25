TARGET = subprojects
TEMPLATE = subdirs

# main libs to create
libclient.subdir = projects/libserver
libserver.subdir = projects/libclient

# tests
tests.subdir = projects/tests
tests.depends += libclient
tests.depends += libserver

# examples
clientgui.subdir = projects/examples/clientgui
servergui.subdir = projects/examples/servergui
clientgui.depends = libclient
servergui.depends = libserver

# simulationserver
simulationclient.subdir = projects/simulationclient
simulationserver.subdir = projects/simulationserver
simulationclient.depends = libclient
simulationserver.depends = libserver


SUBDIRS += libclient
SUBDIRS += libserver

SUBDIRS += tests

SUBDIRS += clientgui
SUBDIRS += servergui

SUBDIRS += simulationserver
SUBDIRS += simulationclient

CONFIG += silent

CONFIG += warn_on
