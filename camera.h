#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Camera
{
public:
    Camera();
public:
    QVector3D m_position = {0,5,10};//���λ��
    QVector3D m_direction = {0,0,-1};//������߷���
    QVector3D m_up = {0,1,0};//�������

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    float zoom = 45.0;

    //�ӽ������
    float left = -1,right = 1,bottom = -1,top = 1,zNear = 0.1,zFar = 100.0;
    int windowWidth = 0.0f;
    int windowHeight = 0.0f;
    QMatrix4x4 getView();
    QMatrix4x4 getNormalView();//��Զ��z����view
    QMatrix4x4 getProjection();
    QMatrix4x4 getOrthogonal();
    void updataDirection();
};

#endif // CAMERA_H
