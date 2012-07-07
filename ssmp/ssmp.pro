TEMPLATE = app
TARGET = ssmp
DESTDIR = ../Output
QT += core gui sql opengl svg
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/irrKlang-1.3.0/include \
        .

LIBS += -L'E:/libs/irrKlang-1.3.0/lib/win32-visualStudio' \
        -L'E:/libs/taglib/taglib/MinSizeRel' \
        -l'tag' \
        -l'irrKlang'\

RC_FILE = ssmp.rc
#QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__
HEADERS += dbi/TagExtractor.h \
    ssmp.h \
    recentalbumsview.h \
    dbi/dbi.h \
    options/optionswindow.h \
    playlist.h \
    playback/playbackmgr.h \
    playback/musicslider.h \
    albumtab/albumtab.h \
    albumtab/prettytext.h
SOURCES += dbi/dbi.cpp \
    options/optionswindow.cpp \
    main.cpp \
    recentalbumsview.cpp \
    ssmp.cpp \
    dbi/TagExtractor.cpp \
    playlist.cpp \
    playback/playbackmgr.cpp \
    playback/musicslider.cpp \
    albumtab/albumtab.cpp \
    albumtab/prettytext.cpp
FORMS += ssmp.ui \
    options/options.ui \
    playback/playbackwidget.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
