#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QTextStream>
#include <QFile>

#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

# include "material.h"
#include "trackball.h"

class Model : public QOpenGLExtraFunctions
{
    std::unique_ptr<QVector4D []> vertices;
    std::unique_ptr<QVector3D []> normals;
    std::unique_ptr<unsigned int []> indices;

    int numShaders;

    QOpenGLWidget * glWidget;

    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboNormals = 0;
    GLuint vboIndices = 0;

    QMatrix4x4 modelMatrix;
    QVector3D midPoint;
    double invDiag;       

public:
    unsigned int numVertices;
    unsigned int numFaces;

    std::vector<GLuint> shaderProgram;
    int shaderIndex;

    Material material;
    TrackBall trackBall;

    Model(QOpenGLWidget * _glWidget);
    ~Model();

    void createNormals();
    void createVBOs();
    void createShaders();
    void destroyVBOs();
    void destroyShaders();

    void readOFFFile(const QString & fileName);

    void drawModel(float angleY);
};

#endif // MODEL_H
