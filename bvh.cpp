#include "bvh.h"

BVH::BVH(const QVector<QVector3D>& vertices,const QVector<GLuint>& indices)
{
    incomeTriangles(vertices,indices);
    m_root = buildBVHwithSAH(m_triangles, 0, m_triangles.size() - 1, 8);
}

HitResult BVH::hitBVH(Ray ray)
{
    HitResult res = hitBVH(ray,m_triangles,m_root);
    return res;;
}

BVH::BVHNode *BVH::buildBVH(QVector<Triangle> &triangles, int l, int r, int n)
{
    if (l > r) return 0;

       BVHNode* node = new BVHNode();
       node->AA = QVector3D(1145141919.0f, 1145141919.0f, 1145141919.0f);
       node->BB = QVector3D(-1145141919.0f, -1145141919.0f, -1145141919.0f);

       // ���� AABB
       for (int i = l; i <= r; i++) {
           // ��С�� AA
           float minx = min(triangles[i].p1[0], min(triangles[i].p2[0], triangles[i].p3[0]));
           float miny = min(triangles[i].p1[1], min(triangles[i].p2[1], triangles[i].p3[1]));
           float minz = min(triangles[i].p1[2], min(triangles[i].p2[2], triangles[i].p3[2]));
           node->AA[0] = min(node->AA[0], minx);
           node->AA[1] = min(node->AA[1], miny);
           node->AA[2] = min(node->AA[2], minz);
           // ���� BB
           float maxx = max(triangles[i].p1[0], max(triangles[i].p2[0], triangles[i].p3[0]));
           float maxy = max(triangles[i].p1[1], max(triangles[i].p2[1], triangles[i].p3[1]));
           float maxz = max(triangles[i].p1[2], max(triangles[i].p2[2], triangles[i].p3[2]));
           node->BB[0] = max(node->BB[0], maxx);
           node->BB[1] = max(node->BB[1], maxy);
           node->BB[2] = max(node->BB[2], maxz);
       }

       // ������ n �������� ����Ҷ�ӽڵ�
       if ((r - l + 1) <= n) {
           node->n = r - l + 1;
           node->index = l;
           return node;
       }

       // ����ݹ齨��
       float lenx = node->BB[0] - node->AA[0];
       float leny = node->BB[1] - node->AA[1];
       float lenz = node->BB[2] - node->AA[2];
       // �� x ����
       if (lenx >= leny && lenx >= lenz)
           std::sort(triangles.begin() + l, triangles.begin() + r + 1,  cmpx);
       // �� y ����
       if (leny >= lenx && leny >= lenz)
           std::sort(triangles.begin() + l, triangles.begin() + r + 1,  cmpy);
       // �� z ����
       if (lenz >= lenx && lenz >= leny)
           std::sort(triangles.begin() + l, triangles.begin() + r + 1,  cmpz);

       // �ݹ�
       int mid = (l + r) / 2;
       node->left = buildBVH(triangles, l, mid, n);
       node->right = buildBVH(triangles, mid + 1, r, n);

       return node;
}

