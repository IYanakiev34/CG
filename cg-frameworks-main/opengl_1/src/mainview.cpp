#include "mainview.h"

#include <QDateTime>
#include "model.h"
#include "myvertex.h"

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
    glDeleteVertexArrays(1,&d_pyrVAO);
    glDeleteBuffers(1,&d_pyrVBO);
    glDeleteBuffers(1,&d_pyrIBO);
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

    // Create pyramid
    createPyramid();

    // Create knot
    createKnot();

    // Initialize to identity the rotation and scaling matrices
    d_rotation.setToIdentity();
    d_scale.setToIdentity();

    // Set up projection matrix also constant for now so we can leave it here
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

    // Set the model matrix
    d_pyrModel = d_pyrTrans * d_rotation * d_scale;

    // Send the uniform matrices
    glUniformMatrix4fv(d_modLocation,1,GL_FALSE,d_pyrModel.data());
    glUniformMatrix4fv(d_projLocation,1,GL_FALSE,d_proj.data());

    // Draw pyramid
    glBindVertexArray(d_pyrVAO);
    glDrawElements(GL_TRIANGLES,d_pyrElements,GL_UNSIGNED_INT,nullptr);

    // Set the model matrix for the knot and send it to the GPU
    d_knotModel = d_knotTrans * d_rotation * d_scale;
    glUniformMatrix4fv(d_modLocation,1,GL_FALSE,d_knotModel.data());
    glUniformMatrix4fv(d_projLocation,1,GL_FALSE,d_proj.data());

    // Draw knot
    glBindVertexArray(d_knotVAO);
    glDrawElements(GL_TRIANGLES,d_knotElements,GL_UNSIGNED_INT,nullptr);

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
    QMatrix4x4 x_rot,y_rot,z_rot;
    x_rot.rotate(rotateX,1,0,0);
    y_rot.rotate(rotateY,0,1,0);
    z_rot.rotate(rotateZ,0,0,1);

    d_rotation = x_rot*y_rot*z_rot;

    update();
}

/**
 * @brief MainView::setScale Changes the scale of the displayed objects.
 * @param scale The new scale factor. A scale factor of 1.0 should scale the
 * mesh to its original size.
 */
void MainView::setScale(float scale) {
    d_scale.setToIdentity();
    d_scale.scale(scale);

    update();
}

/**
 * @brief MainView::onMessageLogged OpenGL logging function, do not change.
 *
 * @param Message The message to be logged.
 */
void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
    qDebug() << " → Log:" << Message;
}

void MainView::createPyramid()
{
    // Create the vertex array object and bind it
    glGenVertexArrays(1,&d_pyrVAO);
    glBindVertexArray(d_pyrVAO);

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
    glGenBuffers(1,&d_pyrVBO);
    glBindBuffer(GL_ARRAY_BUFFER,d_pyrVBO);
    glBufferData(GL_ARRAY_BUFFER,6*5*sizeof(float),vertData,GL_STATIC_DRAW);

    // pyramid indices
    unsigned ind[]
    {
        0,4,1,
        1,4,2,
        2,4,3,
        0,3,4
    };
    d_pyrElements = 12;

    // Create and bind the IBO
    glGenBuffers(1,&d_pyrIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,d_pyrIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*6*sizeof(unsigned),ind,GL_STATIC_DRAW);

    // Create the vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void *)(3*sizeof(float)));


    // Initialize matrices for transformation
    d_pyrTrans.translate(QVector3D(-2,0,-6));
}

void MainView::createKnot()
{
    // Create vertex array object for the knot
    glGenVertexArrays(1,&d_knotVAO);
    glBindVertexArray(d_knotVAO);

    // Create the model
    Model knotModel(":/models/knot.obj");
    QVector<QVector3D> knotVerts = knotModel.getVertices_indexed();

    QVector<MyVertex> verts;
    verts.reserve(knotVerts.size());

    // Convert to my vertex format
    for (std::size_t idx = 0; idx != knotVerts.size(); ++idx)
        verts.append(MyVertex(QVector3D(knotVerts[idx]),QVector3D(abs(knotVerts[idx].x()),abs(knotVerts[idx].y()),abs(knotVerts[idx].z()))));


    // Create the VBO
    glGenBuffers(1,&d_knotVBO);
    glBindBuffer(GL_ARRAY_BUFFER,d_knotVBO);
    glBufferData(GL_ARRAY_BUFFER,verts.size() * sizeof(MyVertex),verts.data(),GL_STATIC_DRAW);

    // Create IBO
    d_knotElements = knotModel.getIndices().size();
    glGenBuffers(1,&d_knotIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,d_knotIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,knotModel.getIndices().size() * sizeof(unsigned),knotModel.getIndices().data(),GL_STATIC_DRAW);

    // Enable attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(MyVertex),0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(MyVertex),(void *)(sizeof(QVector3D)));

    // Set up the translation of the knot
    d_knotTrans.translate(QVector3D(2,0,-6));
}
