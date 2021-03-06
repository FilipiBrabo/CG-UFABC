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
    GLuint vboVertices = 0;
    GLuint vboColors = 0;
    GLuint vboIndices = 0;

    GLuint vao = 0;

    std::unique_ptr<QVector4D []> vertices = nullptr;
    std::unique_ptr<QVector4D []> colors = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;

    GLuint shaderProgram;

    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() ;

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();


signals:

private:
    QTimer timer;
    bool blinkFlag = false;

public slots:
    void toggleBackgroundColor(bool);
    void switchDiagonal(bool);

//    void changeV0Color(int num);
//    void changeV1Color(int num);
//    void changeV2Color(int num);
//    void changeV3Color(int num);
protected :
    void initializeGL();
    void resizeGL(int width , int height);
    void paintGL();
};

#endif // OPENGLWIDGET_H
