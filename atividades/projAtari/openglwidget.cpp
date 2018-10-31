#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.339412, 0.339412, 0.339412, 1);

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    createShaders();
    createVBOs();

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    time.start();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

//    drawPacman();
//    drawMoeda();
    drawCar();
}

void OpenGLWidget::createShaders()
{
    // makeCurrent();
    destroyShaders();

    QFile vs(":/shaders/vshader1.glsl");
    QFile fs(":/shaders/fshader1.glsl");

    vs.open(QFile::ReadOnly | QFile::Text);
    fs.open(QFile::ReadOnly | QFile::Text);

    QTextStream streamVs(&vs), streamFs(&fs);

    QString qtStringVs = streamVs.readAll();
    QString qtStringFs = streamFs.readAll();

    std::string stdStringVs = qtStringVs.toStdString();
    std::string stdStringFs = qtStringFs.toStdString();

    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    const GLchar *source = stdStringVs.c_str();

    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteShader(vertexShader);
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = stdStringFs.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    shaderProgram = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link our program
    glLinkProgram(shaderProgram);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);

        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vs.close();
    fs.close();
}

void OpenGLWidget::destroyShaders()
{
    makeCurrent();
    glDeleteProgram(shaderProgram);
}

void OpenGLWidget::createVBOs()
{
    makeCurrent();
    destroyVBOs();

    createCarVBO();
}

void OpenGLWidget::createCarVBO() {
    numVerticesCar = 4;
    numFacesCar = 2;

    vertices = std::make_unique<QVector4D []>(numVerticesCar);
    colors = std::make_unique<QVector4D []>(numVerticesCar);
    indices = std::make_unique<unsigned int []>(numFacesCar);

    //vertices of the car
    vertices[0] = QVector4D(-0.25, -0.25, 0, 1);
    vertices[1] = QVector4D(0.25, -0.25, 0, 1);
    vertices[2] = QVector4D(0.25, 0.25, 0, 1);
    vertices[3] = QVector4D(-0.25, 0.25, 0, 1);

    //Color of the car
    for (int i = 0; i < numVerticesCar; i++){
        colors[i] = QVector4D(1, 0, 0, 1);
    }

    //Topology of the car mesh
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

    glGenVertexArrays(1, &vaoCar);
    glBindVertexArray(vaoCar);

    glGenBuffers(1, &vboVerticesCar);
    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesCar);
    glBufferData(GL_ARRAY_BUFFER, numVerticesCar * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboColorsCar);
    glBindBuffer(GL_ARRAY_BUFFER, vboColorsCar);
    glBufferData(GL_ARRAY_BUFFER, numVerticesCar * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vboIndicesCar);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesCar);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFacesCar * 3 * sizeof(unsigned int), indices.get(), GL_DYNAMIC_DRAW);

}


void OpenGLWidget::destroyVBOs()
{
    glDeleteBuffers(1, &vboVerticesCar);
    glDeleteBuffers(1, &vboColorsCar);
    glDeleteBuffers(1, &vboIndicesCar);

    glDeleteVertexArrays(1, &vaoCar);

    vboVerticesCar = 0;
    vboIndicesCar = 0;
    vboColorsCar = 0;
    vaoCar = 0;
}

void OpenGLWidget::animate()
{
//    float elapsedTime = time.restart() / 1000.0f;

//    // Change player X position
//    playerPosX += 1.5f * elapsedTime;


//    // Check if pacman is out of window
//    if (playerPosX >= 1.5f && playerPosY > 1) {
//        playerPosX = -1.0f;
//        playerPosY = -0.6f;
//    }
//    if (playerPosX >= 1.5f) {
//        playerPosY += 0.2;
//        playerPosX = -1.0;
//    }

    update();
}
void OpenGLWidget::drawCar() {
    GLint locTranslation = glGetUniformLocation(shaderProgram, "translation");
    GLint locScaling = glGetUniformLocation(shaderProgram, "scaling");

    glUseProgram(shaderProgram);

    glBindVertexArray(vaoCar);

    glUniform4f(locTranslation, 0, -0.75, 0, 0);
    glUniform1f(locScaling, 1.0);
    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}


