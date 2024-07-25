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

int main()
{
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
    pRenderWindow->startRun();
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
