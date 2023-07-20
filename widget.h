#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//-----------QOpenGL
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include "camera.h"
#include "model.h"
#include <algorithm>
#include <QPainter>
#include <QPen>
#include "instance.h"
#include "ray.h"
#include "lines.h"
class Widget : public QOpenGLWidget,protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr):QOpenGLWidget(parent),m_program(0),m_camera{}{
        m_camera.windowHeight = height();
        m_camera.windowWidth = width();
        m_camera.m_position = {0.0,2.0,10};
    }
    ~Widget();
    void initializeGL()override;
    void paintGL()override;
    void resizeGL(int w,int h)override;

    //����
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void move();
    void keyPressEvent(QKeyEvent* keyEvent)
    {
        keyBoardState[*keyEvent->text().toLatin1().data()] = true;
    }

    void keyReleaseEvent(QKeyEvent* keyEvent)
    {
        keyBoardState[*keyEvent->text().toLatin1().data()] = false;
    }
    //����Ļ���ת��������
    Ray getRayWorld(float fx, float fy);
    //���� �������� ������  isClosestPoint����ģʽ false�ཻ�ĵ�  true ����ĵ�
    bool getIntersectionPoint(Ray ray,QVector3D &res,Triangle &triangle, bool isClosestPoint = false);
    //��������ƽ��Ľ���
    QVector3D getIntersectionPointOfRayAndPlane(const Ray& ray,const QVector3D& point, const QVector3D& normalVector);
    //��������ת����ĽǶ�
    bool getBallRotateAxislAndAngle(float lastX, float lastY, float x, float y,QVector3D &axisl,float &angle);
    int lineSphereIntersection(QVector3D center, double r, Ray ray, float t[2]);
private:
    QOpenGLShaderProgram *m_program;
    Camera m_camera;
    QVector3D m_rotateControl = QVector3D(0, 0, 0);// ��ת����
    QVector3D m_scaleControl = QVector3D(1, 1, 1);//���Ų���
    QVector3D m_translateControl = QVector3D(0, 0, 0);//λ�ƿ���

    //������ת
    QVector3D m_rotateAxisl = { 0,0,1 };//��ת��
    float m_rotateAngle = 0;//��ת�Ƕ�
    double lastX = 0.0, lastY = 0.0;
    QVector<Model*> models;
    QVector<Instance>Instances;
    Lines *m_Lines;          //
    //����״̬
    bool keyBoardState[1024] = {};

    QPoint m_selectionStartPos;//ѡ����ʼ��
    QPoint m_selectionEndPos;//ѡ����ֹ��
    bool m_isSelecting = false;
    bool testModel = false;//����ģʽ  //ʵ�ʵĿ�ѡ��������һ�������ж��������ǲ�����Ͳ����
    QVector3D m_pickedPoint = {0,0,0};
    double rotateLastX = 0;
    double rotateLastY = 0;

};
#endif // WIDGET_H
