#include "VBO.h"


VBO::VBO()
{
    glGenBuffers(1, &vbo);
    bind();
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::setData(const void* data, size_t size, unsigned int drawType) const
{
    glBufferData(GL_ARRAY_BUFFER, size, data,drawType);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &vbo);
}
