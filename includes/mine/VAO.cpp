#include "VAO.h"
VAO::VAO()
{
    glGenVertexArrays(1,&vao);
    bind();
}

void VAO::bind() const 
{
    glBindVertexArray(vao);
}

void VAO::setData(const void* data, size_t size, unsigned int drawType) const 
{
    return;
}

void VAO::linkAttrib(const VBO& vbo, unsigned int index, int size, GLenum type, 
    GLboolean normalized, GLsizei stride, const void* pointer) const
{
    bind();
    vbo.bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void VAO::enableAttrib(int index) const
{
    glEnableVertexAttribArray(index);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &vao);
}
