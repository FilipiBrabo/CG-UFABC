#include "model.h"

Model::Model(QOpenGLWidget * _glWidget)
{
    glWidget = _glWidget;
    glWidget->makeCurrent();
    initializeOpenGLFunctions();

    shaderIndex = 0;
    numShaders = 0;
}

Model::~Model()
{
    destroyVBOs();
    destroyShaders();
}

void Model::createNormals()
{
    normals = std::make_unique<QVector3D []>(numVertices);

    // Compute face normals
    for (int i = 0; i < numFaces; ++i)
    {
       int idxA = indices[i * 3 + 0];
       int idxB = indices[i * 3 + 1];
       int idxC = indices[i * 3 + 2];
       QVector4D vtA = vertices[idxA];
       QVector4D vtB = vertices[idxB];
       QVector4D vtC = vertices[idxC];
       QVector3D BmA = (vtB - vtA).toVector3D();
       QVector3D CmB = (vtC - vtB).toVector3D();
       QVector3D faceNormal = QVector3D::crossProduct(BmA, CmB);
       faceNormal.normalize();

       normals[idxA] += faceNormal;
       normals[idxB] += faceNormal;
       normals[idxC] += faceNormal;
    }

    for (int i = 0; i < numVertices; ++i)
    {
        normals[i].normalize();
    }
}

void Model::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboNormals);
    glDeleteBuffers(1, &vboIndices);
    glDeleteVertexArrays(1, &vao);
    vboVertices = 0;
    vboIndices = 0;
    vboNormals = 0;
    vao = 0;
}

void Model::destroyShaders()
{
    for (GLuint shaderProgramID : shaderProgram)
    {
        glDeleteProgram (shaderProgramID);
    }
}

void Model::createVBOs()
{
    glWidget->makeCurrent();

    destroyVBOs();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector3D), normals.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);
}

