TEMPLATE = app
TARGET = ssmp
DESTDIR = ../mingwout
QT += core gui sql phonon opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB QT_PHONON_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        .

win32 {
    win32-g++:LIBS += -L'E:/libs/taglib/Release/release' \
        -l:libTagLib1.a
}

DEPENDPATH += .
MOC_DIR += GeneratedFiles/debug
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
RC_FILE = ssmp.rc
QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__
include(ssmp.pri)
