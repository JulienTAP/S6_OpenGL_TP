#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<GL/glew.h>
#include"VBO.h"

class VAO
{
    public:
        GLuint ID;
        VAO();

        void LinkAttrib(VBO VBO, GLuint layout, GLuint numComp, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void Delete();
};

#endif