#pragma once

#include "shape.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <vector>

class Sphere : public Shape
{
public:
    Sphere(Shader *shader_program, float radius = 0.5f, int slices = 16);
    void draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection) override;

private:
    unsigned int num_indices;
    VAO vao;
    VBO vbo;
    EBO ebo;
};
