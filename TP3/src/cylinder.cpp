#include "cylinder.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "VAO.h"
#include "VBO.h"

Cylinder::Cylinder(Shader *shader_program, float height, float radius, int slices)
    : Shape(shader_program)
{
    // generate vertices
    std::vector<glm::vec3> vertices;
    for (int i = 0; i < slices; i++)
    {
        float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
        float x = radius * glm::cos(theta);
        float y = radius * glm::sin(theta);
        vertices.push_back(glm::vec3(x, y, 0.5f * height));
        vertices.push_back(glm::vec3(x, y, -0.5f * height));
    }

    // add top and bottom vertices
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.5f * height));
    vertices.push_back(glm::vec3(0.0f, 0.0f, -0.5f * height));

    // generate indices
    std::vector<GLuint> indices;
    // side triangles
    for (int i = 0; i < slices; i++)
    {
        // first triangle of the quad
        indices.push_back(2 * i);
        indices.push_back(2 * i + 1);
        indices.push_back((2 * i + 2) % (2 * slices));

        // Second triangle of the quad
        indices.push_back(2 * i + 1);
        indices.push_back((2 * i + 3) % (2 * slices));
        indices.push_back((2 * i + 2) % (2 * slices));
    }

    // top circle
    for (int i = 0; i < slices; i++)
    {
        indices.push_back(2 * i);                      
        indices.push_back((2 * i + 2) % (2 * slices)); 
        indices.push_back(2 * slices);                 
    }

    // bottom circle
    for (int i = 0; i < slices; i++)
    {
        indices.push_back(2 * i + 1);                  
        indices.push_back(2 * slices + 1);             
        indices.push_back((2 * i + 3) % (2 * slices)); 
    }

    // generate GLfloat and GLuint arrays

    GLfloat RealVertices[vertices.size() * 6];
    for (size_t i = 0; i < vertices.size(); i++)
    {
        RealVertices[i * 6] = vertices[i].x;     // position X
        RealVertices[i * 6 + 1] = vertices[i].y; // position Y
        RealVertices[i * 6 + 2] = vertices[i].z; // position Z

        // Dynamically calculate colors based on vertex position
        RealVertices[i * 6 + 3] = (vertices[i].x + radius) / (2 * radius); // R
        RealVertices[i * 6 + 4] = (vertices[i].y + radius) / (2 * radius); // G
        RealVertices[i * 6 + 5] = (vertices[i].z + height) / (2 * height); // B
    }

    GLuint RealIndices[indices.size()];
    for (size_t i = 0; i < indices.size(); i++)
    {
        RealIndices[i] = indices[i];
    }

    // BEFORE
    // glGenVertexArrays(1, &test);
    // glBindVertexArray(test);

    // glGenBuffers(2, &buffers[0]);

    // // create vertex buffer
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // // create index buffer
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // AFTER (using my classes)

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

void Cylinder::draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection)
{
    glUseProgram(this->shader_program_ID_);

    // BEFORE
    // glGenVertexArrays(1, &test);

    // AFTER
    vao.Bind();

    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, nullptr);
}
