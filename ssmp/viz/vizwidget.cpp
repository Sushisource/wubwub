#include "vizwidget.h"

VizWidget::VizWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void VizWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // Prepare a complete shader program...
    if ( !prepareShaders(vertShader, fragShader) )
        return;
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if ( !m_shader.bind() )
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }

    //Call the viz's initialization logic
    vizInitialize();
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
