#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader_s.h"
#include <HiveLogger.h>
#include <RenderWindow.h>
#include <RenderConfiguration.h>


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void customRender();
//Shader LightingShader("../shaders/vertPerpixelShading.glsl", "../shaders/fragPerpixelShading.glsl");
int main()
{
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
    GLFWwindow* pWindow =pRenderWindow->createWindow();
    HIVE_LOG_INFO("GLAD : {}", gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    //pRenderWindow->startRun();
    Shader LightingShader("directionLight.vs", "directionLight.fs");

    float Vertices[] = {
        //Verices              Color             Normal
         0.5f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,  0.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
        -0.5f,-0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,
        -0.5f, 0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,0.0f,1.0f,
    };
    unsigned int Indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

    while (!glfwWindowShouldClose(pWindow))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        LightingShader.use();
        LightingShader.setVec3("uViewPos", CameraPos);
        LightingShader.setFloat("uShininess", 32.0f);
        LightingShader.setFloat("uAmbientStrength", 0.1f);

        glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 ViewMat = glm::lookAt(CameraPos, CameraPos + Front, Up);
        LightingShader.setMat4("uProjection", ProjectionMat);
        LightingShader.setMat4("uView", ViewMat);

        glm::mat4 model = glm::mat4(1.0f);
        LightingShader.setMat4("uModel", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate();
    return 0;
}

//void customRender() {
//    glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
//    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    LightingShader.use();
//    LightingShader.setVec3("uViewPos", CameraPos);
//    LightingShader.setFloat("uShininess", 32.0f);
//    LightingShader.setFloat("uAmbientStrength", 0.1f);
//
//    glm::mat4 ProjectionMat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    glm::mat4 ViewMat = glm::lookAt(CameraPos, CameraPos + Front, Up);
//    LightingShader.setMat4("uProjection", ProjectionMat);
//    LightingShader.setMat4("uView", ViewMat);
//
//    glm::mat4 model = glm::mat4(1.0f);
//    LightingShader.setMat4("uModel", model);
//}
