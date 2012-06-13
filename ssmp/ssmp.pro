TEMPLATE = app
TARGET = ssmp
DESTDIR = ../mingwout
QT += core gui sql opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/irrKlang-1.3.0/include \
        .

win32 {
    win32-g++:LIBS += -L'E:/libs/taglib/taglib-build-desktop-Qt_4_8_2__qtmingw__Release/release' \
        -l:libTagLib1.a
}

LIBS += -L'E:/libs/irrKlang-1.3.0/bin/win32-gcc' \
        -l'irrKlang'\
        -l'ikpFlac'\
        -l'ikpMP3'

RC_FILE = ssmp.rc
QMAKE_CXXFLAGS += -std=c++0x #-U__STRICT_ANSI__
#QMAKE_LFLAGS += -static-libgcc -static-libstdc++
HEADERS += dbi/TagExtractor.h \
    ssmp.h \
    recentalbumsview.h \
    dbi/dbi.h \
    options/optionswindow.h \
    playlist.h \
    playback/playbackmgr.h \
    playback/musicslider.h \
    albumtab.h
SOURCES += dbi/dbi.cpp \
    options/optionswindow.cpp \
    main.cpp \
    recentalbumsview.cpp \
    ssmp.cpp \
    dbi/TagExtractor.cpp \
    playlist.cpp \
    playback/playbackmgr.cpp \
    playback/musicslider.cpp \
    albumtab.cpp
FORMS += ssmp.ui \
    options/options.ui \
    playback/playbackwidget.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
