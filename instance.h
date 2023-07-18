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

    void initializeAABB(const QVector3D& AA,const QVector3D& BB);
    void updateAABB();
    //QMatrix4x4 modelMat;//�任����
    unsigned modelIndex = 0;//ģ�ͱ��
    QVector3D color;
    QVector3D m_AA, m_BB;

private:
    QVector3D m_scaleControl = { 1.0,1.0,1.0 };//���ſ���
    QVector3D m_rotateControl = { 0.0,0.0,0.0 };//��ת����
    QVector3D m_translateControl = { 0.0,0.0,0.0 };//ƽ�ƿ���

    QVector<QVector3D>m_boxPoints;//�����ʼAABB��������
};

#endif // INSTANCE_H
