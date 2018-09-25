#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class OpenGLWidget : public QOpenGLWidget , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() ;


signals:

public slots:
    void toggleBackgroundColor(bool);

protected :
    void initializeGL();
    void resizeGL(int width , int height);
    void paintGL();
};

#endif // OPENGLWIDGET_H