BVH::BVHNode *BVH::buildBVHwithSAH(QVector<Triangle> &triangles, int l, int r, int n)
{
    if (l > r) return 0;

       BVHNode* node = new BVHNode();
       node->AA = QVector3D(1145141919.0f, 1145141919.0f, 1145141919.0f);
       node->BB = QVector3D(-1145141919.0f, -1145141919.0f, -1145141919.0f);

       // ���� AABB
       for (int i = l; i <= r; i++) {
           // ��С�� AA
           float minx = min(triangles[i].p1[0], min(triangles[i].p2[0], triangles[i].p3[0]));
           float miny = min(triangles[i].p1[1], min(triangles[i].p2[1], triangles[i].p3[1]));
           float minz = min(triangles[i].p1[2], min(triangles[i].p2[2], triangles[i].p3[2]));
           node->AA[0] = min(node->AA[0], minx);
           node->AA[1] = min(node->AA[1], miny);
           node->AA[2] = min(node->AA[2], minz);
           // ���� BB
           float maxx = max(triangles[i].p1[0], max(triangles[i].p2[0], triangles[i].p3[0]));
           float maxy = max(triangles[i].p1[1], max(triangles[i].p2[1], triangles[i].p3[1]));
           float maxz = max(triangles[i].p1[2], max(triangles[i].p2[2], triangles[i].p3[2]));
           node->BB[0] = max(node->BB[0], maxx);
           node->BB[1] = max(node->BB[1], maxy);
           node->BB[2] = max(node->BB[2], maxz);
       }

       // ������ n �������� ����Ҷ�ӽڵ�
       if ((r - l + 1) <= n) {
           node->n = r - l + 1;
           node->index = l;
           return node;
       }

       // ����ݹ齨��
       float Cost = INF;
       int Axis = 0;
       int Split = (l + r) / 2;
       for (int axis = 0; axis < 3; axis++) {
           // �ֱ� x��y��z ������
           if (axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpx);
           if (axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpy);
           if (axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpz);

           // leftMax[i]: [l, i] ������ xyz ֵ
           // leftMin[i]: [l, i] ����С�� xyz ֵ
           QVector<QVector3D> leftMax(r - l + 1, QVector3D(-INF, -INF, -INF));
           QVector<QVector3D> leftMin(r - l + 1, QVector3D(INF, INF, INF));
           // ����ǰ׺ ע�� i-l �Զ��뵽�±� 0
           for (int i = l; i <= r; i++) {
               Triangle& t = triangles[i];
               int bias = (i == l) ? 0 : 1;  // ��һ��Ԫ�����⴦��

               leftMax[i - l][0] = max(leftMax[i - l - bias][0], max(t.p1[0], max(t.p2[0], t.p3[0])));
               leftMax[i - l][1] = max(leftMax[i - l - bias][1], max(t.p1[1], max(t.p2[1], t.p3[1])));
               leftMax[i - l][2] = max(leftMax[i - l - bias][2], max(t.p1[2], max(t.p2[2], t.p3[2])));

               leftMin[i - l][0] = min(leftMin[i - l - bias][0], min(t.p1[0], min(t.p2[0], t.p3[0])));
               leftMin[i - l][1] = min(leftMin[i - l - bias][1], min(t.p1[1], min(t.p2[1], t.p3[1])));
               leftMin[i - l][2] = min(leftMin[i - l - bias][2], min(t.p1[2], min(t.p2[2], t.p3[2])));
           }

           // rightMax[i]: [i, r] ������ xyz ֵ
           // rightMin[i]: [i, r] ����С�� xyz ֵ
           QVector<QVector3D> rightMax(r - l + 1, QVector3D(-INF, -INF, -INF));
           QVector<QVector3D> rightMin(r - l + 1, QVector3D(INF, INF, INF));
           // �����׺ ע�� i-l �Զ��뵽�±� 0
           for (int i = r; i >= l; i--) {
               Triangle& t = triangles[i];
               int bias = (i == r) ? 0 : 1;  // ��һ��Ԫ�����⴦��

               rightMax[i - l][0] = max(rightMax[i - l + bias][0], max(t.p1[0], max(t.p2[0], t.p3[0])));
               rightMax[i - l][1] = max(rightMax[i - l + bias][1], max(t.p1[1], max(t.p2[1], t.p3[1])));
               rightMax[i - l][2] = max(rightMax[i - l + bias][2], max(t.p1[2], max(t.p2[2], t.p3[2])));

               rightMin[i - l][0] = min(rightMin[i - l + bias][0], min(t.p1[0], min(t.p2[0], t.p3[0])));
               rightMin[i - l][1] = min(rightMin[i - l + bias][1], min(t.p1[1], min(t.p2[1], t.p3[1])));
               rightMin[i - l][2] = min(rightMin[i - l + bias][2], min(t.p1[2], min(t.p2[2], t.p3[2])));
           }

           // ����Ѱ�ҷָ�
           float cost = INF;
           int split = l;
           for (int i = l; i <= r - 1; i++) {
               float lenx, leny, lenz;
               // ��� [l, i]
               QVector3D leftAA = leftMin[i - l];
               QVector3D leftBB = leftMax[i - l];
               lenx = leftBB[0] - leftAA[0];
               leny = leftBB[1] - leftAA[1];
               lenz = leftBB[2] - leftAA[2];
               float leftS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
               float leftCost = leftS * (i - l + 1);

               // �Ҳ� [i+1, r]
               QVector3D rightAA = rightMin[i + 1 - l];
               QVector3D rightBB = rightMax[i + 1 - l];
               lenx = rightBB[0] - rightAA[0];
               leny = rightBB[1] - rightAA[1];
               lenz = rightBB[2] - rightAA[2];
               float rightS = 2.0 * ((lenx * leny) + (lenx * lenz) + (leny * lenz));
               float rightCost = rightS * (r - i);

               // ��¼ÿ���ָ����С��
               float totalCost = leftCost + rightCost;
               if (totalCost < cost) {
                   cost = totalCost;
                   split = i;
               }
           }
           // ��¼ÿ�������Ѵ�
           if (cost < Cost) {
               Cost = cost;
               Axis = axis;
               Split = split;
           }
       }

       // �������ָ�
       if (Axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpx);
       if (Axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpy);
       if (Axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1,  cmpz);

       // �ݹ�
       node->left = buildBVHwithSAH(triangles, l, Split, n);
       node->right = buildBVHwithSAH(triangles, Split + 1, r, n);

       return node;
}

