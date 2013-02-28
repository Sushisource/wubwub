#ifndef VIZRENDERTHREAD_H
#define VIZRENDERTHREAD_H

#include <QThread>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QtOpenGL>
#include "playback/playbackwidget.h"
#include "glhelp/uniformbufferobject.h"

class VizWidget; //I hade header files

class VizRenderThread : public QThread
{
    Q_OBJECT
public:
    PlaybackWidget* playbackWidget;

    explicit VizRenderThread(VizWidget *parent = 0);

    void run();
    void stop();

    void resize(int w, int h);

signals:
    
public slots:

protected:

private:
    bool rendering, needsResize;
    int winWidth, winHeight, frameCount;
    VizWidget* parent;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    UniformBufferObject fftBuffer;
    QString vertShader;
    QString fragShader;
    GLfloat fftData[FFT_SIZE];

    void initialize();
    bool prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath);
    void updateShaders();
    void updateFFT();
};

#endif // VIZRENDERTHREAD_H
