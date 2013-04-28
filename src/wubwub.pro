TEMPLATE = app
TARGET = wubwub
DESTDIR = ..
QT += core gui sql opengl svg quick
DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB QT_OPENGL_LIB
INCLUDEPATH += E:/libs/taglib \
        E:/libs/taglib/taglib \
        E:/libs/taglib/taglib/ogg \
        E:/libs/taglib/taglib/toolkit \
        E:/libs/bass64/c \
        .

LIBS += -L'E:/libs/bass64/c' \
        -L'E:/libs/taglib/taglib/Release' \
        -l'tag' \
        -l'bass'\

RC_FILE = ../res/wubwub.rc
HEADERS += dbi/TagExtractor.h \
    qmltab/recentalbumstab.h \
    dbi/dbi.h \
    options/optionswindow.h \
    playlist.h \
    playback/musicslider.h \
    albumtab/albumtab.h \
    albumtab/prettytext.h \
    playback/playbackwidget.h \
    viz/vizwidget.h \
    viz/firstviz.h \
    viz/vizrenderthread.h \
    viz/glhelp/uniformbufferobject.h \
    qmltab/qmltab.h \
    wubwub.h \
    search/wwsearch.h \
    albumtab/wwtabwidget.h \
    qmltab/qmlalbumtab.h \
    qmltab/artistview.h
SOURCES += dbi/dbi.cpp \
    options/optionswindow.cpp \
    main.cpp \
    qmltab/recentalbumstab.cpp \
    dbi/TagExtractor.cpp \
    playlist.cpp \
    playback/musicslider.cpp \
    albumtab/albumtab.cpp \
    albumtab/prettytext.cpp \
    playback/playbackwidget.cpp \
    viz/vizwidget.cpp \
    viz/firstviz.cpp \
    viz/vizrenderthread.cpp \
    viz/glhelp/uniformbufferobject.cpp \
    qmltab/qmltab.cpp \
    wubwub.cpp \
    search/wwsearch.cpp \
    albumtab/wwtabwidget.cpp \
    qmltab/qmlalbumtab.cpp \
    qmltab/artistview.cpp
FORMS += wubwub.ui \
    options/options.ui \
    playback/playbackwidget.ui

OTHER_FILES += \
    shaders/basic.vert \
    shaders/basic.frag \
    ../res/qml/albumview.qml \
    ../res/qml/albumrecord.qml \
    ../res/qml/qmldir \
    ../res/qml/artistview.qml

RESOURCES += \
    ../res/res.qrc

QMAKE_CXXFLAGS += /DNOMINMAX
