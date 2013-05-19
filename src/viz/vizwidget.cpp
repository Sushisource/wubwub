#include "vizwidget.h"

VizWidget::VizWidget(QWidget *parent) :
    QGLWidget(parent),
    shaderProgram(this),
    vertexBuffer(QOpenGLBuffer::VertexBuffer),
    fftBuffer()
{
    playbackWidget = NULL;
    vertShader = "./shaders/basic.vert";
    fragShader = "./shaders/basic.frag";
    needsResize = false;
    frameCount = 0;
    qDebug() << isValid();
}

VizWidget::~VizWidget()
{
}

void VizWidget::setPlayBackPointer(PlaybackWidget *p)
{
    playbackWidget = p;
}

void VizWidget::updateShaders()
{
    shaderProgram.setUniformValue("winSize", winWidth, winHeight);
    shaderProgram.setUniformValue("time", frameCount);
    fftBuffer.write(0, &fftData, FFT_SIZE);
}

void VizWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void VizWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    frameCount = 0;

    // Prepare a complete shader program...
    if ( !prepareShaders(vertShader, fragShader) )
        return;
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !shaderProgram.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }

    // Make the background quad and bind it to the vertex buffer
    GLfloat vertices[] = {-1, -1, 0, //bottom left corner
                          -1,  1, 0, //top left corner
                           1,  1, 0, //top right corner
                           1, -1, 0}; // bottom right corner

    vertexBuffer.create();
    vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if ( !vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    vertexBuffer.allocate(vertices, 3 * 4 * sizeof( float ));
    shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    shaderProgram.enableAttributeArray("vertex");


    // Setup the fft buffer
    fftBuffer.create();
    fftBuffer.bind(shaderProgram.programId(), "fftBlock");
    fftBuffer.allocate(FFT_SIZE);
}

void VizWidget::closeEvent(QCloseEvent *evt)
{
    fftBuffer.release();
    vertexBuffer.release();
    shaderProgram.removeAllShaders();
    shaderProgram.release();
    QGLWidget::closeEvent(evt);
}

void VizWidget::paintGL()
{
    qDebug() << isValid();
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //Update the fft buffer
    updateFFT();
    updateShaders();
    // Draw stuff
    glDrawArrays(GL_QUADS, 0, 4);

    ++frameCount;
}

void VizWidget::showEvent(QShowEvent *e)
{
    QGLWidget::showEvent(e);
    qDebug() << isValid();
}

void VizWidget::hideEvent(QHideEvent *e)
{
    QGLWidget::hideEvent(e);
}

bool VizWidget::prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath)
{
    // First we load and compile the vertex shader...
    bool result = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertShaderPath);
    if ( !result )
        qWarning() << shaderProgram.log();

    // ...now the fragment shader...
    result = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragShaderPath);
    if ( !result )
        qWarning() << shaderProgram.log();

    // ...and finally we link them to resolve any references.
    result = shaderProgram.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << shaderProgram.log();

    return result;
}

void VizWidget::updateFFT()
{
    if(playbackWidget != NULL)
        playbackWidget->getFFT(&fftData);
}
