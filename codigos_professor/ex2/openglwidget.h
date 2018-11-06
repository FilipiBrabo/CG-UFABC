#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <memory>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

private:
    QTimer timer;
    bool blinkFlag = false;

public:

    GLuint vboVertices = 0;
    GLuint vboColors = 0;
    GLuint vboIndices = 0;

    GLuint vboCoordText = 0;
    GLuint textureID = 0;

    GLuint vao = 0;

    std::unique_ptr<QVector4D []> vertices = nullptr; //QVector4D *vertices = NULL;
    std::unique_ptr<QVector4D []> colors = nullptr;
    std::unique_ptr<QVector2D[]> coordText = nullptr;
    std::unique_ptr<unsigned int[]> indices = nullptr;


    GLuint shaderProgram;

    public:

    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void loadTexture(const QImage &image);

signals:

public slots:
    void toggleBackgroundColor(bool);
    void loadTexture();

protected:
    void initializeGL();
    void resizeGL(int,int);
    void paintGL();

};

#endif // OPENGLWIDGET_H
