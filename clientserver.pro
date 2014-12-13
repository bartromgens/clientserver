TARGET = subprojects
TEMPLATE = subdirs

# main libs to create
libclient.subdir = projects/libserver
libserver.subdir = projects/libclient

libclientjson.subdir = projects/libserverjson
libserverjson.subdir = projects/libclientjson

# tests
tests.subdir = projects/tests
tests.depends += libclient
tests.depends += libserver
tests.depends += libclientjson
tests.depends += libserverjson

# examples
clientgui.subdir = projects/examples/clientgui
servergui.subdir = projects/examples/servergui
clientgui.depends = libclient
servergui.depends = libserver


SUBDIRS += libclient
SUBDIRS += libserver

SUBDIRS += libclientjson
SUBDIRS += libserverjson

SUBDIRS += tests

SUBDIRS += clientgui
SUBDIRS += servergui


CONFIG += silent

CONFIG += warn_on
