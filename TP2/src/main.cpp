#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"

/// constants for the camera
const float FOV = 45.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

const unsigned int width = 800;
const unsigned int height = 800;

// use left mouse button to interact with the camera
// use z, q, d, d to move the camera
// use left control to move down
// use space to move up

int main(){

    GLfloat verticies[] = {
        -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    
        -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    
         0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,    
         0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,    
         0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,    
    };

    GLuint indices[] = 
    {
        0,1,2,
        0,2,3,
        0,1,4,
        1,2,4,
        2,3,4,
        3,0,4,
    };


    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    

    GLFWwindow* window = glfwCreateWindow(width,height,"TP2 - Julien TAP",NULL,NULL);
    if(window==NULL){
        std::cout<<"failed creating the window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!glewInit()){
        perror("failed init");
    }

    glViewport(0,0,width,height);

    Shader shaderProgram("./shaders/default.vert.txt", "./shaders/default.frag.txt");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(verticies,sizeof(verticies));
    EBO EBO1(indices,sizeof(indices));

    VAO1.LinkAttrib(VBO1,0,3,GL_FLOAT,6*sizeof(float),(void*)0);
    VAO1.LinkAttrib(VBO1, 1,3,GL_FLOAT, 6* sizeof(float), (void*)(3*sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    glClearColor(0.07f,0.13f,0.17f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    glEnable(GL_DEPTH_TEST);

   // create camera
   Camera camera(width, height, glm::vec3(0.0f, 0.5f, 3.0f), FOV, nearPlane, farPlane);
   glm::mat4 modelMatrix = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f,0.13f,0.17f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();

        camera.Inputs(window);

        // retrieve view and projection matrices
        glm::mat4 viewMatrix = camera.getViewMatrix();
        glm::mat4 projectionMatrix = camera.getProjectionMatrix();

        // pass matrices to shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

        //rotate the pyramid using model matrix
        modelMatrix = glm::rotate(modelMatrix, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int),GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();    

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}