#ifndef RAY_H
#define RAY_H
#include "triangle.h"
#include <QVector3D>
#define INF 114514.0
// 求交结果
struct HitResult {
    Triangle* triangle = NULL;
    float distance = INF;
};
// 光线
typedef struct Ray {
    QVector3D startPoint = QVector3D(0, 0, 0);    // 起点
    QVector3D direction = QVector3D(0, 0, 0);     // 方向
}Ray;

#endif // RAY_H
