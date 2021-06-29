CONFIG -= qt
QT -= core gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics, PdfReader, DjVuFile, XpsFile, PdfWriter)

core_linux:include($$PWD/../../Common/3dParty/icu/icu.pri)
core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32
core_linux:LIBS += -lz

SOURCES += main.cpp

# for officefilechecker
SOURCES += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp \
    $$CORE_ROOT_DIR/Common/3dParty/pole/pole.cpp \
    $$CORE_ROOT_DIR/Common/DocxFormat/Source/Base/unicode_util.cpp

DESTDIR = $$PWD_ROOT_DIR/build
