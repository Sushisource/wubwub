TEMPLATE = app
TARGET = ssmp
DESTDIR = ../mingwout
QT += core gui sql phonon opengl
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB QT_PHONON_LIB
INCLUDEPATH += C:/Code/taglib \
        C:/Code/taglib/taglib \
        C:/Code/taglib/taglib/toolkit \
        .

win32 {
    win32-g++:LIBS += -L'C:/Code/taglib/taglib-build-desktop-Qt_4_8_0__4_8_0__Release/release' \
        -l:libTagLib1.a
}

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__
include(ssmp.pri)
