#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    float FOV;
    float nearPlane;
    float farPlane;
    int width;
    int height;

    bool firstClick = true;

    float speed = 0.1f;
    float sensitivity = 100.0f;

    Camera(int width, int height, glm::vec3 position, float FOV, float nearPlane, float farPlane);

    void Inputs(GLFWwindow *window);

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Orientation, Up);
    }

    glm::mat4 getProjectionMatrix()
    {
        return glm::perspective(glm::radians(FOV), (float)(width / height), nearPlane, farPlane);
    }
};

#endif