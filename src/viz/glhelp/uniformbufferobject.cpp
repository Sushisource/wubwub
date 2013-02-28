#include "uniformbufferobject.h"

#include <QGLContext>

#include <QDebug>

QVector<GLuint> UniformBufferObject::m_bindingIndices = QVector<GLuint>();

GLuint UniformBufferObject::bindingIndex() {
    for(GLuint i = 0, size = m_bindingIndices.size(); i < size; i++) {
        if(m_bindingIndices.at(i) != i) {
            m_bindingIndices.insert(i, i);
            return i;
        }
    }
    m_bindingIndices.append(m_bindingIndices.size());
    return m_bindingIndices.size();
}

UniformBufferObject::UniformBufferObject() :
    m_bufferId(0), m_glContext(NULL), m_inited(false),
    glBindBuffer(NULL), glBindBufferBase(NULL), glBindBufferRange(NULL), glBufferData(NULL), glDeleteBuffers(NULL), glGenBuffers(NULL),
    glGetActiveUniformBlockiv(NULL), glGetActiveUniformsiv(NULL), glGetUniformBlockIndex(NULL), glGetUniformIndices(NULL),
    glUniformBlockBinding(NULL)
{
}

bool UniformBufferObject::create() {
    const QGLContext* m_glContext = QGLContext::currentContext();

    if(m_glContext) {
        if(!m_inited && !initFunctionPointers(m_glContext)) {
            qDebug("Cannot find Uniform Buffer Objects related functions");
            return false;
        }

        GLuint tmpBufferId = 0;
        glGenBuffers(1, &tmpBufferId);

        if(tmpBufferId) {
                m_bufferId = tmpBufferId;
            this->m_glContext = m_glContext;

            return true;
        } else {
            qDebug("Invalid buffer Id");
        }
    }

    qDebug("Could not retrieve buffer");

    return false;
}

bool UniformBufferObject::isCreated() const {
    return (bool)(m_bufferId != 0);
}

void UniformBufferObject::destroy() {
    if(m_bufferId != 0) {
        glDeleteBuffers(1, &m_bufferId);
    }
    m_bufferId = 0;
    m_glContext = NULL;
}

bool UniformBufferObject::bind() {
    if(!isCreated()) {
        qDebug("Buffer not created");
        return false;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);

    return true;
}

bool UniformBufferObject::bind(GLuint progId, QString uniformName) {
    GLuint tmpBlockIdx = glGetUniformBlockIndex(progId, uniformName.toUtf8());

    if(tmpBlockIdx == GL_INVALID_INDEX) {
        qDebug() << QString("Could not find block index of block named: %1").arg(uniformName);

        return false;
    }

    GLint tmpBlockSize;
    glGetActiveUniformBlockiv(progId, tmpBlockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &tmpBlockSize);

    GLuint tmpBindingIdx = bindingIndex();
    glUniformBlockBinding(progId, tmpBlockIdx, tmpBindingIdx);

    glBindBufferBase(GL_UNIFORM_BUFFER, tmpBindingIdx, m_bufferId);

    if(glGetError() == GL_INVALID_VALUE || glGetError() == GL_INVALID_ENUM) {
        qDebug() << "ERROR";
    }

    UBOInfo info;
    info.progId = progId;
    info.uniformName = uniformName;
    info.blockIdx = tmpBlockIdx;
    info.blockSize = tmpBlockSize;
    info.bindingIdx = tmpBindingIdx;

    m_UBOInfos.append(info);

    return true;
}

void UniformBufferObject::release() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint UniformBufferObject::bufferId() const {
    return m_bufferId;
}

void UniformBufferObject::write(int offset, const void *data, int count) {
    if(!isCreated())
        return;

    bind();

    glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data);
}

void UniformBufferObject::allocate(const void *data, int count) {
    if(!isCreated())
        return;

    bind();

    glBufferData(GL_UNIFORM_BUFFER, count, data, GL_DYNAMIC_DRAW);
}

bool UniformBufferObject::initFunctionPointers(const QGLContext* m_glContext) {
    glBindBuffer = (PFNGLBINDBUFFERPROC)m_glContext->getProcAddress("glBindBuffer");
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)m_glContext->getProcAddress("glBindBufferBase");
    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)m_glContext->getProcAddress("glBindBufferRange");
    glBufferData = (PFNGLBUFFERDATAPROC)m_glContext->getProcAddress("glBufferData");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)m_glContext->getProcAddress("glBufferSubData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)m_glContext->getProcAddress("glDeleteBuffers");
    glGenBuffers = (PFNGLGENBUFFERSPROC)m_glContext->getProcAddress("glGenBuffers");
    glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)m_glContext->getProcAddress("glGetActiveUniformBlockiv");
    glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)m_glContext->getProcAddress("glGetActiveUniformsiv");
    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)m_glContext->getProcAddress("glGetUniformBlockIndex");
    glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)m_glContext->getProcAddress("glGetUniformIndices");
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)m_glContext->getProcAddress("glUniformBlockBinding");

    if(!glBindBuffer ||
            !glBindBufferBase ||
            !glBindBufferRange ||
            !glBufferData ||
            !glBufferSubData ||
            !glDeleteBuffers ||
            !glGenBuffers ||
            !glGetActiveUniformBlockiv ||
            !glGetActiveUniformsiv ||
            !glGetUniformBlockIndex ||
            !glGetUniformIndices ||
            !glUniformBlockBinding)
    {
        qDebug("Could not init function pointers");
        return false;
    }

    return true;
}
