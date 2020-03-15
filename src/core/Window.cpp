#include "core/Window.h"

Window::Window()
{

}

Window::~Window()
{

}

void Window::clean()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();

    Logger::printInfo("Window::clean", "glfwDestroyWindow!");
}

void Window::setData(const WindowCreateInfo& createInfo)
{
    windowInfo.width      = createInfo.initialWidth;
    windowInfo.height     = createInfo.initialHeight;
    windowInfo.resizable  = createInfo.resizable;
    windowInfo.title      = createInfo.title;
}

void Window::update() const
{
    glfwPollEvents();
}

int Window::createWindow(const WindowCreateInfo& createInfo)
{
    if (!glfwInit())
    {
        Logger::printError("Window::createWindow", "glfwInit failed!");

        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, windowInfo.resizable);

    m_window = glfwCreateWindow(createInfo.initialWidth, createInfo.initialHeight, createInfo.title.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        Logger::printError("Window::createWindow", "glfwCreateWindow failed!");

        return 1;
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);

    return 0;
}

int Window::createWindow(Window* window, const WindowCreateInfo& createInfo)
{
    window->setData(createInfo);

    return window->createWindow(createInfo);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto display = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    display->windowInfo.resizedStatus = true;

    display->windowInfo.width  = width;
    display->windowInfo.height = height;
}

bool Window::isClosed() const
{
    return glfwWindowShouldClose(m_window);
}
