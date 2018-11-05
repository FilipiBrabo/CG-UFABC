#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget (parent)
{
    forwardSpeed = 0;
    rotateSpeed = 0;
    strafeSpeed = 0;
    angleY = 0;
    time.start();
    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.3f, 0.3f, 0.3f, 1);

    qDebug(" OpenGL version : %s", glGetString(GL_VERSION));
    qDebug(" GLSL %s", glGetString (GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
    camera.resizeViewport(w, h);

    if (model)
        model->trackBall.resizeViewport(w , h);
}

void OpenGLWidget::paintGL()
{
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!model)
        return;

    float speedScale = 0.1f;
    camera.strafe(strafeSpeed * speedScale);
    camera.moveForward(forwardSpeed * speedScale);

    float rotateScale = 1;
    camera.turn(rotateSpeed * rotateScale);

    int shaderProgramID = model->shaderProgram[model->shaderIndex];

    QVector4D ambientProduct = light.ambient * model->material.ambient;

    QVector4D diffuseProduct[2];
    diffuseProduct[0] = light.diffuse[0] * model->material.diffuse;
    diffuseProduct[1] = light.diffuse[1] * model->material.diffuse;

    QVector4D specularProduct[2];
    specularProduct[0] = light.specular[0] * model->material.specular;
    specularProduct[1] = light.specular[1] * model->material.specular;

    GLuint locProjection = glGetUniformLocation(shaderProgramID, "projectionMatrix");
    GLuint locView = glGetUniformLocation(shaderProgramID, "viewMatrix");
    GLuint locLightPosition = glGetUniformLocation(shaderProgramID, "lightPosition");
    GLuint locAmbientProduct = glGetUniformLocation(shaderProgramID, "ambientProduct");
    GLuint locDiffuseProduct = glGetUniformLocation(shaderProgramID, "diffuseProduct");
    GLuint locSpecularProduct = glGetUniformLocation(shaderProgramID, "specularProduct");

    glUniformMatrix4fv(locProjection, 1, GL_FALSE, camera.projectionMatrix.data());
    glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
    glUniform4fv(locLightPosition, 2, &(light.position[0][0]));
    glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
    glUniform4fv(locDiffuseProduct, 2, &(diffuseProduct[0][0]));
    glUniform4fv(locSpecularProduct, 2, &(specularProduct[0][0]));

    model->drawModel(angleY);
}

void OpenGLWidget::animate()
{
    float speed = 10;
    float elapsedTime = time.restart() / 1000.0f;
    angleY = angleY + elapsedTime * speed;
    if (angleY >= 360)
        angleY -= 360;

    update();
}

// "OFF Files (*.off)"

void OpenGLWidget::showFileOpenDialog()
{
    QByteArray fileFormat = "off";
    QString fileName = QFileDialog::getOpenFileName(
                this, "Open File", QDir::homePath(),
                QString("\%1 Files (*.\%2)")
                .arg(QString(fileFormat.toUpper()))
                .arg(QString(fileFormat)), 0
#ifdef Q_OS_LINUX
, QFileDialog::DontUseNativeDialog
#endif
    );
    if(!fileName.isEmpty())
    {
        model = std::make_shared<Model>(this);
        model->readOFFFile(fileName);
        model->trackBall.resizeViewport(width(), height());
        emit statusBarMessage(QString("Vertices: \%1, Faces: \%2")
        .arg(model->numVertices).arg(model->numFaces));
        emit enableComboShaders(true);
    }
    update();
}

void OpenGLWidget::changeShader(int shaderIndex)
{
    if (!model)
        return;

    model->shaderIndex = shaderIndex;
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        forwardSpeed = 1;
    if (event->key() == Qt::Key_S)
        forwardSpeed = -1;
    if (event->key() == Qt::Key_A)
        rotateSpeed = -1;
    if (event->key() == Qt::Key_D)
        rotateSpeed = 1;
    if (event->key() == Qt::Key_Q)
        strafeSpeed = -1;
    if (event->key() == Qt::Key_E)
        strafeSpeed = 1;

    update();
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W && forwardSpeed > 0)
        forwardSpeed = 0;
    if (event->key() == Qt::Key_S && forwardSpeed < 0)
        forwardSpeed = 0;
    if (event->key() == Qt::Key_A && rotateSpeed < 0)
        rotateSpeed = 0;
    if (event->key() == Qt::Key_D && rotateSpeed > 0)
        rotateSpeed = 0;
    if (event->key() == Qt::Key_Q && strafeSpeed < 0)
        strafeSpeed = 0;
    if (event->key() == Qt::Key_E && strafeSpeed > 0)
        strafeSpeed = 0;

    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (! model )
        return ;
    model->trackBall.mouseMove (event -> localPos ());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (! model )
        return ;
    if (event -> button () & Qt :: LeftButton )
        model -> trackBall . mousePress (event -> localPos ());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (! model )
        return ;
    if (event -> button () & Qt :: LeftButton )
        model -> trackBall . mouseRelease (event -> localPos ());
}

