#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector3D>
#include <QVector>
#include <QMatrix4x4>

#include "camera.h"


/**
 * @brief The MainView class is resonsible for the actual content of the main
 * window.
 */
class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    // Pyramid IBO VAO VBO
    unsigned d_pyrVAO;
    unsigned d_pyrVBO;
    unsigned d_pyrIBO;

    // Knot IBO VAO VBO
    unsigned d_knotVAO;
    unsigned d_knotIBO;
    unsigned d_knotVBO;

    // Pyr Model
    QMatrix4x4 d_pyrModel;

    // Reuse for both
    QMatrix4x4 d_scale;
    QMatrix4x4 d_rotation;

    // Pyramid translation
    QMatrix4x4 d_pyrTrans;

    // Knot translation and Knot Model Matrix
    QMatrix4x4 d_knotTrans;
    QMatrix4x4 d_knotModel;

    // Element amount
    unsigned d_pyrElements;
    unsigned d_knotElements;

    unsigned d_projLocation;
    unsigned d_modLocation;

    // Camera
    Camera d_camera;

    // Delta time
    float d_deltaTime;

public:
    MainView(QWidget *parent = nullptr);
    ~MainView() override;

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(float scale);

protected:
    void initializeGL() override;
    void resizeGL(int newWidth, int newHeight) override;
    void paintGL() override;

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void wheelEvent(QWheelEvent *ev) override;

private slots:
    void onMessageLogged(QOpenGLDebugMessage Message);

private:
    QOpenGLDebugLogger debugLogger;
    QTimer timer;  // timer used for animation

    QOpenGLShaderProgram shaderProgram;

    void createShaderProgram();

    void createPyramid();
    void createKnot();
    void createCamera();
};

#endif  // MAINVIEW_H
