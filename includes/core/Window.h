#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "core/utils/logs/Logger.h"

class Window
{
    private:

        /* private structures */

        struct WindowInfo
        {
            uint32_t      width;
            uint32_t      height;
            uint32_t      resizable;

            std::string   title;

            bool          resizedStatus;
        };

    public:

        /* static variables */



        /* structure */

        struct WindowCreateInfo
        {
            uint32_t              initialWidth      = 0;
            uint32_t              initialHeight     = 0;
            uint32_t              resizable         = GLFW_FALSE;


            std::string           title             = "noName";
        };

        /* functions */

        Window();
        ~Window();

        void                      clean();
        void                      setData(const WindowCreateInfo& createInfo);

        void                      update() const;

        static int                createWindow(Window* window, const WindowCreateInfo& createInfo);
        static void               framebufferResizeCallback(GLFWwindow* window, int width, int height);

        bool                      isClosed() const;

        inline GLFWwindow*        getWindow() const { return m_window; }

        /* variables */

        WindowInfo                windowInfo;

    private:

        /* functions */

        int                       createWindow(const WindowCreateInfo& createInfo);

        /* variables */

        GLFWwindow*               m_window;
};
