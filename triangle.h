#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QVector3D>

struct Triangle;
bool cmpx(const Triangle& t1, const Triangle& t2);
bool cmpy(const Triangle& t1, const Triangle& t2);
bool cmpz(const Triangle& t1, const Triangle& t2);

typedef struct Triangle{
    QVector3D p1,p2,p3;     //����
    QVector3D center;       //����
    Triangle(QVector3D a,QVector3D b,QVector3D c){
        p1 = a,p2 = b,p3 = c;
        center = (p1+p2+p3)/QVector3D(3,3,3);
    }
    Triangle()
    {
        p1 = QVector3D(0,0,0),p2 = QVector3D(0,0,0),p3 = QVector3D(0,0,0);
        center = (p1+p2+p3)/QVector3D(3,3,3);
    }
}Triangle;


#endif // TRIANGLE_H
