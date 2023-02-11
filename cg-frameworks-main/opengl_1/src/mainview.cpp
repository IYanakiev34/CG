#include "mainview.h"

#include <QDateTime>

/**
 * @brief MainView::MainView Constructs a new main view.
 *
 * @param parent Parent widget.
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    qDebug() << "MainView destructor";

    makeCurrent();
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&IBO);
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions.
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
            SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }

    QString glVersion{reinterpret_cast<const char *>(glGetString(GL_VERSION))};
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background
    // color.
    glClearColor(0.37f, 0.42f, 0.45f, 0.0f);

    // Creatye shader and get uniform locations
    createShaderProgram();
    d_modLocation = shaderProgram.uniformLocation("modelMat");
    d_projLocation = shaderProgram.uniformLocation("projMat");

    // Create the vertex array object and bind it
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    // pyramid vertices
    float vertData[] =
    {
        -1.f,1.f,1.f,1.f,0.f,0.f,
        1.f,1.f,1.f,0.f,1.f,0.f,
        1.f,-1.f,1.f,1.f,1.f,0.f,
        -1.f,-1.f,1.f,0.f,0.f,1.f,
        0.f,0.f,-1.f,1.f,0.f,1.f,
    };

    // Create vertex array buffer
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,6*5*sizeof(float),vertData,GL_STATIC_DRAW);

    // pyramid indices
    unsigned ind[]
    {
        0,4,1,
        1,4,2,
        2,4,3,
        0,3,4
    };
    // Create and bind the IBO
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*6*sizeof(unsigned),ind,GL_STATIC_DRAW);

    // Create the vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void *)(3*sizeof(float)));


    // Set up the model and projection matrices
    d_model.translate(QVector3D(-2,0,-6));
    d_aspectRatio = (float) this->width() / (float) this->height();
    d_proj.perspective(60.f,d_aspectRatio,0.2f,20.f);
}

/**
 * @brief MainView::createShaderProgram Creates a new shader program with a
 * vertex and fragment shader.
 */
void MainView::createShaderProgram() {
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/fragshader.glsl");

    shaderProgram.link();
}

/**
 * @brief MainView::paintGL Actual function used for drawing to the screen.
 *
 */
void MainView::paintGL() {
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Send the uniform matrices
    glUniformMatrix4fv(d_modLocation,1,GL_FALSE,d_model.data());
    glUniformMatrix4fv(d_projLocation,1,GL_FALSE,d_proj.data());

    // Draw here
    glBindVertexArray(VAO);
    // TODO: could fix magic number here
    glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,nullptr);

    shaderProgram.release();
}

/**
 * @brief MainView::resizeGL Called upon resizing of the screen.
 *
 * @param newWidth The new width of the screen in pixels.
 * @param newHeight The new height of the screen in pixels.
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    // update aspect ratio
    d_aspectRatio = (float) newWidth / (float) newHeight;

    // update projection matrix
    QMatrix4x4 tmp;
    tmp.perspective(60,d_aspectRatio,0.2f,20.f);
    d_proj = tmp;
}

/**
 * @brief MainView::setRotation Changes the rotation of the displayed objects.
 * @param rotateX Number of degrees to rotate around the x axis.
 * @param rotateY Number of degrees to rotate around the y axis.
 * @param rotateZ Number of degrees to rotate around the z axis.
 */
void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << ","
             << rotateZ << ")";
    Q_UNIMPLEMENTED();
}

/**
 * @brief MainView::setScale Changes the scale of the displayed objects.
 * @param scale The new scale factor. A scale factor of 1.0 should scale the
 * mesh to its original size.
 */
void MainView::setScale(float scale) {
    qDebug() << "Scale changed to " << scale;
    Q_UNIMPLEMENTED();
}

/**
 * @brief MainView::onMessageLogged OpenGL logging function, do not change.
 *
 * @param Message The message to be logged.
 */
void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
    qDebug() << " → Log:" << Message;
}
