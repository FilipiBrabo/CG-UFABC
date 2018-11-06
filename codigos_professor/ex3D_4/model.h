#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>

#include <fstream>
#include <limits>
#include <iostream>
#include <memory>
#include "material.h"
#include "trackball.h"

class Model : public QOpenGLExtraFunctions
{
public:
    Model(QOpenGLWidget *_glWidget);
    ~Model();

    std::unique_ptr<QVector4D []> vertices;
    std::unique_ptr<QVector3D []> normals;
    std::unique_ptr<unsigned int[]> indices;
    std::unique_ptr<QVector2D []>texCoords;

    QOpenGLWidget *glWidget;

    unsigned int numVertices;
    unsigned int numFaces;

    GLuint vao = 0;

    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboNormals = 0;
    GLuint vboTexCoords = 0;

    GLuint textureID = 0;

    Material material;
    int shaderIndex = 0;
    int numShaders = 0;
    std::vector<GLuint> shaderProgram;
    void createNormals();


    QMatrix4x4 modelMatrix;
    QVector3D midPoint;
    double invDiag=0;


    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void loadTexture(const QImage &image);

    void readOFFFile(const QString &fileName);

    void createTexCoords();

    void drawModel();

    TrackBall trackBall;
    };
 #endif // MODEL_H
