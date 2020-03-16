#include <iostream>

#include "core/utils/File.h"

#include "core/CoreComponents.h"
#include "core/rendering/Scene.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

CoreComponents  coreComponents;
Scene           scene;

void init()
{
    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.initialWidth             = 1280;
    windowCreateInfo.initialHeight            = 720;
    windowCreateInfo.resizable                = GLFW_FALSE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    CoreComponents::CoreComponentsCreateInfo coreComponentsCreateInfo = {};
    coreComponentsCreateInfo.windowCreateInfo                         = windowCreateInfo;
    coreComponentsCreateInfo.imageUsage                               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (CoreComponents::createCoreComponents(&coreComponents, coreComponentsCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCoreComponents failed!");
    }

    RenderPass::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.format                           = coreComponents.getSwapchain()->getImageFormat();
    renderPassCreateInfo.samples                          = VK_SAMPLE_COUNT_1_BIT;
    renderPassCreateInfo.loadOp                           = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassCreateInfo.storeOp                          = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassCreateInfo.stencilLoadOp                    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassCreateInfo.stencilStoreOp                   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassCreateInfo.initialLayout                    = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassCreateInfo.finalLayout                      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    Shader::ShaderCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.vertexShaderCode         = File::readFile("res/shaders/spir-v/main.vert");
    shaderCreateInfo.fragmentShaderCode       = File::readFile("res/shaders/spir-v/main.frag");

    Scene::SceneCreateInfo sceneCreateInfo  = {};
    sceneCreateInfo.pCoreComponents         = &coreComponents;
    sceneCreateInfo.renderPassCreateInfo    = renderPassCreateInfo;
    sceneCreateInfo.shaderCreateInfo        = shaderCreateInfo;
    sceneCreateInfo.name                    = "main";

    if (Scene::createScene(&scene, sceneCreateInfo) != 0)
    {
        Logger::printError("main::init", "createScene failed!");
    }
}

void update()
{
    coreComponents.getWindow()->update();
}

void render()
{

}

void clean()
{
    scene.clean();
    coreComponents.clean();
}

int main()
{
    init();

    Logger::init("_____GAME__ENGINE_____");

    double  tickTime    = 1.0 / TICK_CAP;
    double  renderTime  = 1.0 / FRAME_CAP;

    double  lastTickTime    = glfwGetTime();
    double  timer = lastTickTime;
    double  lastRenderTime  = glfwGetTime();
    double  deltaTime       = 0,
            nowTime         = 0;

    int     frames          = 0,
            ticks           = 0;

    while(!coreComponents.getWindow()->isClosed())
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTickTime) / tickTime;
        lastTickTime = nowTime;

        while(deltaTime >= 1.0)
        {
            ticks++;
            update();
            deltaTime--;
        }

        if(nowTime - lastRenderTime >= renderTime)
        {
            lastRenderTime = nowTime;
            render();
            frames++;
        }

        if(glfwGetTime() - timer > 1.0)
        {
            timer++;

            Logger::printText("main", "Synk - Vulkan Engine : FPS = " + std::to_string(frames) + " | TPS = " + std::to_string(ticks));

            frames = 0;
            ticks = 0;
        }
    }

    Logger::exit("_____GAME__ENGINE_____");

    clean();

    return 0;
}