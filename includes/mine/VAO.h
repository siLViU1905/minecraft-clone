#ifndef __VAO_H__
#define __VAO_H__
#include "VBO.h"

class VAO
{
    unsigned int vao;
    public:
    VAO();

   void bind() const;

   void setData(const void* data, size_t size, unsigned int drawType = GL_STATIC_DRAW) const;

   void linkAttrib(const VBO& vbo, unsigned int index, int size, GLenum type, 
    GLboolean normalized, GLsizei stride, const void* pointer) const;

    void enableAttrib(int index) const;

   ~VAO();
};


#endif // __VAO_H__