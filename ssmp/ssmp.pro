TEMPLATE = app
TARGET = ssmp
CONFIG += x86
DESTDIR = ../output
QT += core gui sql opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB

win32 {
    INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/irrKlang-1.3.0/include \
        .

    LIBS += -L'E:/libs/irrKlang-1.3.0/bin/win32-gcc'

    win32-g++:LIBS += -L'E:/libs/taglib/taglib-build-desktop-Qt_4_8_2__qtmingw__Release/release' \
        -l:libTagLib1.a

    RC_FILE = ssmp.rc
}

mac {
    INCLUDEPATH += /Developer/Libraries/taglib/output/include \
                   /Developer/Libraries/irrKlang-1.4.0/include

    LIBS += -L/Developer/Libraries/irrKlang-1.4.0/bin/macosx-gcc \
            -L/Developer/Libraries/taglib/output/lib
}

LIBS += -l'irrKlang'\
        -l'tag'\
        #-l'ikpFLAC'\
        #-l'ikpMP3'


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
