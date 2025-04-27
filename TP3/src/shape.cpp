// shape.cpp

#include "shape.h"

Shape::Shape(Shader *shader_program) : shader_program_ID_(shader_program->get_id())
{
}

void Shape::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection)
{

    GLint loc = glGetUniformLocation(this->shader_program_ID_, "modelMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));

    loc = glGetUniformLocation(this->shader_program_ID_, "viewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

    loc = glGetUniformLocation(this->shader_program_ID_, "projectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
}