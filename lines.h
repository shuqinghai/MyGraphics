#ifndef LINES_H
#define LINES_H
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <memory>
#include <QOpenGLVertexArrayObject>
#include <QVector>
#include <QOpenGLFunctions>
#include "triangle.h"
class Lines
{
public:
    Lines();
    void bindData(QOpenGLContext *context);
    void updateVertices();
    void addBox(QVector3D AA,QVector3D BB);
    void addTriangle(Triangle triangle);
    QVector<QVector3D> m_vertices = { {0,0,0},{1,1,1} };     // 顶点坐标
    QVector<GLuint> m_indices = {0,0,0};    // 顶点索引
    //OpenGL
    std::shared_ptr<QOpenGLVertexArrayObject> m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    std::shared_ptr<QOpenGLContext> m_context;//openGL上下文
};

#endif // LINES_H
