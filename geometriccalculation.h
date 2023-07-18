#ifndef GEOMETRICCALCULATION_H
#define GEOMETRICCALCULATION_H
#include "triangle.h"
#include "ray.h"
#include <algorithm>
#include <QVector>
using std::min;
using std::max;
class GeometricCalculation
{
public:
    static float hitTriangle(Triangle* triangle,Ray ray);
    static HitResult hitTriangleArray(Ray ray,QVector<Triangle>& triangles,int l,int r);
    static float hitAABB(Ray r,QVector3D AA,QVector3D BB);
    //返回每个分量的最大值构成的新向量
    static QVector3D maxVec(const QVector3D& v1, const QVector3D& v2) {
        float maxX = std::max(v1.x(), v2.x());
        float maxY = std::max(v1.y(), v2.y());
        float maxZ = std::max(v1.z(), v2.z());

        return QVector3D(maxX, maxY, maxZ);
    }
     //返回每个分量的最小值构成的新向量
    static QVector3D minVec(const QVector3D& v1, const QVector3D& v2) {
        float minX = std::min(v1.x(), v2.x());
        float minY = std::min(v1.y(), v2.y());
        float minZ = std::min(v1.z(), v2.z());

        return QVector3D(minX, minY, minZ);
    }
};

#endif // GEOMETRICCALCULATION_H