void Model::createShaders()
{
    glWidget->makeCurrent();
    destroyShaders();
    shaderProgram.clear();


    numShaders = 8;
    QString vertexShaderFile [] = {":/shaders/vshader1.glsl",
                                   ":/shaders/vflat.glsl",
                                   ":/shaders/vgouraud.glsl",
                                   ":/shaders/vphong.glsl",
                                   ":/shaders/vnormal.glsl",
                                   ":/shaders/vphongnormal.glsl",
                                   ":/shaders/vtoon.glsl",
                                   ":/shaders/vphong2.glsl"};
    QString fragmentShaderFile [] = {":/shaders/fshader1.glsl",
                                     ":/shaders/fflat.glsl",
                                     ":/shaders/fgouraud.glsl",
                                     ":/shaders/fphong.glsl",
                                     ":/shaders/fnormal.glsl",
                                     ":/shaders/fphongnormal.glsl",
                                     ":/shaders/ftoon.glsl",
                                     ":/shaders/fphong2.glsl"};

    for(int i = 0; i < numShaders; ++i)
    {
        QFile vs(vertexShaderFile[i]);
        QFile fs(fragmentShaderFile[i]);

        vs.open(QFile::ReadOnly | QFile::Text);
        fs.open(QFile::ReadOnly | QFile::Text);

        QTextStream streamVs (& vs), streamFs (& fs);
        QString qtStringVs = streamVs . readAll ();
        QString qtStringFs = streamFs . readAll ();
        std :: string stdStringVs = qtStringVs . toStdString ();
        std :: string stdStringFs = qtStringFs . toStdString ();

        // Create an empty vertex shader handle
        GLuint vertexShader = 0;
        vertexShader = glCreateShader( GL_VERTEX_SHADER );
        // Send the vertex shader source code to GL
        const GLchar * source = stdStringVs . c_str ();
        glShaderSource ( vertexShader , 1, & source , 0);
        // Compile the vertex shader
        glCompileShader ( vertexShader );
        GLint isCompiled = 0;
        glGetShaderiv ( vertexShader , GL_COMPILE_STATUS , & isCompiled );
        if ( isCompiled == GL_FALSE )
        {
            GLint maxLength = 0;
            glGetShaderiv ( vertexShader , GL_INFO_LOG_LENGTH , & maxLength );
            // The maxLength includes the NULL character
            std :: vector <GLchar > infoLog ( maxLength );
            glGetShaderInfoLog (vertexShader, maxLength , &maxLength, &infoLog[0]);
            qDebug ("%s", & infoLog [0]);
            glDeleteShader ( vertexShader );
            return ;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = 0;
        fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );
        // Send the fragment shader source code to GL
        // Note that std :: string ’s . c_str is null terminated .
        source = stdStringFs . c_str ();
        glShaderSource ( fragmentShader , 1, & source , 0);
        // Compile the fragment shader
        glCompileShader ( fragmentShader );
        glGetShaderiv ( fragmentShader , GL_COMPILE_STATUS , & isCompiled );
        if ( isCompiled == GL_FALSE )
        {
            GLint maxLength = 0;
            glGetShaderiv ( fragmentShader , GL_INFO_LOG_LENGTH , & maxLength );
            std :: vector <GLchar > infoLog ( maxLength );
            glGetShaderInfoLog ( fragmentShader , maxLength , & maxLength , & infoLog[0]) ;
            qDebug ("%s", & infoLog [0]) ;
            glDeleteShader ( fragmentShader );
            glDeleteShader ( vertexShader );
            return ;
        }

        GLuint shaderProgramID = glCreateProgram ();
        shaderProgram.push_back(shaderProgramID);

        // Attach our shaders to our program
        glAttachShader ( shaderProgramID , vertexShader );
        glAttachShader ( shaderProgramID , fragmentShader );
        // Link our program
        glLinkProgram ( shaderProgramID );
        // Note the different functions here : glGetProgram *
        // instead of glGetShader *.
        GLint isLinked = 0;
        glGetProgramiv ( shaderProgramID , GL_LINK_STATUS , ( int *)& isLinked );
        if ( isLinked == GL_FALSE )
        {
            GLint maxLength = 0;
            glGetProgramiv ( shaderProgramID , GL_INFO_LOG_LENGTH , & maxLength );
            // The maxLength includes the NULL character
            std :: vector <GLchar> infoLog ( maxLength );
            glGetProgramInfoLog ( shaderProgramID , maxLength , & maxLength , & infoLog[0]) ;
            qDebug ("%s", & infoLog [0]) ;
            glDeleteProgram ( shaderProgramID );
            glDeleteShader ( vertexShader );
            glDeleteShader ( fragmentShader );
            return ;
        }

        glDetachShader(shaderProgramID, vertexShader);
        glDetachShader(shaderProgramID, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        vs.close();
        fs.close();
    }
}

void Model::drawModel(float angleY)
{
    // The matrix is composed on the CPU
    modelMatrix.setToIdentity();
    //modelMatrix.rotate(angleY, 0, 1, 0);
    modelMatrix.rotate(trackBall.getRotation());
    modelMatrix.scale(invDiag, invDiag, invDiag);
    modelMatrix.translate(-midPoint);

    glBindVertexArray(vao);
    glUseProgram (shaderProgram[shaderIndex]);

    // Now we send the matrix to the GPU
    GLuint locModelMatrix = glGetUniformLocation(shaderProgram[shaderIndex], "modelMatrix");
    glUniformMatrix4fv(locModelMatrix, 1, GL_FALSE, modelMatrix.data());

    GLuint locNormalMatrix = glGetUniformLocation(shaderProgram[shaderIndex], "normalMatrix");
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, modelMatrix.normalMatrix().data());

    GLuint locShininess = glGetUniformLocation(
                shaderProgram[shaderIndex],
                "shininess");
    glUniform1f(locShininess, material.shininess);

    glDrawElements (GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}

void Model::readOFFFile(QString const & fileName)
{
    std::ifstream stream;
    stream.open(fileName.toUtf8(), std::ifstream::in);
    if (!stream.is_open())
    {
        qWarning("Cannot open file .");
        return;
    }

    std::string line;
    stream >> line;
    stream >> numVertices >> numFaces >> line;
    vertices = std::make_unique<QVector4D []>(numVertices);
    indices = std::make_unique<unsigned int []>(numFaces * 3);

    if (numVertices > 0)
    {
        float minLim = std::numeric_limits<float>::lowest();
        float maxLim = std::numeric_limits<float>::max();
        QVector4D max(minLim, minLim, minLim, 1.0);
        QVector4D min(maxLim, maxLim, maxLim, 1.0);
        for (unsigned int i = 0; i < numVertices ; ++i)
        {
            float x, y, z;
            stream >> x >> y >> z;
            max.setX(std::max(max.x(), x));
            max.setY(std::max(max.y(), y));
            max.setZ(std::max(max.z(), z));
            min.setX(std::min(min.x(), x));
            min.setY(std::min(min.y(), y));
            min.setZ(std::min(min.z(), z));
            vertices[i] = QVector4D(x, y, z, 1.0);
        }

        midPoint = QVector3D((min + max) * 0.5);
        // sqrt (3) is the diagonal of a unitary cube
        invDiag = std::sqrt(3.0) / (max - min).length();

        for (unsigned int i = 0; i < numFaces; ++i)
        {
            unsigned int a, b, c;
            stream >> line >> a >> b >> c;
            indices [i * 3 + 0] = a;
            indices [i * 3 + 1] = b;
            indices [i * 3 + 2] = c;
        }
        stream.close();
        createShaders();
        createNormals();
        createVBOs();
    }
}
