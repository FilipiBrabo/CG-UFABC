#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>

#include "model.h"
#include "light.h"
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    int m_width, m_height;

    float angleY;

    QTime time;
    QTimer timer;

    Camera camera;
    Light light;

    float forwardSpeed;
    float rotateSpeed;
    float strafeSpeed;

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    std::shared_ptr<Model> model = nullptr;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void statusBarMessage(QString);
    void enableComboShaders(bool);

public slots:
    void showFileOpenDialog();
    void animate();
    void changeShader(int);
};

#endif // OPENGLWIDGET_H
