#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "model.h"
#include "light.h"
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

    std::shared_ptr<Model> model = nullptr;
    void keyPressEvent(QKeyEvent *event);

    Light light;
    Camera camera;

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    QTimer timer;


protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();



signals:
    void statusBarMessage(QString);
    void enableComboShaders(bool);

public slots:
    void showFileOpenDialog();
    void changeShader(int);
    void animate();
    void loadTexture();

};

#endif // OPENGLWIDGET_H
