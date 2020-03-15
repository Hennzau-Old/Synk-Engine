#include "core/Surface.h"

Surface::Surface()
{

}

Surface::~Surface()
{

}

void Surface::clean()
{
    vkDestroySurfaceKHR(m_surfaceComponents.instance->getInstance(), m_surface, nullptr);

    Logger::printInfo("Surface::clean", "vkDestroySurfaceKHR!");
}

void Surface::setData(const SurfaceCreateInfo& createInfo)
{
    m_surfaceComponents.instance = createInfo.instance;
    m_surfaceComponents.window   = createInfo.window;
}

int Surface::createSurface(const SurfaceCreateInfo& createInfo)
{
    if (glfwCreateWindowSurface(createInfo.instance->getInstance(), createInfo.window->getWindow(), nullptr, &m_surface) != VK_SUCCESS)
    {
        Logger::printError("Surface::createSurface", "glfwCreateWindowSurface failed!");

        return 1;
    }

    return 0;
}

int Surface::createSurface(Surface* surface, const SurfaceCreateInfo& createInfo)
{
    surface->setData(createInfo);

    return surface->createSurface(createInfo);
}
