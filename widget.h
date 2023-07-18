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
    //光线 交点坐标 三角形
    bool getIntersectionPoint(Ray ray,QVector3D &res,Triangle &triangle);

private:
    QOpenGLShaderProgram *m_program;
    Camera m_camera;
    QVector3D m_rotateControl = QVector3D(0, 0, 0);// 旋转参数
    QVector3D m_scaleControl = QVector3D(1, 1, 1);//缩放参数
    double lastX = 0.0, lastY = 0.0;
    QVector<Model*> models;
    QVector<Instance>Instances;
    Lines *m_Lines;          //
    //键盘状态
    bool keyBoardState[1024] = {};

};
#endif // WIDGET_H
