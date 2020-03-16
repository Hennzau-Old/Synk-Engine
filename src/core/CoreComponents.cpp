#include "core/CoreComponents.h"

CoreComponents::CoreComponents()
{

}

CoreComponents::~CoreComponents()
{

}

void CoreComponents::clean()
{
    Logger::init("___CLEAN_____CORE_____");

    m_swapchain.clean();
    m_logicalDevice.clean();
    m_surface.clean();
    m_instance.clean();
    m_window.clean();

    Logger::exit("___CLEAN_____CORE_____");
}

void CoreComponents::setData(const CoreComponentsCreateInfo& createInfo)
{
    coreComponentsInfo.windowCreateInfo = createInfo.windowCreateInfo;
    coreComponentsInfo.imageUsage       = createInfo.imageUsage;
}

int CoreComponents::createWindow()
{
    if (Window::createWindow(&m_window, coreComponentsInfo.windowCreateInfo) != 0)
    {
        Logger::printError("coreComponents::createWindow", "createWindow failed!");
    } else
    {
        Logger::printSuccess("coreComponents::createWindow", "createWindow succeed!");
    }

    return 0;
}

int CoreComponents::createInstance()
{
    Instance::InstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.appName                      = m_window.windowInfo.title;
    instanceCreateInfo.appVersion                   = VK_MAKE_VERSION(0, 0, 1);
    instanceCreateInfo.engineName                   = "No Engine";
    instanceCreateInfo.engineVersion                = VK_MAKE_VERSION(0, 0, 1);

    if (Instance::createInstance(&m_instance, instanceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createInstance", "createInstance failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createInstance", "createInstance succeed!");
    }

    return 0;
}

int CoreComponents::createSurface()
{
    Surface::SurfaceCreateInfo surfaceCreateInfo  = {};
    surfaceCreateInfo.pInstance                   = &m_instance;
    surfaceCreateInfo.pWindow                     = &m_window;

    if (Surface::createSurface(&m_surface, surfaceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createSurface", "createSurface failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createSurface", "createSurface succeed!");
    }

    return 0;
}

int CoreComponents::createPhysicalDevice()
{
    PhysicalDevice::PhysicalDeviceCreateInfo physicalDeviceCreateInfo = {};
    physicalDeviceCreateInfo.pInstance                                = &m_instance;
    physicalDeviceCreateInfo.pSurface                                 = &m_surface;

    if (PhysicalDevice::createPhysicalDevice(&m_physicalDevice, physicalDeviceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createPhysicalDevice", "createPhysicalDevice failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createPhysicalDevice", "createPhysicalDevice succeed!");
    }

    return 0;
}

int CoreComponents::createLogicalDevice()
{
    LogicalDevice::LogicalDeviceCreateInfo logicalDeviceCreateInfo  = {};
    logicalDeviceCreateInfo.pInstance                               = &m_instance;
    logicalDeviceCreateInfo.pPhysicalDevice                         = &m_physicalDevice;

    if (LogicalDevice::createLogicalDevice(&m_logicalDevice, logicalDeviceCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createLogicalDevice", "createLogicalDevice failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createLogicalDevice", "createLogicalDevice succeed!");
    }

    return 0;
}

int CoreComponents::createSwapchain()
{
    Swapchain::SwapchainCreateInfo swapchainCreateInfo  = {};
    swapchainCreateInfo.pWindow                         = &m_window;
    swapchainCreateInfo.pSurface                        = &m_surface;
    swapchainCreateInfo.pPhysicalDevice                 = &m_physicalDevice;
    swapchainCreateInfo.pLogicalDevice                  = &m_logicalDevice;
    swapchainCreateInfo.imageUsage                     = coreComponentsInfo.imageUsage;

    if (Swapchain::createSwapchain(&m_swapchain, swapchainCreateInfo) != 0)
    {
        Logger::printError("CoreComponents::createSwapchain", "createSwapchain failed!");
    } else
    {
        Logger::printSuccess("CoreComponents::createSwapchain", "createSwapchain succeed!");
    }

    return 0;
}

int CoreComponents::endCreation()
{
    Logger::exit("___CORE__Components___");

    return 0;
}

int CoreComponents::createCoreComponents(CoreComponents* coreComponents, const CoreComponentsCreateInfo& createInfo)
{
    coreComponents->setData(createInfo);

    Logger::init("___CORE__Components___");

    return  coreComponents->createWindow() +
            coreComponents->createInstance() +
            coreComponents->createSurface() +
            coreComponents->createPhysicalDevice() +
            coreComponents->createLogicalDevice() +
            coreComponents->createSwapchain() +
            coreComponents->endCreation();
}
