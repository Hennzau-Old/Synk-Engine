#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/Instance.h"

class Surface
{
    private:

        /* private structures */

        struct SurfaceInfo
        {

        };

        struct SurfaceComponents
        {
            Instance*           instance;
            Window*             window;
        }                       m_surfaceComponents;

    public:

        /* static variables */



        /* structure */

        struct SurfaceCreateInfo
        {
            Instance*           instance;
            Window*             window;
        };

        /* functions */

        Surface();
        ~Surface();

        void                      clean();
        void                      setData(const SurfaceCreateInfo& createInfo);

        static int                createSurface(Surface* surface, const SurfaceCreateInfo& createInfo);

        inline VkSurfaceKHR&      getSurface() { return m_surface; }

        /* variables */

    private:

        /* functions */

        int                       createSurface(const SurfaceCreateInfo& createInfo);

        /* variables */

        VkSurfaceKHR              m_surface;
};
