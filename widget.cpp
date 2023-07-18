#include "widget.h"

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    // 创建着色器程序对象
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vshader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fshader.fsh");

    m_program->link();
    m_program->create();//创建着色器程序对象
    m_Lines = new Lines();
   
    Model *model = new Model("Stanford Bunny.obj", context());
    models.append(model);
    for (int i = 0; i < 1000; i++)
    {
        Instance instance;
        instance.modelIndex = 0;
        QVector3D AA,BB;
        models[instance.modelIndex]->m_BVH->getAABB(AA,BB);
        instance.initializeAABB(AA,BB);
        instance.translate(i*0.5,0,0);
        int n = i % 3;
        if (n == 0)
        {
            instance.color = { 1.0,0,0 };
        }
        else if (n == 1)
        {
            instance.color = { 0,1,0 };
        }
        else
        {
            instance.color = {0,0,1};
        }
        instance.updateAABB();
        
        
        Instances.append(instance);
        

    }
    m_Lines->bindData(context());
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.4 ,5.0, 1.0);   // 背景颜色 -- 黑
}

void Widget::paintGL()
{
    move();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //构造模型变换矩阵
    QMatrix4x4 unit = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    QMatrix4x4 scaleMat = unit;
    m_program->bind();
    Instances[0].setRotate(m_rotateControl);
    Instances[0].setScale(m_scaleControl);
    m_program->setUniformValue("view",m_camera.getView());
    m_program->setUniformValue("projection",m_camera.getProjection());
    //m_Lines->m_vertices.clear();//清空box
    for (int i = 0; i < Instances.count(); i++)
    {
        m_program->setUniformValue("model", Instances[i].getModelMatrix());
        m_program->setUniformValue("color",QVector3D(0,0,0));
        models[Instances[i].modelIndex]->m_vao->bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_program->setUniformValue("color", Instances[i].color);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);
        models[Instances[i].modelIndex]->m_vao->release();//释放vao
        //m_Lines->addBox(Instances[i].m_AA, Instances[i].m_BB);
    }
   
    //绘制线
    
    m_Lines->updateVertices();
    m_Lines->m_vao->bind();
    m_program->setUniformValue("color", QVector3D(0, 1, 0));
    m_program->setUniformValue("model", unit);
    glDrawArrays(GL_LINES, 0, m_Lines->m_vertices.size());
    m_Lines->m_vao->release();


    //绘制拾取点

    //关闭深度测试

    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f);//设置高两点的颜色
    glPointSize(10.0f);//设置高亮点的大小
    glBegin(GL_POINTS);
    glVertex3f(m_pickedPoint.x(), m_pickedPoint.y(), m_pickedPoint.z());
    glEnd();
    m_program->release();//释放着色器程序
    //开启深度测试
    glEnable(GL_DEPTH_TEST);

}

void Widget::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();//openGL函数
    f->glViewport(0,0,w,h);
    m_camera.windowHeight = height();
    m_camera.windowWidth = width();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{

    // 调整旋转
    m_rotateControl[1] += 200 * (event->x() - lastX) / 512;
    m_rotateControl[0] += 200 * (event->y() - lastY) / 512;
    //限制x轴的旋转
    m_rotateControl[0] = std::clamp(m_rotateControl[0], -90.0f, 90.0f);
    lastX = event->x(), lastY = event->y();

    if (m_isSelecting)
    {
        m_selectionEndPos = event->pos();
    }
    update();    // 重绘
}

void Widget::wheelEvent(QWheelEvent *event)
{
    QPoint p = event->angleDelta();

    if(p.y()>0)
    {
        m_scaleControl[0] += 1  * 0.1;
        m_scaleControl[1] += 1  * 0.1;
        m_scaleControl[2] += 1  * 0.1;
    }

    else
    {
        m_scaleControl[0] -= 1  * 0.1;
        m_scaleControl[1] -= 1  * 0.1;
        m_scaleControl[2] -= 1  * 0.1;
    }
    m_camera.zoom = std::clamp(m_camera.zoom,2.0f,90.0f);
    update();//重绘
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QVector3D res = {};
    Triangle tri;
    bool pickupMode = true;//拾取模式
    if (getIntersectionPoint(getRayWorld(event->x(), event->y()), res,tri, pickupMode))
    {
        qDebug() << res;
        m_Lines->m_vertices.append(m_camera.m_position);
        m_Lines->m_vertices.append(res);
        m_Lines->addTriangle(tri);
        m_pickedPoint = res;
    }
    lastX = event->pos().x();
    lastY = event->pos().y();

    if (event->button() == Qt::LeftButton && testModel) {
        m_selectionStartPos = event->pos();
        m_selectionEndPos = event->pos();
        m_isSelecting = true;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isSelecting) {
        QVector3D res = {};
        Triangle tri;
        // 处理框选结束事件
        for (int i = m_selectionStartPos.x();i< m_selectionEndPos.x();i++)
        {
            for (int j = m_selectionStartPos.y(); j < m_selectionEndPos.y(); j++)
            {
                if (getIntersectionPoint(getRayWorld(i, j), res, tri))
                {
                    qDebug() << res;
                    m_Lines->m_vertices.append(m_camera.m_position);
                    m_Lines->m_vertices.append(res);
                    m_Lines->addTriangle(tri);
                }
            }
           
        }
        m_isSelecting = false;
        // 执行框选操作等
    }
}

