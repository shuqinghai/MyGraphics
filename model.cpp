#include "model.h"

Model::Model(QString filepath,QOpenGLContext *context)
{
    readObj(filepath);
    bindData(context);
    m_BVH = new BVH(m_vertices,m_indices);
}

Model::~Model()
{
    delete m_BVH;
}

void Model::readObj(QString filepath)
{
    // ���ļ���
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "�ļ�" << filepath << "��ʧ��";
        exit(-1);
    }

    // ������ȡ
    int offset = m_vertices.size();

    // ���ж�ȡ
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList tokens = line.split(" ", QString::SkipEmptyParts);

        if (tokens.isEmpty())
            continue;

        QString type = tokens[0];
        if (type == "v") {
            if (tokens.size() >= 4) {
                GLfloat x = tokens[1].toFloat();
                GLfloat y = tokens[2].toFloat();
                GLfloat z = tokens[3].toFloat();
                m_vertices.push_back(QVector3D(x, y, z));
            }
        } else if (type == "f") {
            if (tokens.size() >= 4) {
                int v0 = tokens[1].toInt() - 1 + offset;
                int v1 = tokens[2].toInt() - 1 + offset;
                int v2 = tokens[3].toInt() - 1 + offset;
                m_indices.push_back(v0);
                m_indices.push_back(v1);
                m_indices.push_back(v2);
            }
        }
    }
    for (auto& v : m_vertices) {
        v[0] *= 5.0, v[1] *= 5.0, v[2] *= 5.0;
        v[1] -= 0.5;
    }
    file.close();
}

void Model::bindData(QOpenGLContext *context)
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
