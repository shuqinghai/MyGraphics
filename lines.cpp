#include "lines.h"

Lines::Lines()
{

}

void Lines::updateVertices()
{
    QOpenGLFunctions* f = m_context->functions();//openGL����
    m_vao->bind();
    //����vbo�����Ұ�vbo
    QOpenGLBuffer vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    if (vbo.isCreated())
        vbo.bind();
    vbo.allocate(sizeof(QVector3D) * (m_vertices.size()));//����vbo�Ļ���ռ�

    // ��λ��
    GLuint offset_position = 0;//��������ƫ����
    GLuint size_position = m_vertices.size() * sizeof(QVector3D);
    m_vbo.write(offset_position, &m_vertices[0], size_position);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  // vaoָ��vPosition���������ݽ�����ʽ
    f->glEnableVertexAttribArray(0);
    ////������
    //GLuint offset_normal = size_position;
    //GLuint size_normal = m_vertexNormal.size() * sizeof(QVector3D);
    //m_vbo.write(offset_normal, &m_vertexNormal[0], size_normal);

    vbo.release();//�ͷ�vbo
    m_vao->release();//�ͷ�vao
}

void Lines::addBox(QVector3D AA, QVector3D BB)
{
	float x1 = AA.x(), y1 = AA.y(), z1 = AA.z();
	float x2 = BB.x(), y2 = BB.y(), z2 = BB.z();
	m_vertices.push_back(QVector3D(x1, y1, z1)), m_vertices.push_back(QVector3D(x2, y1, z1));
	m_vertices.push_back(QVector3D(x1, y1, z1)), m_vertices.push_back(QVector3D(x1, y1, z2));
	m_vertices.push_back(QVector3D(x1, y1, z1)), m_vertices.push_back(QVector3D(x1, y2, z1));
	m_vertices.push_back(QVector3D(x2, y1, z1)), m_vertices.push_back(QVector3D(x2, y1, z2));
	m_vertices.push_back(QVector3D(x2, y1, z1)), m_vertices.push_back(QVector3D(x2, y2, z1));
	m_vertices.push_back(QVector3D(x1, y2, z1)), m_vertices.push_back(QVector3D(x2, y2, z1));
	m_vertices.push_back(QVector3D(x1, y1, z2)), m_vertices.push_back(QVector3D(x1, y2, z2));
	m_vertices.push_back(QVector3D(x1, y2, z1)), m_vertices.push_back(QVector3D(x1, y2, z2));
	m_vertices.push_back(QVector3D(x1, y2, z2)), m_vertices.push_back(QVector3D(x2, y2, z2));
	m_vertices.push_back(QVector3D(x1, y1, z2)), m_vertices.push_back(QVector3D(x2, y1, z2));
	m_vertices.push_back(QVector3D(x2, y2, z1)), m_vertices.push_back(QVector3D(x2, y2, z2));
	m_vertices.push_back(QVector3D(x2, y1, z2)), m_vertices.push_back(QVector3D(x2, y2, z2));
}

void Lines::addTriangle(Triangle triangle)
{
    m_vertices.push_back(triangle.p1), m_vertices.push_back(triangle.p2);
    m_vertices.push_back(triangle.p2), m_vertices.push_back(triangle.p3);
    m_vertices.push_back(triangle.p3), m_vertices.push_back(triangle.p1);
}
void Lines::bindData(QOpenGLContext *context)
{
    m_context = std::make_shared<QOpenGLContext>();
    m_context->create();
    m_context->setShareContext(context);
    QOpenGLFunctions* f = m_context->functions();//openGL����

    //����vao����vao
    m_vao = std::make_shared<QOpenGLVertexArrayObject>();
    m_vao->create();
    if (m_vao->isCreated())
        m_vao->bind();
    //����vbo�����Ұ�vbo
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vbo.create();
    if (m_vbo.isCreated())
        m_vbo.bind();
    m_vbo.allocate(sizeof(QVector3D) * (m_vertices.size()));//����vbo�Ļ���ռ�

    // ��λ��
    GLuint offset_position = 0;//��������ƫ����
    GLuint size_position = m_vertices.size()*sizeof(QVector3D);
    m_vbo.write(offset_position, &m_vertices[0], size_position);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  // vaoָ��vPosition���������ݽ�����ʽ
    f->glEnableVertexAttribArray(0);
    ////������
    //GLuint offset_normal = size_position;
    //GLuint size_normal = m_vertexNormal.size() * sizeof(QVector3D);
    //m_vbo.write(offset_normal, &m_vertexNormal[0], size_normal);

    //����ebo����ebo����
    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ebo.allocate(sizeof(int) * m_indices.size());
    m_ebo.write(0, &m_indices[0], sizeof(int) * m_indices.size());

    m_vbo.release();//�ͷ�vbo
    m_vao->release();//�ͷ�vao
}
