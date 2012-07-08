TEMPLATE = app
TARGET = ssmp
DESTDIR = ../Output
QT += core gui sql opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/bass64/c \
        .

LIBS += -L'E:/libs/bass64/c' \
        -L'E:/libs/taglib/taglib/MinSizeRel' \
        -l'tag' \
        -l'bass'\

RC_FILE = ssmp.rc
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
