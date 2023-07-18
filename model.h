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
    // ��ȡ obj
    void readObj(QString filepath);
    void bindData(QOpenGLContext* context);
    //������Ϣ
    std::shared_ptr<QOpenGLVertexArrayObject> m_vao;
    //BVH
    BVH *m_BVH;

    QVector<QVector3D> m_vertices;     // ��������
    QVector<GLuint> m_indices;    // ��������
    QVector<QVector3D>m_vertexNormal;//����
    //OpenGL

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;
    std::shared_ptr<QOpenGLContext> m_context;//openGL������

};

#endif // MODEL_H
