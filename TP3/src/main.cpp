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
#include "shape.h"
#include "node.h"
#include "cylinder.h"
#include "sphere.h"

// screen size
const unsigned int width = 1000;
const unsigned int height = 1000;

// constants for the camera
const float FOV = 45.0f;
const float nearPlane = 0.1f;
const float farPlane = 100.0f;

// constants for the animation
float rightUpperAngle = 0.0f;
float rightLowerAngle = 0.0f;
float leftUpperAngle = 0.0f;
float leftLowerAngle = 0.0f;
bool starting = true;
bool rightDone = false;
bool leftDone = false;

int main()
{

    // Init GLFW
    glfwInit();

    // Set version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow *window = glfwCreateWindow(width, height, "TP3 - Julien TAP", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed creating the window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set the context for the current window
    glfwMakeContextCurrent(window);

    if (!glewInit())
    {
        perror("failed init");
    }

    // set viewport
    glViewport(0, 0, width, height);

    // create shaders
    Shader shaderProgram("./shaders/default.vert.txt", "./shaders/default.frag.txt");

    //  set some options
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glEnable(GL_DEPTH_TEST);

    // create camera
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 8.0f), FOV, nearPlane, farPlane);

    // root node as spine
    Node *root = new Node(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    Cylinder *spine = new Cylinder(&shaderProgram, 3.25f, 0.1f, 16);
    root->add(spine);

    // clavicle
    Node *clavicleNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.125f)));
    clavicleNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Cylinder *clavicle = new Cylinder(&shaderProgram, 2.0f, 0.1f, 16);
    clavicleNode->add(clavicle);
    root->add(clavicleNode);

    // shoulders
    Node *rightShoulderNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    Node *leftShoulderNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    Sphere *rightShoulder = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftShouler = new Sphere(&shaderProgram, 0.15f, 16);
    leftShoulderNode->add(leftShouler);
    rightShoulderNode->add(rightShoulder);
    clavicleNode->add(leftShoulderNode);
    clavicleNode->add(rightShoulderNode);

    // head
    Node *headNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.875f)));
    Sphere *head = new Sphere(&shaderProgram, 0.4f, 16);
    headNode->add(head);
    root->add(headNode);

    // upper arms
    Node *rightArmNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.0f, 0.0f)));
    rightArmNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Node *leftArmNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.0f, 0.0f)));
    leftArmNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Cylinder *rightArm = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    Cylinder *leftArm = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    rightArmNode->add(rightArm);
    leftArmNode->add(leftArm);
    rightShoulderNode->add(rightArmNode);
    leftShoulderNode->add(leftArmNode);

    // elbows
    Node *rightElbowNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftElbowNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Sphere *rightElbow = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftElbow = new Sphere(&shaderProgram, 0.15f, 16);
    rightElbowNode->add(rightElbow);
    leftElbowNode->add(leftElbow);
    rightArmNode->add(rightElbowNode);
    leftArmNode->add(leftElbowNode);

    // forearms
    Node *rightForearmNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftForearmNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Cylinder *rightForearm = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    Cylinder *leftForearm = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    rightForearmNode->add(rightForearm);
    leftForearmNode->add(leftForearm);
    rightElbowNode->add(rightForearmNode);
    leftElbowNode->add(leftForearmNode);

    // wrists
    Node *rightWristNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftWristNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Sphere *rightWrist = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftWrist = new Sphere(&shaderProgram, 0.15f, 16);
    rightWristNode->add(rightWrist);
    leftWristNode->add(leftWrist);
    rightForearmNode->add(rightWristNode);
    leftForearmNode->add(leftWristNode);

    // pelvis
    Node *pelvisNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.625f)));
    pelvisNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Cylinder *pelvis = new Cylinder(&shaderProgram, 1.0f, 0.1f, 16);
    pelvisNode->add(pelvis);
    root->add(pelvisNode);

    // hips
    Node *rightHipNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)));
    Node *leftHipNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0f, 0.5f)));
    Sphere *rightHip = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftHip = new Sphere(&shaderProgram, 0.15f, 16);
    rightHipNode->add(rightHip);
    leftHipNode->add(leftHip);
    pelvisNode->add(rightHipNode);
    pelvisNode->add(leftHipNode);

    // upper legs
    Node *rightUpperLegNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.0f, 0.0f)));
    rightUpperLegNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Node *leftUpperLegNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.0f, 0.0f)));
    leftUpperLegNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Cylinder *rightUpperLeg = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    Cylinder *leftUpperLeg = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    rightUpperLegNode->add(rightUpperLeg);
    leftUpperLegNode->add(leftUpperLeg);
    rightHipNode->add(rightUpperLegNode);
    leftHipNode->add(leftUpperLegNode);

    // knees
    Node *rightKneeNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftKneeNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Sphere *rightKnee = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftKnee = new Sphere(&shaderProgram, 0.15f, 16);
    rightKneeNode->add(rightKnee);
    leftKneeNode->add(leftKnee);
    rightUpperLegNode->add(rightKneeNode);
    leftUpperLegNode->add(leftKneeNode);

    // lower legs
    Node *rightLowerLegNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftLowerLegNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Cylinder *rightLowerLeg = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    Cylinder *leftLowerLeg = new Cylinder(&shaderProgram, 1.5f, 0.1f, 16);
    rightLowerLegNode->add(rightLowerLeg);
    leftLowerLegNode->add(leftLowerLeg);
    rightKneeNode->add(rightLowerLegNode);
    leftKneeNode->add(leftLowerLegNode);

    // ankles
    Node *rightAnkleNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Node *leftAnkleNode = new Node(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.75f)));
    Sphere *rightAnkle = new Sphere(&shaderProgram, 0.15f, 16);
    Sphere *leftAnkle = new Sphere(&shaderProgram, 0.15f, 16);
    rightAnkleNode->add(rightAnkle);
    leftAnkleNode->add(leftAnkle);
    rightLowerLegNode->add(rightAnkleNode);
    leftLowerLegNode->add(leftAnkleNode);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        shaderProgram.Activate();

        // check camera inputs
        camera.Inputs(window);

        // Retrieve view and projection matrices
        glm::mat4 viewMatrix = camera.getViewMatrix();
        glm::mat4 projectionMatrix = camera.getProjectionMatrix();

        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // draw the root node
        root->draw(modelMatrix, viewMatrix, projectionMatrix);

        // animation (while F key is pressed)
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            if (starting)
            {
                rightUpperAngle += 1.0f;
                leftLowerAngle += 1.5f;
                rightShoulderNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                rightElbowNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                leftHipNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
                leftKneeNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
                if (rightUpperAngle >= 30.0f)
                {
                    starting = false;
                    rightDone = true;
                }
            }
            else if (rightDone)
            {
                rightUpperAngle -= 1.0f;
                leftLowerAngle -= 1.5f;
                leftUpperAngle += 1.0f;
                rightLowerAngle += 1.5f;
                rightShoulderNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                rightElbowNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                leftHipNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
                leftKneeNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
                leftShoulderNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                leftElbowNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                rightHipNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
                rightKneeNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
                if (rightUpperAngle <= 0.0f)
                {
                    rightDone = false;
                    leftDone = true;
                }
            }
            else if (leftDone)
            {
                rightUpperAngle += 1.0f;
                leftLowerAngle += 1.5f;
                leftUpperAngle -= 1.0f;
                rightLowerAngle -= 1.5f;
                rightShoulderNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                rightElbowNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                leftHipNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
                leftKneeNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
                leftShoulderNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                leftElbowNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                rightHipNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
                rightKneeNode->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-1.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
                if (rightUpperAngle >= 30.0f)
                {
                    leftDone = false;
                    rightDone = true;
                }
            }

            root->transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete shaders
    shaderProgram.Delete();

    // delete window and terminate
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}