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
    QOpenGLShaderProgram m_shader;
    QString vertShader;
    QString fragShader;

    void initializeGL();
    virtual void vizInitialize() = 0;
    bool prepareShaders(const QString& vertShaderPath,
                        const QString& fragShaderPath);

};

#endif // VIZWIDGET_H
