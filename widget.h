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

    //交互
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
    //由屏幕组表转世界向量
    Ray getRayWorld(float fx, float fy);
    //光线 交点坐标 三角形  isClosestPoint返回模式 false相交的点  true 最近的点
    bool getIntersectionPoint(Ray ray,QVector3D &res,Triangle &triangle, bool isClosestPoint = false);
    //求射线与平面的交点
    QVector3D getIntersectionPointOfRayAndPlane(const Ray& ray,const QVector3D& point, const QVector3D& normalVector);
    //计算球旋转的轴的角度
    bool getBallRotateAxislAndAngle(float lastX, float lastY, float x, float y,QVector3D &axisl,float &angle);
    int lineSphereIntersection(QVector3D center, double r, Ray ray, float t[2]);
private:
    QOpenGLShaderProgram *m_program;
    Camera m_camera;
    QVector3D m_rotateControl = QVector3D(0, 0, 0);// 旋转参数
    QVector3D m_scaleControl = QVector3D(1, 1, 1);//缩放参数
    QVector3D m_translateControl = QVector3D(0, 0, 0);//位移控制

    //按球旋转
    QVector3D m_rotateAxisl = { 0,0,1 };//旋转轴
    float m_rotateAngle = 0;//旋转角度
    double lastX = 0.0, lastY = 0.0;
    QVector<Model*> models;
    QVector<Instance>Instances;
    Lines *m_Lines;          //
    //键盘状态
    bool keyBoardState[1024] = {};

    QPoint m_selectionStartPos;//选择起始点
    QPoint m_selectionEndPos;//选择终止点
    bool m_isSelecting = false;
    bool testModel = false;//测试模式  //实际的框选可能是拉一个框来判断三角形是不是在筒子里
    QVector3D m_pickedPoint = {0,0,0};
    double rotateLastX = 0;
    double rotateLastY = 0;

};
#endif // WIDGET_H
