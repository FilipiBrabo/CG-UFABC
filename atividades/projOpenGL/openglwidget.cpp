#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :  QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL()
{
    //glClearColor(1,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWidget::toggleBackgroundColor(bool changeBColor) {
    makeCurrent();
    qDebug() << changeBColor;
    if (changeBColor)
        glClearColor(0, 0, 0, 1);
    else
        glClearColor(1, 1, 1, 1);
    update();
}
