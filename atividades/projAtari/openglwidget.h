#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include <memory>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    GLuint vboVertices = 0;
    GLuint vboColors = 0;
    GLuint vboIndices = 0;

    GLuint vboVerticesCar = 0;
    GLuint vboColorsCar = 0;
    GLuint vboIndicesCar = 0;

    GLuint vaoCar = 0;

    std::unique_ptr<QVector4D []> vertices = nullptr;
    std::unique_ptr<QVector4D []> colors = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;

    GLuint shaderProgram;

    float playerPosX = 0.0f;
    float playerPosY = -1.0f;
    float raio = 0.2f;
    unsigned int numVertices, numFaces, numVerticesCar, numFacesCar;

    QTimer timer;
    QTime time;

public:
    explicit OpenGLWidget (QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createCarVBO();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawCar();

protected :
    void initializeGL();
    void resizeGL (int width, int height);
    void paintGL();


public slots:
    void animate();
};
#endif
