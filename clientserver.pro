TARGET = subprojects
TEMPLATE = subdirs

# main libs to create
libclient.subdir = projects/libserver
libserver.subdir = projects/libclient

SUBDIRS += libclient
SUBDIRS += libserver

# examples
clientgui.subdir = projects/examples/clientgui
servergui.subdir = projects/examples/servergui

SUBDIRS += clientgui
SUBDIRS += servergui