void Widget::move()
{
    static float speed = 1.0;
    if (keyBoardState['w'])
    {
        m_camera.m_position +=0.005f*speed*m_camera.m_direction;
    }
    if (keyBoardState['s'])
    {
        m_camera.m_position -=0.005f*speed*m_camera.m_direction;
    }
    if (keyBoardState['a'])
    {
        m_camera.m_position -=0.005f*speed*QVector3D::crossProduct(m_camera.m_direction,m_camera.m_up).normalized();
    }
    if (keyBoardState['d'])
    {
        m_camera.m_position +=0.005f*speed*QVector3D::crossProduct(m_camera.m_direction,m_camera.m_up).normalized();
    }
    if (keyBoardState['q'])
    {
        m_camera.m_position +=0.005f*speed*m_camera.m_up;
    }
    if (keyBoardState['e'])
    {
        m_camera.m_position -=0.005f*speed*m_camera.m_up;
    }
    if(keyBoardState[' '])
    {
        speed = 20.0;
    }
    else
    {
        speed = 1.0;
    }

    update();
}

Ray Widget::getRayWorld(float fx,float fy)
{
    Ray ray;
    //将鼠标位置转换为NDC坐标
    //QVector2D ndcPos = QVector2D(fx/width()*2.0f - 1.0f,1.0f - fy/height()*2.0f);
    ////qDebug()<<ndcPos<<"NDC";

    ////将NDC坐标转为世界坐标
    //QVector4D rayClip = QVector4D(ndcPos.x(),ndcPos.y(),-1.0f,1.0f);
    //QVector4D rayEye = rayClip*m_camera.getProjection().inverted();
    //rayEye = QVector4D(rayEye.x(),rayEye.y(),-1.0f,0.0f);
    //QVector3D rayDirection = rayEye.toVector3D().normalized();

    ////***原来
    //QVector3D rayWorld = (rayEye*m_camera.getNormalView().inverted()).toVector3D().normalized();
    //QMatrix4x4 rotate;
    //rotate.rotate(m_camera.yaw,QVector3D(0,1,0));
    //rotate.rotate(-m_camera.pitch,QVector3D(1,0,0));
    //rayWorld = rayWorld*rotate;
    ////当相机的direction变动时 先按照没变动的变换出世界坐标，之后再旋转到目标方向
    //ray.direction = rayWorld;
    //ray.startPoint = m_camera.m_position;
    //ray.direction = rayDirection;
    //ray.startPoint = m_camera.m_position;
    //return ray;

    //=======================================================================

    QMatrix4x4 projection = m_camera.getProjection();

    float x = (2.0f * fx) / this->width() - 1.0f;
    float y = 1.0f - (2.0f * fy) / this->height();
    float z = 1.0f;
    QVector3D ray_nds = QVector3D (x, y, z);
    QVector4D ray_clip = QVector4D (ray_nds.x(), ray_nds.y(), ray_nds.z(), 1.0f);
    QVector4D ray_eye = projection.inverted() * ray_clip;
    QVector4D ray_world = m_camera.getView().inverted() * ray_eye;
    x = ray_world.x();
    y = ray_world.y();
    z = ray_world.z();
    float w = ray_world.w();
    x /= w;
    y /= w;
    z /= w;
    ray.direction = (QVector3D(x,y,z) - m_camera.m_position).normalized();
    ray.startPoint = m_camera.m_position;
    return ray;
}

bool Widget::getIntersectionPoint(Ray ray,QVector3D &res,Triangle &triangle,bool isClosestPoint)
{
    QVector3D vector = ray.direction;//原始方向
    QVector3D position = ray.startPoint;//原始起点
    HitResult hitRes;//求交结果
    for(int i=0;i<Instances.count();i++)
    {
        Instances[i].updateAABB();
        double boxDis = -1;
        boxDis = GeometricCalculation::hitAABB(ray, Instances[i].m_AA, Instances[i].m_BB);
        if(boxDis != -1)//有交点
        {
            //将ray转为局部坐标系下
            Ray locationRay = ray;//局部ray
            QMatrix4x4 inverseTransform = (Instances[i].getModelMatrix()).inverted();
            locationRay.direction = (inverseTransform*QVector4D(ray.direction,0.0f)).toVector3D().normalized();
            locationRay.startPoint = (inverseTransform*QVector4D(ray.startPoint,1.0f)).toVector3D();

            hitRes = models[Instances[i].modelIndex]->m_BVH->hitBVH(locationRay);
            if (abs(114514.0 - hitRes.distance) < 1e-6)
            {
                continue;//接着查询下个盒子
            }
            else
            {
                //三角形变换到全局坐标系
                QVector3D p1 = Instances[i].getModelMatrix() * hitRes.triangle->p1;
                QVector3D p2 = Instances[i].getModelMatrix() * hitRes.triangle->p2;
                QVector3D p3 = Instances[i].getModelMatrix() * hitRes.triangle->p3;
                
                QVector3D localInterPoint = locationRay.startPoint + locationRay.direction * hitRes.distance;//局部交点坐标
                QVector3D realInterPoint = Instances[i].getModelMatrix() * localInterPoint;
                triangle = Triangle(p1, p2, p3);
                
                if (isClosestPoint)//返回与交点最近的点
                {
                    res = triangle.findClosestPoint(realInterPoint);
                }
                else//返回交点
                {
                    res = realInterPoint;
                }
                return true;
            }
        }

    }
    return false;
}

