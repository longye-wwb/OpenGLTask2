#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "shader_s.h"
#include "HiveLogger.h"
#include "RenderWindow.h"
#include "RenderConfiguration.h"

int main()
{
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
    pRenderWindow->startRun();
}   
