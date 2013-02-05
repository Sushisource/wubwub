#include "vizrenderthread.h"
#include "viz/vizwidget.h"

VizRenderThread::VizRenderThread(VizWidget *parent) :
    QThread(parent),
    vertexBuffer(QOpenGLBuffer::VertexBuffer),
    fftBuffer()
{
    this->parent = parent;
    playbackWidget = NULL;
    vertShader = ":/shaders/basic.vert";
    fragShader = ":/shaders/basic.frag";
    frameCount = 0;
    rendering = true;
    needsResize = false;
}

void VizRenderThread::resize(int w, int h)
{
    this->winWidth = w;
    this->winHeight = h;
    needsResize = true;
}

void VizRenderThread::run()
{
    parent->makeCurrent();
    initialize();
    while(rendering)
    {
        if(needsResize)
            glViewport(0, 0, (GLint)winWidth, (GLint)winHeight);

        // Clear the buffer with the current clearing color
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //Update the fft buffer
        updateFFT();
        updateShaders();
        // Draw stuff
        glDrawArrays(GL_QUADS, 0, 4);

        parent->swapBuffers();
        ++frameCount;
        msleep(16);
    }
}

void VizRenderThread::updateShaders()
{
    shaderProgram.setUniformValue("winSize", winWidth, winHeight);
    shaderProgram.setUniformValue("time", frameCount);
    fftBuffer.write(0, &fftData, FFT_SIZE);
}

void VizRenderThread::updateFFT()
{
    if(playbackWidget != NULL)
        playbackWidget->getFFT(&fftData);
}

void VizRenderThread::stop()
{
    rendering = false;
}

void VizRenderThread::initialize()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

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

bool VizRenderThread::prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath)
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
