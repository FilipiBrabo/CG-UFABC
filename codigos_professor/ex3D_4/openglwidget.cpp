#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    connect(& timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL version %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.resizeViewport(width, height);

    if (model)
        model->trackBall.resizeViewport(width, height);
    update();
}

void OpenGLWidget::paintGL()
{
    glClearColor(0,0,0,1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!model)
        return;

    glUseProgram(model->shaderProgram[model->shaderIndex]);

    GLuint shaderProgramID = model->shaderProgram[model->shaderIndex];

    QVector4D ambientProduct = light.ambient * model->material.ambient;
    QVector4D diffuseProduct = light.diffuse *  model->material.diffuse;
    QVector4D specularProduct = light.specular *  model->material.specular;
    GLint locProjection = glGetUniformLocation(shaderProgramID,  "projection");
    GLint locView = glGetUniformLocation(shaderProgramID, "view");
    GLint locLightPosition = glGetUniformLocation(shaderProgramID,  "lightPosition");
    GLint locAmbientProduct = glGetUniformLocation(shaderProgramID, "ambientProduct");
    GLint locDiffuseProduct = glGetUniformLocation(shaderProgramID, "diffuseProduct");
    GLint locSpecularProduct = glGetUniformLocation(shaderProgramID,"specularProduct");

    glUniformMatrix4fv(locProjection, 1, GL_FALSE, camera.projectionMatrix.data());
    glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
    glUniform4fv(locLightPosition, 1, &(light.position[0]));
    glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
    glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
    glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));

    model->drawModel();
}

void OpenGLWidget::changeShader(int shaderIndex)
{
    if(!model)
        return;
    model->shaderIndex=shaderIndex;
    update();
}


void OpenGLWidget::showFileOpenDialog()
{
    QByteArray fileFormat = "off";
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open File", QDir::homePath(),
        QString("\%1 Files (*.\%2)").arg(QString(fileFormat.toUpper())).arg(QString(fileFormat)), 0
#ifdef Q_OS_LINUX
        , QFileDialog::DontUseNativeDialog
#endif
    );
    int shaderIndex = 0;
    if (!fileName.isEmpty())
    {
        if (model)
            shaderIndex = model->shaderIndex;
        model = std::make_shared<Model>(this);
        model->shaderIndex = shaderIndex;
        model->readOFFFile(fileName);
        model->trackBall.resizeViewport(width(), height());
        emit statusBarMessage(QString("Vertices: \%1, Faces: \%2").arg(model->numVertices).arg(model->numFaces));
        emit enableComboShaders(true);
    }
    update();
}

void OpenGLWidget::animate()
{
    update();
}


 void OpenGLWidget::keyPressEvent(QKeyEvent *event)
 {
     switch(event->key())
     {
     case Qt::Key_Escape:
         qApp->quit();
     }
 }


 void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     if (!model)
         return;
     model->trackBall.mouseMove(event->localPos());
 }

 void OpenGLWidget::mousePressEvent(QMouseEvent *event)
 {
     if (!model)
         return;
     if (event->button() & Qt::LeftButton)
         model->trackBall.mousePress(event->localPos());
 }

 void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
 {
     if (!model)
         return;
     if (event->button() & Qt::LeftButton)
         model->trackBall.mouseRelease(event->localPos());
 }

 void OpenGLWidget :: loadTexture () {
     QString fileName = QFileDialog :: getOpenFileName (this ,
                                                        QString ("Open Image File") , "",
                                                        QString ("JPEG (*.jpg *.jpeg);;PNG (*.png )") , 0
        # ifdef Q_OS_LINUX
             , QFileDialog :: DontUseNativeDialog
        # endif
             ) ;
     if (! fileName . isEmpty () && model ) {
         QImage image ;
         image . load ( fileName ) ;
         image = image . convertToFormat ( QImage :: Format_RGBA8888 ) ;
         model->loadTexture(image);
     }
     update () ;
 }
