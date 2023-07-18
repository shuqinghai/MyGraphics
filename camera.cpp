#include "camera.h"

Camera::Camera()
{

}

QMatrix4x4 Camera::getView()
{
    updataDirection();
    QMatrix4x4 view;
    view.lookAt(m_position,m_position+m_direction,m_up);
    return view;
}

QMatrix4x4 Camera::getNormalView()
{
    QMatrix4x4 view;
    view.lookAt(m_position,m_position+QVector3D(0,0,-1),m_up);
    return view;
}

QMatrix4x4 Camera::getProjection()
{
    QMatrix4x4 projection;
    projection.perspective(zoom,windowWidth/windowHeight,zNear,zFar);
    return projection;
}

QMatrix4x4 Camera::getOrthogonal()
{
    QMatrix4x4 orth;
    orth.ortho(left,right,bottom,top,zNear,zFar);
    return orth;
}

void Camera::updataDirection()
{
    m_direction[0] = qCos(qDegreesToRadians(pitch))*sin(qDegreesToRadians(yaw));
    m_direction[1] = qSin(qDegreesToRadians(pitch));
    m_direction[2] = -qCos(qDegreesToRadians(pitch))*qCos(qDegreesToRadians(yaw));
}
