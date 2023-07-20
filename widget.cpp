#include "widget.h"

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    // ������ɫ���������
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vshader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fshader.fsh");

    m_program->link();
    m_program->create();//������ɫ���������
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
    //������Ȳ���
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.4 ,5.0, 1.0);   // ������ɫ -- ��
    //���������
    QSurfaceFormat format;
    format.setSamples(4);  // ���ÿ���ݲ����������Ը�����Ҫ����
    QOpenGLContext* context = QOpenGLContext::currentContext();
    context->setFormat(format);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // ���Ը�����Ҫ���ò�ͬ����ʾ����



}

void Widget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    move();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //����ģ�ͱ任����
    QMatrix4x4 unit = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    QMatrix4x4 scaleMat = unit;
    m_program->bind();
    m_program->setUniformValue("view",m_camera.getView());
    m_program->setUniformValue("projection",m_camera.getProjection());
    //m_Lines->m_vertices.clear();//���box

    glLineWidth(1.0f);//�����߿�


    for (int i = 0; i < Instances.count(); i++)
    {
        Instances[i].setRevolutionPoint(m_pickedPoint);
        Instances[i].setBallRotate(m_rotateAxisl, m_rotateAngle);
        //Instances[i].setWorldRotate(m_rotateControl);
        Instances[i].setScale(m_scaleControl);
        Instances[i].setWorldTranslate(m_translateControl);
        m_program->setUniformValue("model", Instances[i].getModelMatrix());
        
        models[Instances[i].modelIndex]->m_vao->bind();
        //������
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_program->setUniformValue("color", Instances[i].color);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);

        glEnable(GL_POLYGON_OFFSET_FILL);//���������ƫ��
        glPolygonOffset(1.0f, 1.0f); // ����ƫ������ֵ��������Ҫ���е���

        //�����߿�
        m_program->setUniformValue("color", QVector3D(0, 0, 0));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, models[Instances[i].modelIndex]->m_indices.size(), GL_UNSIGNED_INT, 0);
        models[Instances[i].modelIndex]->m_vao->release();//�ͷ�vao
        //m_Lines->addBox(Instances[i].m_AA, Instances[i].m_BB);

    }
   
    //������
     //�ر���Ȳ���
    glDisable(GL_DEPTH_TEST);
    m_Lines->updateVertices();
    m_Lines->m_vao->bind();
    m_program->setUniformValue("color", QVector3D(1, 1, 0));
    m_program->setUniformValue("model", unit);
    glDrawArrays(GL_LINES, 0, m_Lines->m_vertices.size());
    m_Lines->m_vao->release();


    //����ʰȡ��

    
   
    m_program->setUniformValue("color", QVector3D(0, 0, 128.0/255.0));//���ø��������ɫ
    glPointSize(10.0f);//���ø�����Ĵ�С
    glBegin(GL_POINTS);
    glVertex3f(m_pickedPoint.x(), m_pickedPoint.y(), m_pickedPoint.z());
    glEnd();
    m_program->release();//�ͷ���ɫ������
    //������Ȳ���
    glEnable(GL_DEPTH_TEST);

    //����Ļ�ϻ�һ��Ȧ
    glDisable(GL_DEPTH_TEST);
    // ����Բ�İ뾶����������
    int radius = height() / 4;
    int centerX = width() / 2;
    int centerY = height() / 2;

    // ʹ��QPainter����Բ
    QPainter painter(this);
    painter.setPen(Qt::yellow);
    //painter.setBrush(Qt::red);
    painter.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);
    glEnable(GL_DEPTH_TEST);
}

