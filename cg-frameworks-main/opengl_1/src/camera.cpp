#include "camera.h"


Camera::Camera(float fov,float aspect,float near, float far, QVector3D pos)
    : d_position(pos),
      d_fieldOfView(fov),
      d_nearPlane(near),
      d_farPlane(far),
      d_aspectRatio(aspect)
{
    // Camera direction will be at the (0,0,0) origin of the scene
    QVector3D const origin{0.f,0.f,0.f};
    d_direction = QVector3D(pos - origin).normalized();
}


QMatrix4x4 Camera::getView() const
{
    QMatrix4x4 view;
    view.lookAt(d_position,d_direction,getUpDirection());
    return view;
}

QMatrix4x4 Camera::getProjection() const
{
    QMatrix4x4 proj;
    proj.perspective(d_fieldOfView,d_aspectRatio,d_nearPlane,d_farPlane);
    return proj;
}
