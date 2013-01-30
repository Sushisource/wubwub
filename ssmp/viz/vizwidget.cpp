#include "vizwidget.h"

VizWidget::VizWidget(QWidget *parent) :
    QGLWidget(parent),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
}

void VizWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // Prepare a complete shader program...
    if ( !prepareShaders(":/shaders/basic.vert", ":/shaders/basic.frag" ) )
        return;
    // We need us some vertex data. Start simple with a triangle ;-)
    float points[] = { -0.5f, -0.5f, 0.0f, 1.0f,
                       0.5f, -0.5f, 0.0f, 1.0f,
                       0.0f,  0.5f, 0.0f, 1.0f };
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    m_vertexBuffer.allocate( points, 3 * 4 * sizeof( float ) );
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shader.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }

    // Enable the "vertex" attribute to bind it to our currently bound
    // vertex buffer.
    m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 4 );
    m_shader.enableAttributeArray( "vertex" );
}

void VizWidget::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0, 3 );
}

bool VizWidget::prepareShaders(const QString &vertShaderPath, const QString &fragShaderPath)
{
    // First we load and compile the vertex shader...
    bool result = m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, vertShaderPath);
    if ( !result )
        qWarning() << m_shader.log();

    // ...now the fragment shader...
    result = m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragShaderPath);
    if ( !result )
        qWarning() << m_shader.log();

    // ...and finally we link them to resolve any references.
    result = m_shader.link();
    if ( !result )
        qWarning() << "Could not link shader program:" << m_shader.log();

    return result;
}
