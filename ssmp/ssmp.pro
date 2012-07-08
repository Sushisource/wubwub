TEMPLATE = app
TARGET = ssmp
DESTDIR = ../Output
QT += core gui sql opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        .

    LIBS += -L'E:/libs/irrKlang-1.3.0/bin/win32-gcc'


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
    playback/musicslider.h \
    albumtab/albumtab.h \
    albumtab/prettytext.h \
    playback/playbackwidget.h
SOURCES += dbi/dbi.cpp \
    options/optionswindow.cpp \
    main.cpp \
    recentalbumsview.cpp \
    ssmp.cpp \
    dbi/TagExtractor.cpp \
    playlist.cpp \
    playback/musicslider.cpp \
    albumtab/albumtab.cpp \
    albumtab/prettytext.cpp \
    playback/playbackwidget.cpp
FORMS += ssmp.ui \
    options/options.ui \
    playback/playbackwidget.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
