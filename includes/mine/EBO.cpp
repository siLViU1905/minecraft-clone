#include "EBO.h"
EBO::EBO()
{
    glGenBuffers(1, &ebo);
    bind();
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void EBO::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::setData(const void* data, size_t size, unsigned int drawType) const
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data,drawType);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &ebo);
}
