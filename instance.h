#ifndef INSTANCE_H
#define INSTANCE_H
#include "model.h"
#include "QMatrix4x4"
#include <QVector3D>
class Instance
{
public:
    Instance();

    QMatrix4x4 getModelMatrix();
    QMatrix4x4 getScale();
    QMatrix4x4 getTranslate();
    QMatrix4x4 getRotate();
    void translate(const QVector3D& value);
    void rotate(const QVector3D& angle);
    void scale(const QVector3D& value);
    void translate(const float& xV, const float& yV, const float& zV);
    void rotate(const float& xAngle, const float& yAngle, const float& zAngle);
    void setScale(const QVector3D& value);
    void setTranslate(const QVector3D& value);
    void setRotate(const QVector3D& angle);
    void setRevolutionPoint(const QVector3D& point);
    void setWorldRotate(const QVector3D& angle);
    void setBallRotate(const QVector3D& axisl, float angle);
    void updateBallRotateMatrix();
    void setWorldTranslate(const QVector3D& value);
    void initializeAABB(const QVector3D& AA,const QVector3D& BB);
    void updateAABB();
    //QMatrix4x4 modelMat;//变换矩阵
    unsigned modelIndex = 0;//模型编号
    QVector3D color;
    QVector3D m_AA, m_BB;

private:

    //局部变换
    QVector3D m_scaleControl = { 1.0,1.0,1.0 };//缩放控制
    QVector3D m_rotateControl = { 0.0,0.0,0.0 };//旋转控制
    QVector3D m_translateControl = { 0.0,0.0,0.0 };//平移控制

    //世界变换
    QVector3D m_RevolutionPoint = { 0.0,0.0,0.0 };//公转点
    QVector3D m_worldRotateControl = { 0.0,0.0,0.0 };//公转控制
    QVector3D m_worldTranslateControl = { 0.0,0.0,0.0 };//公平移
    QVector3D m_axisl = { 0,0,0 };
    float m_angle = 0;

    QVector<QVector3D>m_boxPoints;//代表初始AABB的六个点
    QMatrix4x4 m_modelPublicLast = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};//上次一的公有变换
};

#endif // INSTANCE_H
