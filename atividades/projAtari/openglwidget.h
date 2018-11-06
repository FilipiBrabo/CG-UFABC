#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QRandomGenerator>

#include <memory>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

    QRandomGenerator rd;

    GLuint vboVerticesRoadMark = 0;
    GLuint vboIndicesRoadMark = 0;
    GLuint vboColorsRoadMark = 0;
    GLuint vaoRoadMark = 0;

    GLuint vboVerticesGrass = 0;
    GLuint vboIndicesGrass = 0;
    GLuint vboColorsGrass = 0;
    GLuint vaoGrass = 0;

    GLuint vboVerticesCar = 0;
    GLuint vboColorsCar = 0;
    GLuint vboIndicesCar = 0;
    GLuint vaoCar = 0;

    std::unique_ptr<QVector4D []> vertices = nullptr;
    std::unique_ptr<QVector4D []> colors = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;

    GLuint shaderProgram;

    std::unique_ptr<QVector2D []> playerPos = nullptr;
    std::unique_ptr<QVector2D []> enemiePos = nullptr;
    std::unique_ptr<QVector2D []> roadMarkPos = nullptr;

    float distanceOffset = 0.095f;

    unsigned int numVertices, numFaces, numVerticesCar, numFacesCar;

    float speed = 0;
    bool collided = false;

    QTimer timer;
    QTime time;

    int score = 0;

public:
    explicit OpenGLWidget (QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createCarVBO();
    void createRoadMarkVBO();
    void createGrassVBO();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawCar(QVector2D);
    void drawRoadMark(QVector2D);
    void drawGrass(float x);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    float generatePosX();
    float generatePosY();

protected :
    void initializeGL();
    void resizeGL (int width, int height);
    void paintGL();


public slots:
    void animate();
    void restartGame();
    void exitGame();

signals:
    void updateScore(QString);
    void showBtn();
    void hideBtn();
};
#endif
