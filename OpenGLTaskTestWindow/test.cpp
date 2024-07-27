#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderWindow.h"

TEST(TestOnlyOneWindowOnceDraw, DllOpenGLTask) {
    std::shared_ptr<openGLTask::CRenderWindow> pRenderWindow = std::make_shared<openGLTask::CRenderWindow>();
    pRenderWindow->startRun();
    pRenderWindow->startRun();
    pRenderWindow->startRun();
    pRenderWindow->startRun();
    pRenderWindow->startRun();
}