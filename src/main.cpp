#include <iostream>

#include "core/Window.h"
#include "core/Instance.h"
#include "core/Surface.h"
#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

Window              window;
Instance            instance;
Surface             surface;
PhysicalDevice      physicalDevice;
LogicalDevice       logicalDevice;

void init()
{
    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.initialWidth             = 1280;
    windowCreateInfo.initialHeight            = 720;
    windowCreateInfo.resizable                = GLFW_FALSE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    if(Window::createWindow(&window, windowCreateInfo) != 0)
    {
        Logger::printError("main::init", "createWindow failed!");
    } else
    {
        Logger::printSuccess("main::init", "createWindow succeed!");
    }

    Instance::InstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.appName                      = "Synk Vulkan Engine";
    instanceCreateInfo.engineName                   = "No Engine";
    instanceCreateInfo.appVersion                   = VK_MAKE_VERSION(0, 0, 1);
    instanceCreateInfo.engineVersion                = VK_MAKE_VERSION(0, 0, 1);

    if (Instance::createInstance(&instance, instanceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createInstance failed!");
    } else
    {
        Logger::printSuccess("main::init", "createInstance succeed!");
    }

    Surface::SurfaceCreateInfo surfaceCreateInfo  = {};
    surfaceCreateInfo.instance                    = &instance;
    surfaceCreateInfo.window                      = &window;

    if (Surface::createSurface(&surface, surfaceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createSurface failed!");
    } else
    {
        Logger::printSuccess("main::init", "createSurface succeed!");
    }

    PhysicalDevice::PhysicalDeviceCreateInfo physicalDeviceCreateInfo = {};
    physicalDeviceCreateInfo.instance                                 = &instance;
    physicalDeviceCreateInfo.surface                                  = &surface;

    if (PhysicalDevice::createPhysicalDevice(&physicalDevice, physicalDeviceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPhysicalDevice failed!");
    } else
    {
        Logger::printSuccess("main::init", "createPhysicalDevice succeed!");
    }

    LogicalDevice::LogicalDeviceCreateInfo logicalDeviceCreateInfo  = {};
    logicalDeviceCreateInfo.instance                                = &instance;
    logicalDeviceCreateInfo.physicalDevice                          = &physicalDevice;

    if (LogicalDevice::createLogicalDevice(&logicalDevice, logicalDeviceCreateInfo) != 0)
    {
        Logger::printError("main::init", "createLogicalDevice failed!");
    } else
    {
        Logger::printSuccess("main::init", "createLogicalDevice succeed!");
    }
}

void update()
{
    window.update();
}

void render()
{

}

void clean()
{
    logicalDevice.clean();
    physicalDevice.clean();
    surface.clean();
    instance.clean();
    window.clean();
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

    while(!window.isClosed())
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
