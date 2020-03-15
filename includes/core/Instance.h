#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <cstring>

#include "core/utils/logs/Logger.h"

class Instance
{
    private:

        /* private structures */

        struct InstanceInfo
        {

        };

    public:

        /* static variables */

        const std::vector<const char*> validationLayers
        {
            "VK_LAYER_KHRONOS_validation"
        };

        static const bool enabledValidationLayers = true;

        /* structure */

        struct InstanceCreateInfo
        {
            std::string           appName         = "noName";
            std::string           engineName      = "No Engine";

            uint32_t              appVersion      = VK_MAKE_VERSION(0, 0, 0);
            uint32_t              engineVersion   = VK_MAKE_VERSION(0, 0, 0);
        };

        /* functions */

        Instance();
        ~Instance();

        void                      clean();
        void                      setData(const InstanceCreateInfo& createInfo);

        static int                createInstance(Instance* instance, const InstanceCreateInfo& createInfo);

        inline VkInstance&        getInstance() { return m_instance; }

        /* variables */

        InstanceInfo              instanceInfo;

    private:

        /* functions */

        int                       createInstance(const InstanceCreateInfo& createInfo);
        const char**              getExtensions(uint32_t& extensionCount);

        bool                      checkValidationLayerSupport();

        /* variables */

        VkInstance                m_instance;
};
