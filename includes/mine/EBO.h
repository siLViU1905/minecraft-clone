#ifndef __EBO_H__
#define __EBO_H__
#include "../GL/glad.h"

class EBO
{
   unsigned int ebo;
   public:
   EBO();

   void bind() const;

   void unbind() const;

   void setData(const void* data, size_t size, unsigned int drawType = GL_STATIC_DRAW) const;

   ~EBO();
};

#endif // __EBO_H__