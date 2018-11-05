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

    float playerPosX = 0;
    float playerPosY = -0.8f;
    float distanceOffset = 0.08f;


    float enemiesPosX[3] = {-0.4f, 0, 0.4f};
    float enemiesPosY[3] = {-1, 0, -1};

    unsigned int numVertices, numFaces, numVerticesCar, numFacesCar;
    float speed = 0;
    bool collided = false;

    QTimer timer;
    QTime time;

    int a =0;

public:
    explicit OpenGLWidget (QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createCarVBO();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawCar(float x, float y);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

protected :
    void initializeGL();
    void resizeGL (int width, int height);
    void paintGL();


public slots:
    void animate();
};
#endif
