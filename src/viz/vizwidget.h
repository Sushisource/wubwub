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

signals:
    
public slots:

protected:
    void closeEvent(QCloseEvent *evt);
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    bool rendering, needsResize;
    int winWidth, winHeight, frameCount;
    VizWidget* parent;
    PlaybackWidget* playbackWidget;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    UniformBufferObject fftBuffer;
    QString vertShader;
    QString fragShader;
    GLfloat fftData[FFT_SIZE];

    bool prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath);
    void updateShaders();
    void updateFFT();

};

#endif // VIZWIDGET_H
