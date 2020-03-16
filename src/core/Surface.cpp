#include "core/Surface.h"

Surface::Surface()
{

}

Surface::~Surface()
{

}

void Surface::clean()
{
    vkDestroySurfaceKHR(m_components.pInstance->getInstance(), m_surface, nullptr);

    Logger::printInfo("Surface::clean", "vkDestroySurfaceKHR!");
}

void Surface::setData(const SurfaceCreateInfo& createInfo)
{
    m_components.pInstance = createInfo.pInstance;
    m_components.pWindow   = createInfo.pWindow;
}

int Surface::createSurface()
{
    if (glfwCreateWindowSurface(m_components.pInstance->getInstance(), m_components.pWindow->getWindow(), nullptr, &m_surface) != VK_SUCCESS)
    {
        Logger::printError("Surface::createSurface", "glfwCreateWindowSurface failed!");

        return 1;
    }

    return 0;
}

int Surface::createSurface(Surface* surface, const SurfaceCreateInfo& createInfo)
{
    surface->setData(createInfo);

    return surface->createSurface();
}
