QT -= core
QT -= gui

TARGET = test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp

CORE_ROOT_DIR = $$PWD/../../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(raster)
