#include "firstviz.h"

FirstViz::FirstViz(QWidget *parent) :
    VizWidget(parent),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    vertShader = ":/shaders/basic.vert";
    fragShader = ":/shaders/basic.frag";
}

void FirstViz::vizInitialize()
{
    // We need us some vertex data. Start simple with a triangle ;-)
    float points[] = { -0.3f, -0.3f, 0.0f, 1.0f,
                       0.3f, -0.5f, 0.0f, 1.0f,
                       0.0f,  0.3f, 0.0f, 1.0f };
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";
        return;
    }
    m_vertexBuffer.allocate( points, 3 * 4 * sizeof( float ) );

    // Enable the "vertex" attribute to bind it to our currently bound
    // vertex buffer.
    m_shader.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    m_shader.enableAttributeArray("vertex");
}

void FirstViz::paintGL()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0, 3 );
}
