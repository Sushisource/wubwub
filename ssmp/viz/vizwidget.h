#ifndef VIZWIDGET_H
#define VIZWIDGET_H

#include <QGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QtOpenGL>

class VizWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit VizWidget(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    void initializeGL();
    void paintGL();

private:
    bool prepareShaders(const QString& vertShaderPath,
                        const QString& fragShaderPath);

    QOpenGLShaderProgram m_shader;
    QOpenGLBuffer m_vertexBuffer;
};

#endif // VIZWIDGET_H