void Widget::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();//openGL����
    f->glViewport(0,0,w,h);
    m_camera.windowHeight = height();
    m_camera.windowWidth = width();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->button();
    if (event->buttons() & Qt::LeftButton)//��ת
    {

        //����ת
        getBallRotateAxislAndAngle(rotateLastX, rotateLastY, event->x(), event->y(), m_rotateAxisl, m_rotateAngle);
        qDebug() << m_rotateAngle;
        qDebug() << m_rotateAxisl;
        // 
        // 
        // 
        // ������ת
       // m_rotateControl[1] += 200 * (event->x() - lastX) / 512;
        //m_rotateControl[0] += 200 * (event->y() - lastY) / 512;
        //����x�����ת
        //m_rotateControl[0] = std::clamp(m_rotateControl[0], -90.0f, 90.0f);  
    }
    if (event->buttons() & Qt::RightButton)//ƽ��
    {
        //����1��2
        Ray vector1 = getRayWorld(lastX, lastY);
        Ray vector2 = getRayWorld(event->x(), event->y());
        //����1��2
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
    update();    // �ػ�
   
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
    update();//�ػ�
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QVector3D res = {};
    Triangle tri;
    bool pickupMode = true;//ʰȡģʽ
    if (event->button() == Qt::MidButton &&getIntersectionPoint(getRayWorld(event->x(), event->y()), res,tri, pickupMode))
    {
        qDebug() << res;
        //��������
        //m_Lines->m_vertices.append(m_camera.m_position);
        //m_Lines->m_vertices.append(res);
        m_Lines->addTriangle(tri);
        m_pickedPoint = res;
        ////����ת���� 
        //m_rotateControl = QVector3D(0, 0, 0);
        ////��λ������
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
        // �����ѡ�����¼�
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
        // ִ�п�ѡ������
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
    //�����λ��ת��ΪNDC����
    //QVector2D ndcPos = QVector2D(fx/width()*2.0f - 1.0f,1.0f - fy/height()*2.0f);
    ////qDebug()<<ndcPos<<"NDC";

    ////��NDC����תΪ��������
    //QVector4D rayClip = QVector4D(ndcPos.x(),ndcPos.y(),-1.0f,1.0f);
    //QVector4D rayEye = rayClip*m_camera.getProjection().inverted();
    //rayEye = QVector4D(rayEye.x(),rayEye.y(),-1.0f,0.0f);
    //QVector3D rayDirection = rayEye.toVector3D().normalized();

    ////***ԭ��
    //QVector3D rayWorld = (rayEye*m_camera.getNormalView().inverted()).toVector3D().normalized();
    //QMatrix4x4 rotate;
    //rotate.rotate(m_camera.yaw,QVector3D(0,1,0));
    //rotate.rotate(-m_camera.pitch,QVector3D(1,0,0));
    //rayWorld = rayWorld*rotate;
    ////�������direction�䶯ʱ �Ȱ���û�䶯�ı任���������֮꣬������ת��Ŀ�귽��
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
    QVector3D vector = ray.direction;//ԭʼ����
    QVector3D position = ray.startPoint;//ԭʼ���
    HitResult hitRes;//�󽻽��
    for(int i=0;i<Instances.count();i++)
    {
        Instances[i].updateAABB();
        double boxDis = -1;
        boxDis = GeometricCalculation::hitAABB(ray, Instances[i].m_AA, Instances[i].m_BB);
        if(boxDis != -1)//�н���
        {
            //��rayתΪ�ֲ�����ϵ��
            Ray locationRay = ray;//�ֲ�ray
            QMatrix4x4 inverseTransform = (Instances[i].getModelMatrix()).inverted();
            locationRay.direction = (inverseTransform*QVector4D(ray.direction,0.0f)).toVector3D().normalized();
            locationRay.startPoint = (inverseTransform*QVector4D(ray.startPoint,1.0f)).toVector3D();

            hitRes = models[Instances[i].modelIndex]->m_BVH->hitBVH(locationRay);
            if (abs(114514.0 - hitRes.distance) < 1e-6)
            {
                continue;//���Ų�ѯ�¸�����
            }
            else
            {
                //�����α任��ȫ������ϵ
                QVector3D p1 = Instances[i].getModelMatrix() * hitRes.triangle->p1;
                QVector3D p2 = Instances[i].getModelMatrix() * hitRes.triangle->p2;
                QVector3D p3 = Instances[i].getModelMatrix() * hitRes.triangle->p3;
                
                QVector3D localInterPoint = locationRay.startPoint + locationRay.direction * hitRes.distance;//�ֲ���������
                QVector3D realInterPoint = Instances[i].getModelMatrix() * localInterPoint;
                triangle = Triangle(p1, p2, p3);
                
                if (isClosestPoint)//�����뽻������ĵ�
                {
                    res = triangle.findClosestPoint(realInterPoint);
                }
                else//���ؽ���
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
    //����1��2
    Ray vector1 = getRayWorld(lastX, lastY);
    Ray vector2 = getRayWorld(x,y);

    //������������
    //��Ļ��������
    int radius = height() / 4;//��Ļ��Բ�İ뾶
    Ray screenRay = getRayWorld(width()/2.0, height()/2.0); 
    Ray screenOff = getRayWorld(width() / 2.0 + radius, height() / 2.0);
    QVector3D ballCenter = getIntersectionPointOfRayAndPlane(screenRay, m_pickedPoint, m_camera.m_direction);
    QVector3D ballCenterOffset = getIntersectionPointOfRayAndPlane(screenOff, m_pickedPoint, m_camera.m_direction);
    float ballR = (ballCenterOffset - ballCenter).length();//��뾶
    float t[2];
    QVector3D p1 = { 0,0,0 };//����1 = {0,0,0};//����1
    QVector3D p2 = { 0,0,0 };//����2
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
    QVector3D v1 = {0,0,0};//����������1�Ľ�������
    QVector3D v2 = {0,0,0};//����������2�Ľ�������
    v1 = p1 - ballCenter;
    v2 = p2 - ballCenter;
    angle = acos(QVector3D::dotProduct(v1.normalized(), v2.normalized()))/M_PI*180.0;//�Ƕ�
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
