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
    glClearColor(0, 0, 0, 1);

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

    drawPacman();
    drawMoeda();
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

    createPacmanVBO();
    createMoedaVBO();
}

void OpenGLWidget::createPacmanVBO() {
    numVertices = 50;
    numFaces = 48;

    vertices = std::make_unique<QVector4D []>(numVertices);
    colors = std::make_unique<QVector4D []>(numVertices);
    indices = std::make_unique<unsigned int []>(numFaces * 3);

    float angulo = (2.0f * 3.14159265f)/ (float)(numVertices-2);

    //circle origin
    vertices[0] = QVector4D(playerPosX, playerPosY, 0, 1);

    // vertices of the circle
    for (int i = 1; i < numVertices; i++) {
        float cosAngulo = cos((i-1) * angulo);
        float senAngulo = sin((i-1) * angulo);

        float posX = playerPosX + raio*cosAngulo;
        float posY = playerPosY + raio*senAngulo;

        vertices[i] = QVector4D(posX, posY, 0, 1);
    }

    for (int i = 0; i < numVertices; i++){
        colors[i] = QVector4D(1, 1, 0, 1);
    }

    // Topology of the mesh (pacman && coins)
    for (int i = 15; i < numFaces*3 - 15; i+=3) {
        indices[i] = 0;
        indices[i+1] = i/3 + 1;
        indices[i+2] = i/3 + 2;
    }

    glGenVertexArrays(1, &vaoPacman);
    glBindVertexArray(vaoPacman);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_DYNAMIC_DRAW);

void OpenGLWidget::createMoedaVBO() {
        numVertices = 50;
        numFaces = 48;

        float angulo = (2.0f * 3.14159265f)/ (float)(numVertices-2);

        //circle origin
        vertices[0] = QVector4D(playerPosX, playerPosY, 0, 1);

        // vertices of the circle
        for (int i = 1; i < numVertices; i++) {
            float cosAngulo = cos((i-1) * angulo);
            float senAngulo = sin((i-1) * angulo);

            float posX = playerPosX + raio*cosAngulo;
            float posY = playerPosY + raio*senAngulo;

            vertices[i] = QVector4D(posX, posY, 0, 1);
        }

        for (int i = 0; i < numVertices; i++){
            colors[i] = QVector4D(1, 1, 1, 1);
        }

        // Topology of the mesh (pacman && coins)
        for (int i = 0; i < numFaces*3; i+=3) {
            indices[i] = 0;
            indices[i+1] = i/3 + 1;
            indices[i+2] = i/3 + 2;
        }

        glGenVertexArrays(1, &vaoMoeda);
        glBindVertexArray(vaoMoeda);

        glGenBuffers(1, &vboVerticesMoeda);
        glBindBuffer(GL_ARRAY_BUFFER, vboVerticesMoeda);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &vboColorsMoeda);
        glBindBuffer(GL_ARRAY_BUFFER, vboColorsMoeda);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), colors.get(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &vboIndicesMoeda);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesMoeda);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_DYNAMIC_DRAW);
}

void OpenGLWidget::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
    glDeleteBuffers(1, &vboIndices);
    glDeleteBuffers(1, &vboVerticesMoeda);
    glDeleteBuffers(1, &vboColorsMoeda);
    glDeleteBuffers(1, &vboIndicesMoeda);

    glDeleteVertexArrays(1, &vaoPacman);
    glDeleteVertexArrays(1, &vaoMoeda);

    vboVertices = 0;
    vboIndices = 0;
    vboColors = 0;
    vaoPacman = 0;

    vboVerticesMoeda = 0;
    vboIndicesMoeda = 0;
    vboColorsMoeda = 0;
    vaoMoeda = 0;
}

void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    // Change player X position
    playerPosX += 1.5f * elapsedTime;


    // Check if pacman is out of window
    if (playerPosX >= 1.5f && playerPosY > 1) {
        playerPosX = -1.0f;
        playerPosY = -0.6f;
    }
    if (playerPosX >= 1.5f) {
        playerPosY += 0.2;
        playerPosX = -1.0;
    }

    update();
}

void OpenGLWidget::drawPacman() {

    GLint locScaling = glGetUniformLocation(shaderProgram, "scaling");
    GLint locTranslation = glGetUniformLocation(shaderProgram, "translation");

    glUseProgram(shaderProgram);

    glBindVertexArray(vaoPacman);

    // Pacman
    glUniform4f(locTranslation, playerPosX, playerPosY, 0, 0);
    glUniform1f(locScaling, 0.5);
    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}

void OpenGLWidget::drawMoeda() {
    GLint locScaling = glGetUniformLocation(shaderProgram, "scaling");
    GLint locTranslation = glGetUniformLocation(shaderProgram, "translation");

    glUseProgram(shaderProgram);

    glBindVertexArray(vaoMoeda); //se mudo pro vaoMoeda, não funciona

    for (float i = -1.0f; i < 1.1f; i+= 0.1f) {

            if (playerPosX < i + 0.2f) {
                glUniform4f(locTranslation, i, playerPosY - 0.24f, 0, 0);
                glUniform1f(locScaling, 0.1f);
                glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
            }
    }

    // Draw all the coins that are above pacman
    for (float i = -1.0f; i < 1.1f; i+= 0.1f) {
        for (float j = playerPosY - 0.04f; j < 1.0f; j+= 0.2f) {
                glUniform4f(locTranslation, i, j, 0, 0);
                glUniform1f(locScaling, 0.1);
                glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
            }
    }
}


