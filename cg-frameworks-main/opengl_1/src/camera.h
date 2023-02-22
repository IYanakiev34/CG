#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
    QMatrix4x4 d_proj;
    QMatrix4x4 d_view;

    QVector3D d_direction;
    QVector3D d_position;

    float d_fieldOfView;
    float d_nearPlane;
    float d_farPlane;
    float d_aspectRatio;
public:
    Camera() = default;
    Camera(float fov,float aspect,float near = 0.2f, float far = 20.f, QVector3D pos = QVector3D(0.f,0.f,0.f));

    // Getters
    float getFieldOfView() const;
    QVector3D getPosition() const;
    QVector3D getDirection() const;
    QVector3D getUpDirection() const;
    QVector3D getRightDirection() const;

    QMatrix4x4 getProjection() const;
    QMatrix4x4 getView() const;

    // Setters
    void setAspectRatio(float aspect);
};

inline float Camera::getFieldOfView() const
{
    return d_fieldOfView;
}

inline QVector3D Camera::getPosition() const
{
    return d_position;
}

inline QVector3D Camera::getDirection() const
{
    return d_direction;
}

inline QVector3D Camera::getUpDirection() const
{
    return QVector3D::crossProduct(d_direction,getRightDirection());
}

inline QVector3D Camera::getRightDirection() const
{
    return QVector3D::crossProduct(QVector3D(0.f,1.f,0.f),d_direction).normalized();
}

inline void Camera::setAspectRatio(float aspect)
{
    this->d_aspectRatio = aspect;
}
#endif // CAMERA_H
