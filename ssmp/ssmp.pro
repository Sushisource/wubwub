TEMPLATE = app
TARGET = ssmp
DESTDIR = ../Output
QT += core gui sql opengl svg
DEFINES += QT_SQL_LIB QT_OPENGL_LIB

mac {
    INCLUDEPATH += /Developer/Libraries/bass24-osx \
                   /Developer/Libraries/taglib/output/include
    LIBS += -L/Developer/Libraries/bass24-osx \
            -framework tag \
            -lbass
}

QMAKE_CXXFLAGS += -std=c++11 -U__STRICT_ANSI__
QMAKE_LFLAGS += -F/Library/Frameworks/tag.framework

HEADERS += dbi/TagExtractor.h \
    ssmp.h \
    recentalbumsview.h \
    dbi/dbi.h \
    options/optionswindow.h \
    playlist.h \
    playback/musicslider.h \
    albumtab/albumtab.h \
    albumtab/prettytext.h \
    playback/playbackwidget.h \
    albumtab/ssmptabwidget.h
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
    playback/playbackwidget.cpp \
    albumtab/ssmptabwidget.cpp
FORMS += ssmp.ui \
    options/options.ui \
    playback/playbackwidget.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
