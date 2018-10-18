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

    GLuint vboVerticesMoeda = 0;
    GLuint vboColorsMoeda = 0;
    GLuint vboIndicesMoeda = 0;


    GLuint vaoPacman = 0;
    GLuint vaoMoeda = 0;

    std::unique_ptr<QVector4D []> vertices = nullptr;
    std::unique_ptr<QVector4D []> colors = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;

    GLuint shaderProgram;

    float playerPosX = -0.6f;
    float playerPosY = -0.6f;
    float raio = 0.2f;
    unsigned int numVertices, numFaces;

    QTimer timer;
    QTime time;

public:
    explicit OpenGLWidget (QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createPacmanVBO();
    void createMoedaVBO();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawPacman();
    void drawMoeda();

protected :
    void initializeGL();
    void resizeGL (int width, int height);
    void paintGL();


public slots:
    void animate();
};
#endif
