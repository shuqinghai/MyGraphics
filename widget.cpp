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
    for (int i = 0; i < 100; i++)
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
    //开启抗锯齿
    QSurfaceFormat format;
    format.setSamples(4);  // 设置抗锯齿采样数，可以根据需要调整
    QOpenGLContext* context = QOpenGLContext::currentContext();
    context->setFormat(format);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // 可以根据需要设置不同的提示参数



}

void Widget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    move();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //构造模型变换矩阵
    QMatrix4x4 unit = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    QMatrix4x4 scaleMat = unit;
    m_program->bind();
    m_program->setUniformValue("view",m_camera.getView());
    m_program->setUniformValue("projection",m_camera.getProjection());
    //m_Lines->m_vertices.clear();//清空box

    glLineWidth(1.0f);//设置线宽


    for (int i = 0; i < Instances.count(); i++)
    {
        Instances[i].setRevolutionPoint(m_pickedPoint);
        Instances[i].setBallRotate(m_rotateAxisl, m_rotateAngle);
        //Instances[i].setWorldRotate(m_rotateControl);
        Instances[i].setScale(m_scaleControl);
        Instances[i].setWorldTranslate(m_translateControl);
        m_program->setUniformValue("model", Instances[i].getModelMatrix());
        
        models[Instances[i].modelIndex]->m_vao->bind();
        //绘制面
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_program->setUniformValue("color", Instances[i].color);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);

        glEnable(GL_POLYGON_OFFSET_FILL);//开启多边形偏移
        glPolygonOffset(1.0f, 1.0f); // 调整偏移量的值，根据需要进行调整

        //绘制线框
        m_program->setUniformValue("color", QVector3D(0, 0, 0));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);
        models[Instances[i].modelIndex]->m_vao->release();//释放vao
        //m_Lines->addBox(Instances[i].m_AA, Instances[i].m_BB);

    }
   
    //绘制线
     //关闭深度测试
    glDisable(GL_DEPTH_TEST);
    m_Lines->updateVertices();
    m_Lines->m_vao->bind();
    m_program->setUniformValue("color", QVector3D(1, 1, 0));
    m_program->setUniformValue("model", unit);
    glDrawArrays(GL_LINES, 0, m_Lines->m_vertices.size());
    m_Lines->m_vao->release();


    //绘制拾取点

    
   
    m_program->setUniformValue("color", QVector3D(0, 0, 128.0/255.0));//设置高两点的颜色
    glPointSize(10.0f);//设置高亮点的大小
    glBegin(GL_POINTS);
    glVertex3f(m_pickedPoint.x(), m_pickedPoint.y(), m_pickedPoint.z());
    glEnd();
    m_program->release();//释放着色器程序
    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //在屏幕上画一个圈
    glDisable(GL_DEPTH_TEST);
    // 计算圆的半径和中心坐标
    int radius = height() / 4;
    int centerX = width() / 2;
    int centerY = height() / 2;

    // 使用QPainter绘制圆
    QPainter painter(this);
    painter.setPen(Qt::yellow);
    //painter.setBrush(Qt::red);
    painter.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);
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
    int x = event->button();
    if (event->buttons() & Qt::LeftButton)//旋转
    {

        //球旋转
        getBallRotateAxislAndAngle(rotateLastX, rotateLastY, event->x(), event->y(), m_rotateAxisl, m_rotateAngle);
        qDebug() << m_rotateAngle;
        qDebug() << m_rotateAxisl;
        // 
        // 
        // 
        // 调整旋转
       // m_rotateControl[1] += 200 * (event->x() - lastX) / 512;
        //m_rotateControl[0] += 200 * (event->y() - lastY) / 512;
        //限制x轴的旋转
        //m_rotateControl[0] = std::clamp(m_rotateControl[0], -90.0f, 90.0f);  
    }
    if (event->buttons() & Qt::RightButton)//平移
    {
        //射线1，2
        Ray vector1 = getRayWorld(lastX, lastY);
        Ray vector2 = getRayWorld(event->x(), event->y());
        //交点1，2
        QVector3D point1 = getIntersectionPointOfRayAndPlane(vector1, m_pickedPoint, m_camera.m_direction);
        QVector3D point2 = getIntersectionPointOfRayAndPlane(vector2, m_pickedPoint, m_camera.m_direction);
        m_translateControl += (point2 - point1);
        qDebug() << m_translateControl;
    }
    if (m_isSelecting)
    {
        m_selectionEndPos = event->pos();
    }
    lastX = event->x(), lastY = event->y();
    update();    // 重绘
   
}

