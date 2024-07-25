#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderWindow.h"

int main()
{
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
    pRenderWindow->startRun();
}   
