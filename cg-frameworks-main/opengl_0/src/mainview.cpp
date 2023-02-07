#include "mainview.h"

/**
 * @brief MainView::MainView Mainview constructor. The ": QOpenGLWidget(parent)"
 * notation means calling the super class constructor of QOpenGLWidget with the
 * parameter "parent".
 * @param parent Parent widget.
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "MainView constructor";
}

/**
 * @brief MainView::~MainView Destructor. Can be used to free memory.
 */
MainView::~MainView() {
  qDebug() << "MainView destructor";

  makeCurrent();
  glDeleteBuffers(1,&d_indexBuffer);
  glDeleteBuffers(1,&d_vertexBuffer);
  glDeleteVertexArrays(1,&d_vao);
}

/**
 * @brief MainView::initializeGL Initialize the necessary OpenGL context.
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

  QOpenGLShaderProgram prog;
  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/vertshader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/fragshader.glsl");
  prog.link();
  prog.bind();

  glGenVertexArrays(1,&d_vao);
  glBindVertexArray(d_vao);

  glGenBuffers(1,&d_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,d_vertexBuffer);
  float pos[15] =
  {
      -1.f,-1.f,1.f,1.f,0.f,
      0.f,1.f,1.f,0.f,1.f,
      1.f,-1.f,1.f,0.f,1.f
  };
  glBufferData(GL_ARRAY_BUFFER,3*5*sizeof(float),pos,GL_STATIC_DRAW);

  unsigned indices[3] = {0,1,2};
  glGenBuffers(1,&d_indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,d_indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(unsigned),indices,GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  // position is 2 floats
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
  glEnableVertexAttribArray(1);
  //color is 3 floats
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float),reinterpret_cast<void*>(sizeof(float)*3));
}

/**
 * @brief MainView::resizeGL Called when the window is resized.
 * @param newWidth The new width of the window.
 * @param newHeight The new height of the window.
 */
void MainView::resizeGL(int newWidth, int newHeight) {
  Q_UNUSED(newWidth)
  Q_UNUSED(newHeight)
}

/**
 * @brief MainView::paintGL Draw function. TODO.
 */
void MainView::paintGL() {
    glClearColor(0.2f,0.4f,0.8f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(d_vao);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr);
}

/**
 * @brief MainView::onMessageLogged Debug utility method.
 * @param Message The message to be logged.
 */
void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}