void Widget::wheelEvent(QWheelEvent *event)
{
    QPoint p = event->angleDelta();

    if(p.y()>0)
    {
        //m_scaleControl[0] += 1  * 0.1;
        //m_scaleControl[1] += 1  * 0.1;
        //m_scaleControl[2] += 1  * 0.1;
        m_camera.zoom += 1;
    }

    else
    {
        //m_scaleControl[0] -= 1  * 0.1;
        //m_scaleControl[1] -= 1  * 0.1;
        //m_scaleControl[2] -= 1  * 0.1;
        m_camera.zoom -= 1;
    }
    m_camera.zoom = std::clamp(m_camera.zoom,2.0f,90.0f);
    update();//重绘
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QVector3D res = {};
    Triangle tri;
    bool pickupMode = true;//拾取模式
    if (event->button() == Qt::MidButton &&getIntersectionPoint(getRayWorld(event->x(), event->y()), res,tri, pickupMode))
    {
        qDebug() << res;
        //绘制射线
        //m_Lines->m_vertices.append(m_camera.m_position);
        //m_Lines->m_vertices.append(res);
        m_Lines->addTriangle(tri);
        m_pickedPoint = res;
        ////将旋转清零 
        //m_rotateControl = QVector3D(0, 0, 0);
        ////将位移清零
        //m_translateControl = QVector3D(0, 0, 0);
        //m_rotateAngle = 0;

    }
    lastX = event->pos().x();
    lastY = event->pos().y();
    rotateLastX = lastX;
    rotateLastY = lastY;

    if (event->button() == Qt::MidButton && testModel) {
        m_selectionStartPos = event->pos();
        m_selectionEndPos = event->pos();
        m_isSelecting = true;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton && m_isSelecting) {
        QVector3D res = {};
        Triangle tri;
        // 处理框选结束事件
        for (int i = m_selectionStartPos.x();i< m_selectionEndPos.x();i+=5)
        {
            for (int j = m_selectionStartPos.y(); j < m_selectionEndPos.y(); j+=5)
            {
                if (getIntersectionPoint(getRayWorld(i, j), res, tri))
                {
                    qDebug() << res;
                    //m_Lines->m_vertices.append(m_camera.m_position);
                    //m_Lines->m_vertices.append(res);
                    m_Lines->addTriangle(tri);
                }
            }
           
        }
        m_isSelecting = false;
        // 执行框选操作等
    }
    for (int i = 0; i < Instances.count();i++)
    {
        Instances[i].updateBallRotateMatrix();
    }
    m_rotateAngle = 0;
    m_translateControl = QVector3D(0, 0, 0);
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

QVector3D Widget::getIntersectionPointOfRayAndPlane(const Ray& ray,const QVector3D& point, const QVector3D& normalVector)
{
    // p = origin + t*vector 
    double t = (normalVector.x() * (point.x() - ray.startPoint.x()) + normalVector.y() * (point.y() - ray.startPoint.y()) + normalVector.z() * (point.z() - ray.startPoint.z())) / (ray.direction.x()* normalVector.x() + ray.direction.y()* normalVector.y() + ray.direction.z()* normalVector.z());
    return ray.startPoint + ray.direction * t;
}
bool Widget::getBallRotateAxislAndAngle(float lastX, float lastY, float x, float y, QVector3D& axisl, float& angle)
{
    //射线1，2
    Ray vector1 = getRayWorld(lastX, lastY);
    Ray vector2 = getRayWorld(x,y);

    //计算球心坐标
    //屏幕中心射线
    int radius = height() / 4;//屏幕上圆的半径
    Ray screenRay = getRayWorld(width()/2.0, height()/2.0); 
    Ray screenOff = getRayWorld(width() / 2.0 + radius, height() / 2.0);
    QVector3D ballCenter = getIntersectionPointOfRayAndPlane(screenRay, m_pickedPoint, m_camera.m_direction);
    QVector3D ballCenterOffset = getIntersectionPointOfRayAndPlane(screenOff, m_pickedPoint, m_camera.m_direction);
    float ballR = (ballCenterOffset - ballCenter).length();//球半径
    float t[2];
    QVector3D p1 = { 0,0,0 };//交点1 = {0,0,0};//交点1
    QVector3D p2 = { 0,0,0 };//交点2
    if (lineSphereIntersection(ballCenter, ballR, vector1,t) >0)
    {
        float tt = min(t[0], t[1]);
        p1 = vector1.startPoint + vector1.direction * tt;
    }
    else
    {
        QVector3D op = vector1.startPoint - ballCenter;
        double a = acos(sqrt(QVector3D::dotProduct(op, op) - pow(ballR, 2)) / op.length());
        double b = M_PI / 2.0 - a;
        double c = acos(QVector3D::dotProduct(op.normalized(), vector1.direction.normalized()));
        double d = c - b;
        double l = sin(b) / sin(d) * op.length();
        QVector3D vv = vector1.startPoint + l * vector1.direction.normalized();
        p1 = ballR * (vv - ballCenter).normalized() + ballCenter;
    }
    if (lineSphereIntersection(ballCenter, ballR, vector2, t) > 0)
    {
        float tt = min(t[0], t[1]);
        p2 = vector2.startPoint + vector2.direction * tt;
    }
    else
    {
        QVector3D op = vector2.startPoint - ballCenter;
        double a = acos(sqrt(QVector3D::dotProduct(op, op) - pow(ballR, 2)) / op.length());
        double b = M_PI / 2.0 - a;
        double c = acos(QVector3D::dotProduct(op.normalized(), vector2.direction.normalized()));
        double d = c - b;
        double l = sin(b) / sin(d) * op.length();
        QVector3D vv = vector2.startPoint + l * vector2.direction.normalized();
        p2 = ballR * (vv - ballCenter).normalized() + ballCenter;
    }
    QVector3D v1 = {0,0,0};//球心与射线1的交点向量
    QVector3D v2 = {0,0,0};//球心与射线2的交点向量
    v1 = p1 - ballCenter;
    v2 = p2 - ballCenter;
    angle = acos(QVector3D::dotProduct(v1.normalized(), v2.normalized()))/M_PI*180.0;//角度
    axisl = -1*QVector3D::crossProduct(v1, v2).normalized();
    if (axisl == QVector3D(0, 0, 0))
    {
        axisl = QVector3D(1, 0, 0);
    }
    return true;
}
int Widget::lineSphereIntersection(QVector3D center, double r, Ray ray, float t[2])
{
    float a, b, c, discrm;
    QVector3D pMinusC = ray.startPoint - center;
    a = QVector3D::dotProduct(ray.direction, ray.direction);
    b = 2 * QVector3D::dotProduct(ray.direction, pMinusC);
    c = QVector3D::dotProduct(pMinusC, pMinusC) - r * r;
    discrm = b * b - 4 * a*c;
    if (discrm > 0)
    {
        t[0] = (-b + sqrt(discrm)) / (2 * a);
        t[1] = (-b - sqrt(discrm) / (2 * a));
        return 2;
    }
    else if (discrm == 0)
    {
        //The line is tangent to the sphere
        t[0] = -b / (2 * a);
        return 1;
    }
    else
    {
        return 0;
    }

}
