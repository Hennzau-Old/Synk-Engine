#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Instance.h"
#include "core/PhysicalDevice.h"

class LogicalDevice
{
    private:

        /* private structures */

        struct LogicalDeviceInfo
        {

        };

        struct LogicalDeviceComponents
        {
            Instance*             instance        = nullptr;
            PhysicalDevice*       physicalDevice  = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct LogicalDeviceCreateInfo
        {
            Instance*             instance        = nullptr;
            PhysicalDevice*       physicalDevice  = nullptr;
        };

        /* functions */

        LogicalDevice();
        ~LogicalDevice();

        void                      clean();
        void                      setData(const LogicalDeviceCreateInfo& createInfo);

        void                      wait() const;

        static int                createLogicalDevice(LogicalDevice* logicalDevice, const LogicalDeviceCreateInfo& createInfo);

        inline VkDevice&          getLogicalDevice() { return m_logicalDevice;  }
        inline VkQueue&           getGraphicsQueue() { return m_graphicsQueue;  }
        inline VkQueue&           getPresentQueue () { return m_presentQueue;   }
        inline VkQueue&           getTransferQueue() { return m_transferQueue;  }

        /* variables */

        LogicalDeviceInfo         logicalDeviceInfo;

    private:

        /* functions */

        int                       createLogicalDevice(const LogicalDeviceCreateInfo& createInfo);

        /* variables */

        VkDevice                  m_logicalDevice;

        VkQueue                   m_graphicsQueue;
        VkQueue                   m_presentQueue;
        VkQueue                   m_transferQueue;

};
