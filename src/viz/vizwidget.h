#ifndef VIZWIDGET_H
#define VIZWIDGET_H

#include <QGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QtOpenGL>
#include "playback/playbackwidget.h"
#include "glhelp/uniformbufferobject.h"

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
    bool rendering;
    int winWidth, winHeight, frameCount;
    VizWidget* parent;
    PlaybackWidget* playbackWidget;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    UniformBufferObject fftBuffer;
    QImage fbo;
    QString vertShader;
    QString fragShader;
    GLfloat fftData[FFT_SIZE];
    std::unique_ptr<QTimer> renderClock;

    bool prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath);
    void updateShaders();
    void updateFFT();

};

#endif // VIZWIDGET_H
