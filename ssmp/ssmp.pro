TEMPLATE = app
TARGET = ssmp
DESTDIR = ../mingwout
QT += core gui sql phonon opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB QT_PHONON_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/irrKlang-1.3.0/include \
        .

win32 {
    win32-g++:LIBS += -L'E:/libs/taglib/Release/release' \
        -l:libTagLib1.a
}

LIBS += -L'E:/libs/irrKlang-1.3.0/bin/win32-gcc' \
        -l'irrKlang'\
        -l'ikpMP3'

DEPENDPATH += .
MOC_DIR += GeneratedFiles/debug
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
RC_FILE = ssmp.rc
QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__
HEADERS += TagExtractor.h \
    ssmp.h \
    recentalbumsview.h \
    dbi.h \
    optionswindow.h \
    playlist.h \
    playbackmgr.h \
    musicslider.h
SOURCES += dbi.cpp \
    optionswindow.cpp \
    main.cpp \
    recentalbumsview.cpp \
    ssmp.cpp \
    TagExtractor.cpp \
    playlist.cpp \
    playbackmgr.cpp \
    musicslider.cpp
FORMS += ssmp.ui \
    options.ui \
    playbackwidget.ui

OTHER_FILES += \
    Resources/plus_alt.svg
