#ifndef RAY_H
#define RAY_H
#include "triangle.h"
#include <QVector3D>
#define INF 114514.0
// �󽻽��
struct HitResult {
    Triangle* triangle = NULL;
    float distance = INF;
};
// ����
typedef struct Ray {
    QVector3D startPoint = QVector3D(0, 0, 0);    // ���
    QVector3D direction = QVector3D(0, 0, 0);     // ����
}Ray;

#endif // RAY_H