HitResult BVH::hitBVH(Ray ray, QVector<Triangle> &triangles, BVH::BVHNode *root)
{
    if (root == NULL) return HitResult();

       // ��Ҷ�� ������
       if (root->n > 0) {
           return GeometricCalculation::hitTriangleArray(ray, triangles, root->n, root->n + root->index - 1);
       }

       // ���������� AABB ��
       float d1 = INF, d2 = INF;
       if (root->left) d1 = GeometricCalculation::hitAABB(ray, root->left->AA, root->left->BB);
       if (root->right) d2 = GeometricCalculation::hitAABB(ray, root->right->AA, root->right->BB);

       // �ݹ���
       HitResult r1, r2;
       if (d1 > 0) r1 = hitBVH(ray, triangles, root->left);
       if (d2 > 0) r2 = hitBVH(ray, triangles, root->right);

       return r1.distance < r2.distance ? r1 : r2;
}

void BVH::incomeTriangles(const QVector<QVector3D> &vertices, const QVector<GLuint> &indices)
{
    for(int i = 0;i < indices.size();i += 3)
    {
        m_triangles.push_back(Triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]));
    }
}

//float BVH::hitTriangle(Triangle *triangle, Ray ray)
//{
//    QVector3D p1 = triangle->p1, p2 = triangle->p2, p3 = triangle->p3;
//      QVector3D S = ray.startPoint;        // �������
//      QVector3D d = ray.direction;         // ���߷���
//      QVector3D N = (QVector3D::crossProduct(p2 - p1, p3 - p1)).normalized();    // ������
//      if (QVector3D::dotProduct(N, d) > 0.0f) N = -N;   // ��ȡ��ȷ�ķ�����

//      // ������ߺ�������ƽ��
//      if (fabs(QVector3D::dotProduct(N, d)) < 0.00001f) return INF;

//      // ����
//      float t = (QVector3D::dotProduct(N, p1) - QVector3D::dotProduct(S, N)) / QVector3D::dotProduct(d, N);
//      if (t < 0.0005f) return INF;    // ����������ڹ��߱���

//      // �������
//      QVector3D P = S + d * t;

//      // �жϽ����Ƿ�����������
//      QVector3D c1 = QVector3D::crossProduct(p2 - p1, P - p1);
//      QVector3D c2 = QVector3D::crossProduct(p3 - p2, P - p2);
//      QVector3D c3 = QVector3D::crossProduct(p1 - p3, P - p3);
//      if (QVector3D::dotProduct(c1, N) > 0 && QVector3D::dotProduct(c2, N) > 0 && QVector3D::dotProduct(c3, N) > 0) return t;
//      if (QVector3D::dotProduct(c1, N) < 0 && QVector3D::dotProduct(c2, N) < 0 && QVector3D::dotProduct(c3, N) < 0) return t;

//      return INF;
//}

//HitResult BVH::hitTriangleArray(Ray ray, QVector<Triangle> &triangles, int l, int r)
//{
//    HitResult res;
//       for (int i = l; i <= r; i++) {
//           float d = hitTriangle(&triangles[i], ray);
//           if (d < INF && d < res.distance) {
//               res.distance = d;
//               res.triangle = &triangles[i];
//           }
//       }
//       return res;
//}

//float BVH::hitAABB(Ray r, QVector3D AA, QVector3D BB)
//{
//    // 1.0 / direction
//       QVector3D invdir = QVector3D(1.0 / r.direction[0], 1.0 / r.direction[1], 1.0 / r.direction[2]);

//       QVector3D in = (BB - r.startPoint) * invdir;
//       QVector3D out = (AA - r.startPoint) * invdir;

//       QVector3D tmax = maxVec(in, out);
//       QVector3D tmin = minVec(in, out);

//       float t1 = min(tmax[0], min(tmax[1], tmax[2]));
//       float t0 = max(tmin[0], max(tmin[1], tmin[2]));

//       return (t1 >= t0) ? ((t0 > 0.0) ? (t0) : (t1)) : (-1);
//}

bool BVH::getAABB(QVector3D &AA, QVector3D &BB)
{
    if(!m_root)
    {
        return false;
    }
    else
    {
        AA = m_root->AA;
        BB = m_root->BB;
    }
    return true;
}
