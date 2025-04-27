#include "sphere.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "VAO.h"
#include "VBO.h"

Sphere::Sphere(Shader *shader_program, float radius, int faces)
    : Shape(shader_program)
{
    // generate vertices
    std::vector<glm::vec3> vertices;
    for (int i = 0; i <= faces; i++)
    {
        float theta = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(faces);
        for (int j = 0; j <= faces; j++)
        {
            float phi = 2.0f * glm::pi<float>() * static_cast<float>(j) / static_cast<float>(faces);
            float x = radius * glm::sin(theta) * glm::cos(phi);
            float y = radius * glm::sin(theta) * glm::sin(phi);
            float z = radius * glm::cos(theta);
            vertices.push_back(glm::vec3(x, y, z));
        }
    }
    // add top and bottom vertices
    vertices.push_back(glm::vec3(0.0f, 0.0f, radius));
    vertices.push_back(glm::vec3(0.0f, 0.0f, -radius));

    // generate indices
    std::vector<GLuint> indices;
    // side triangles
    for (int i = 0; i < faces; i++)
    {
        for (int j = 0; j < faces; j++)
        {
            int first = (i * (faces + 1)) + j;
            int second = first + faces + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    // top and bottom triangles
    for (int i = 0; i < faces; i++)
    {
        int first = (faces * (faces + 1)) + i;
        int second = (faces * (faces + 1)) + i + 1;
        indices.push_back(first);
        indices.push_back(second);
        indices.push_back(vertices.size() - 2);

        first = (faces * (faces + 1)) + i + faces * (faces + 1);
        second = (faces * (faces + 1)) + i + faces * (faces + 1) + 1;
        indices.push_back(first);
        indices.push_back(vertices.size() - 1);
        indices.push_back(second);
    }

    // generate GLfloat and GLuint arrays

    GLfloat RealVertices[vertices.size() * 6];
    for (size_t i = 0; i < vertices.size(); i++)
    {
        RealVertices[i * 6] = vertices[i].x;
        RealVertices[i * 6 + 1] = vertices[i].y;
        RealVertices[i * 6 + 2] = vertices[i].z;
        RealVertices[i * 6 + 3] = (vertices[i].x + radius) / (2 * radius); // R
        RealVertices[i * 6 + 4] = (vertices[i].y + radius) / (2 * radius); // G
        RealVertices[i * 6 + 5] = (vertices[i].z + radius) / (2 * radius); // B
    }

    GLuint RealIndices[indices.size()];
    for (size_t i = 0; i < indices.size(); i++)
    {
        RealIndices[i] = indices[i];
    }

    vao.Bind();
    vbo = VBO(RealVertices, sizeof(RealVertices));
    ebo = EBO(RealIndices, sizeof(RealIndices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    num_indices = static_cast<unsigned int>(indices.size());
}

void Sphere::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection)
{
    glUseProgram(this->shader_program_ID_);

    vao.Bind();

    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, nullptr);
}
