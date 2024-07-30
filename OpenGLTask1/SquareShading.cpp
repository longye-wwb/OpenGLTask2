#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderWindow.h"

int main()
{
    std::string XMLPath = "../XML/config.xml";
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
 
    pRenderWindow->startRun([](std::shared_ptr<openGLTask::CDirectionalLight> vDirLight) -> glm::vec3 
        {
         return vDirLight->rotateDirectionalLight(60.0f);
        });
    
}   
