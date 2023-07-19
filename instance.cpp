#include "instance.h"

Instance::Instance()
{

}
QMatrix4x4 Instance::getModelMatrix()
{
	QMatrix4x4 model;
	//缩放——旋转——位移——位移到逆公转点位置——旋转——位移回公转点——位移到逆公转点位置——旋转——位移回公转点
	//将这个操作逆向执行
	//模型变换 左乘 上一次的公有变换 左乘 这一次的公有变换
	QMatrix4x4 modelPublic;//公转变换
	modelPublic.translate(m_worldTranslateControl);
	modelPublic.translate(m_RevolutionPoint);
	modelPublic.rotate(m_worldRotateControl.x(), QVector3D(1, 0, 0));
	modelPublic.rotate(m_worldRotateControl.y(), QVector3D(0, 1, 0));
	modelPublic.rotate(m_worldRotateControl.z(), QVector3D(1, 0, 1));
	modelPublic.translate(-1*m_RevolutionPoint);

	QMatrix4x4 modelPrivate;//私有变换
	modelPrivate.translate(m_translateControl);
	modelPrivate.rotate(m_rotateControl.x(), QVector3D(1, 0, 0));
	modelPrivate.rotate(m_rotateControl.y(), QVector3D(0, 1, 0));
	model.rotate(m_rotateControl.z(), QVector3D(0, 0, 1));

	modelPrivate.scale(m_scaleControl);
	model = modelPublic* m_modelPublicLast*modelPrivate;
	return model;
}

QMatrix4x4 Instance::getScale()
{
	QMatrix4x4 model;
	model.scale(m_scaleControl);
	return model;
}

QMatrix4x4 Instance::getTranslate()
{
	QMatrix4x4 model;
	model.translate(m_translateControl);
	return model;
}

QMatrix4x4 Instance::getRotate()
{
	QMatrix4x4 model;
	model.rotate(m_rotateControl.x(), QVector3D(1, 0, 0));
	model.rotate(m_rotateControl.y(), QVector3D(0, 1, 0));
	model.rotate(m_rotateControl.z(), QVector3D(0, 0, 1));
	return model;
}

void Instance::translate(const QVector3D& value)
{
	m_translateControl += value;
}

void Instance::rotate(const QVector3D& angle)
{
	m_rotateControl += angle;
}

void Instance::scale(const QVector3D& value)
{
	m_scaleControl *= value;
}

void Instance::translate(const float& xV, const float& yV, const float& zV)
{
	translate(QVector3D(xV, yV, zV));
}

void Instance::rotate(const float& xAngle, const float& yAngle, const float& zAngle)
{
	rotate(QVector3D(xAngle, yAngle, zAngle));
}

void Instance::setScale(const QVector3D& value)
{
	m_scaleControl = value;
}

void Instance::setTranslate(const QVector3D& value)
{
	m_translateControl = value;
}

void Instance::setRotate(const QVector3D& angle)
{
    m_rotateControl = angle;
}
void Instance::setRevolutionPoint(const QVector3D& point)
{
	if (point != m_RevolutionPoint)
	{
		//获得上一次的公有变换

		//公有变换顺序为
		//平移旋转中心到原点——旋转——平移回原位置——平移到平移点
		//矩阵变换逆顺序
		QMatrix4x4 modelPublic;//公转变换

		modelPublic.translate(m_worldTranslateControl);
		modelPublic.translate(m_RevolutionPoint);
		modelPublic.rotate(m_worldRotateControl.x(), QVector3D(1, 0, 0));
		modelPublic.rotate(m_worldRotateControl.y(), QVector3D(0, 1, 0));
		modelPublic.rotate(m_worldRotateControl.z(), QVector3D(1, 0, 1));
		modelPublic.translate(-1 * m_RevolutionPoint);
		m_modelPublicLast = modelPublic * m_modelPublicLast;
		m_RevolutionPoint = point;
	}	
}

void Instance::setWorldRotate(const QVector3D& angle)
{
	m_worldRotateControl = angle;
}

void Instance::setWorldTranslate(const QVector3D& value)
{
	m_worldTranslateControl = value;
}
void Instance::initializeAABB(const QVector3D &AA, const QVector3D &BB)
{
    m_AA = AA;
    m_BB = BB;
    //得到表示AABB的立方体的八个点
    m_boxPoints.append(QVector3D(AA.x(),AA.y(),AA.z()));
    m_boxPoints.append(QVector3D(AA.x(),AA.y(),BB.z()));
    m_boxPoints.append(QVector3D(AA.x(),BB.y(),AA.z()));
    m_boxPoints.append(QVector3D(AA.x(),BB.y(),BB.z()));
    m_boxPoints.append(QVector3D(BB.x(),AA.y(),AA.z()));
    m_boxPoints.append(QVector3D(BB.x(),AA.y(),BB.z()));
    m_boxPoints.append(QVector3D(BB.x(),BB.y(),AA.z()));
    m_boxPoints.append(QVector3D(BB.x(),BB.y(),BB.z()));
}

void Instance::updateAABB()
{
    m_AA = {FLT_MAX,FLT_MAX,FLT_MAX};
    m_BB = {-FLT_MAX,-FLT_MAX,-FLT_MAX};
	QMatrix4x4 modelMat = getModelMatrix();
	QVector<QVector3D>boxPoints = m_boxPoints;
	for (int i = 0; i < boxPoints.count(); i++)
	{
		boxPoints[i] = (modelMat*QVector4D(boxPoints[i],1.0f)).toVector3D();
	}
    // 计算 AABB
    for (int i = 0; i < boxPoints.count(); i++) {
        // 最小点 AA
        m_AA[0] = qMin(m_AA[0], boxPoints[i].x());
        m_AA[1] = qMin(m_AA[1], boxPoints[i].y());
        m_AA[2] = qMin(m_AA[2], boxPoints[i].z());
        // 最大点 BB
        m_BB[0] = qMax(m_BB[0], boxPoints[i].x());
        m_BB[1] = qMax(m_BB[1], boxPoints[i].y());
        m_BB[2] = qMax(m_BB[2], boxPoints[i].z());
    }
}
