#ifndef VIZWIDGET_H
#define VIZWIDGET_H

#include <QGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QtOpenGL>
#include "playback/playbackwidget.h"
#include "viz/vizrenderthread.h"

class VizWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit VizWidget(QWidget *parent = 0);
    ~VizWidget();
    
    void setPlayBackPointer(PlaybackWidget *p);
    void stopRenderThread();
    void startRenderThread();
signals:
    
public slots:

protected:
    VizRenderThread renderThread;

    void closeEvent(QCloseEvent *evt);
    void resizeEvent(QResizeEvent *evt);
    void paintEvent(QPaintEvent *);

    virtual void vizInitialize() = 0;

};

#endif // VIZWIDGET_H
