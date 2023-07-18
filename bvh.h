#ifndef BVH_H
#define BVH_H
#include <QVector3D>
#include <QVector>
#include <QOpenGLFunctions>
#include <algorithm>
#include "triangle.h"
#include "ray.h"
#include "geometriccalculation.h"

using std::min;
using std::max;



//// �󽻽��
//struct HitResult {
//    Triangle* triangle = NULL;
//    float distance = INF;
//};

//// ����
//typedef struct Ray {
//    QVector3D startPoint = QVector3D(0, 0, 0);    // ���
//    QVector3D direction = QVector3D(0, 0, 0);     // ����
//}Ray;


class BVH
{
private:
    struct BVHNode{
        BVHNode* left = NULL;   //������������
        BVHNode* right = NULL;
        int n,index;            //Ҷ�ӽڵ���Ϣ
        QVector3D AA,BB;        //��Գư�Χ��
    };

public:
    BVH(const QVector<QVector3D>& vertices,const QVector<GLuint>& indices);
    HitResult hitBVH(Ray ray);
    //����ģ��AABB
    bool getAABB(QVector3D &AA,QVector3D &BB);

private:
    // ���� BVH
    BVHNode* buildBVH(QVector<Triangle>& triangles,int l,int r,int n);
    // SAH �Ż����� BVH
    BVHNode* buildBVHwithSAH(QVector<Triangle>& triangles,int l,int r,int n);
    void incomeTriangles(const QVector<QVector3D>& vertices,const QVector<GLuint>& indices);
    // �� BVH �ϱ�����
    HitResult hitBVH(Ray ray,QVector<Triangle>& triangles,BVHNode* root);
//    float hitTriangle(Triangle* triangle,Ray ray);
//    HitResult hitTriangleArray(Ray ray,QVector<Triangle>& triangles,int l,int r);
//    float hitAABB(Ray r,QVector3D AA,QVector3D BB);


//    //����ÿ�����������ֵ���ɵ�������
//    QVector3D maxVec(const QVector3D& v1, const QVector3D& v2) {
//        float maxX = std::max(v1.x(), v2.x());
//        float maxY = std::max(v1.y(), v2.y());
//        float maxZ = std::max(v1.z(), v2.z());

//        return QVector3D(maxX, maxY, maxZ);
//    }
//     //����ÿ����������Сֵ���ɵ�������
//    QVector3D minVec(const QVector3D& v1, const QVector3D& v2) {
//        float minX = std::min(v1.x(), v2.x());
//        float minY = std::min(v1.y(), v2.y());
//        float minZ = std::min(v1.z(), v2.z());

//        return QVector3D(minX, minY, minZ);
//    }

    QVector<Triangle>m_triangles;
    BVHNode* m_root;
};

#endif // BVH_H
