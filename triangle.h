#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QVector3D>

struct Triangle;
bool cmpx(const Triangle& t1, const Triangle& t2);
bool cmpy(const Triangle& t1, const Triangle& t2);
bool cmpz(const Triangle& t1, const Triangle& t2);

typedef struct Triangle{
    QVector3D p1,p2,p3;     //三点
    QVector3D center;       //中心
    Triangle(QVector3D a,QVector3D b,QVector3D c){
        p1 = a,p2 = b,p3 = c;
        center = (p1+p2+p3)/QVector3D(3,3,3);
    }
    Triangle()
    {
        p1 = QVector3D(0,0,0),p2 = QVector3D(0,0,0),p3 = QVector3D(0,0,0);
        center = (p1+p2+p3)/QVector3D(3,3,3);
    }
    QVector3D findClosestPoint(const QVector3D& point)
    {
        float d1 = (point - p1).length();
        float d2 = (point - p2).length();
        float d3 = (point - p3).length();
        if (d1 <= d2 && d1 <= d3) {
            return p1;
        }
        else if (d2 <= d1 && d2 <= d3) {
            return p2;
        }
        else {
            return p3;
        }
    }
}Triangle;


#endif // TRIANGLE_H
