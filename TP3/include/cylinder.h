#pragma once

#include "shape.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Cylinder : public Shape
{
public:
    Cylinder(Shader *shader_program, float height = 1.0f, float radius = 0.5f, int slices = 16);
    void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) override;

private:
    unsigned int num_indices;
    VAO vao;
    VBO vbo;
    EBO ebo;

    // BEFORE
    //  GLuint test;
    //  GLuint buffers[2];
};
