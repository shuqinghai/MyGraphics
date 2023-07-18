#ifndef MODEL_H
#define MODEL_H
#include <QString>
#include <QVector>
#include <QVector3D>

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <QDebug>
#include <memory>

#include "bvh.h"
class Model
{
public:
    Model(QString filepath,QOpenGLContext *context);
    ~Model();
    // 读取 obj
    void readObj(QString filepath);
    void bindData(QOpenGLContext* context);
    //几何信息
    std::shared_ptr<QOpenGLVertexArrayObject> m_vao;
    //BVH
    BVH *m_BVH;

    QVector<QVector3D> m_vertices;     // 顶点坐标
    QVector<GLuint> m_indices;    // 顶点索引
    QVector<QVector3D>m_vertexNormal;//法线
    //OpenGL

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    std::shared_ptr<QOpenGLContext> m_context;//openGL上下文

};

#endif // MODEL_H
