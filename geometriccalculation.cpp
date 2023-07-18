#include "geometriccalculation.h"
float GeometricCalculation::hitTriangle(Triangle *triangle, Ray ray)
{
    QVector3D p1 = triangle->p1, p2 = triangle->p2, p3 = triangle->p3;
      QVector3D S = ray.startPoint;        // 射线起点
      QVector3D d = ray.direction;         // 射线方向
      QVector3D N = (QVector3D::crossProduct(p2 - p1, p3 - p1)).normalized();    // 法向量
      if (QVector3D::dotProduct(N, d) > 0.0f) N = -N;   // 获取正确的法向量

      // 如果视线和三角形平行
      if (fabs(QVector3D::dotProduct(N, d)) < 0.00001f) return INF;

      // 距离
      float t = (QVector3D::dotProduct(N, p1) - QVector3D::dotProduct(S, N)) / QVector3D::dotProduct(d, N);
      if (t < 0.0005f) return INF;    // 如果三角形在光线背面

      // 交点计算
      QVector3D P = S + d * t;

      // 判断交点是否在三角形中
      QVector3D c1 = QVector3D::crossProduct(p2 - p1, P - p1);
      QVector3D c2 = QVector3D::crossProduct(p3 - p2, P - p2);
      QVector3D c3 = QVector3D::crossProduct(p1 - p3, P - p3);
      if (QVector3D::dotProduct(c1, N) > 0 && QVector3D::dotProduct(c2, N) > 0 && QVector3D::dotProduct(c3, N) > 0) return t;
      if (QVector3D::dotProduct(c1, N) < 0 && QVector3D::dotProduct(c2, N) < 0 && QVector3D::dotProduct(c3, N) < 0) return t;

      return INF;
}

HitResult GeometricCalculation::hitTriangleArray(Ray ray, QVector<Triangle> &triangles, int l, int r)
{
    HitResult res;
       for (int i = l; i <= r; i++) {
           float d = hitTriangle(&triangles[i], ray);
           if (d < INF && d < res.distance) {
               res.distance = d;
               res.triangle = &triangles[i];
           }
       }
       return res;
}

float GeometricCalculation::hitAABB(Ray r, QVector3D AA, QVector3D BB)
{
    // 1.0 / direction
       QVector3D invdir = QVector3D(1.0 / r.direction[0], 1.0 / r.direction[1], 1.0 / r.direction[2]);

       QVector3D in = (BB - r.startPoint) * invdir;
       QVector3D out = (AA - r.startPoint) * invdir;

       QVector3D tmax = maxVec(in, out);
       QVector3D tmin = minVec(in, out);

       float t1 = min(tmax[0], min(tmax[1], tmax[2]));
       float t0 = max(tmin[0], max(tmin[1], tmin[2]));

       return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
}
