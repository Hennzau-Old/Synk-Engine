#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <map>
#include <set>

#include "core/utils/logs/Logger.h"

#include "core/Instance.h"
#include "core/Surface.h"

class PhysicalDevice
{
    public:

        /* static variables */

        const std::vector<const char*> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        /* structure */

        struct PhysicalDeviceCreateInfo
        {
            Instance*               pInstance = nullptr;
            Surface*                pSurface   = nullptr;
        };

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;
            std::optional<uint32_t> transferFamily;

            bool isComplete()
            {
                return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
            }
        };

    private:

          /* private structures */

          struct PhysicalDeviceInfo
          {
              QueueFamilyIndices      queueFamily;
          };

          struct PhysicalDeviceComponents
          {
              Instance*               pInstance = nullptr;
              Surface*                pSurface  = nullptr;

          }   m_components;

    public:

        /* functions */

        PhysicalDevice();
        ~PhysicalDevice();

        void                        clean();
        void                        setData(const PhysicalDeviceCreateInfo& createInfo);

        static int                  createPhysicalDevice(PhysicalDevice* physicalDevice, const PhysicalDeviceCreateInfo& createInfo);

        inline VkPhysicalDevice&    getPhysicalDevice() { return m_physicalDevice; }
        inline QueueFamilyIndices&  getQueueFamilies() { return m_queueFamily; }

        /* variables */

        PhysicalDeviceInfo          physicalDeviceInfo;

    private:

        /* functions */

        int                         createPhysicalDevice();
        uint32_t                    ratePhysicalDeviceSuitability(const VkPhysicalDevice& physicalDevice) const;
        QueueFamilyIndices          findQueueFamilies(const VkPhysicalDevice& physicalDevice) const;
        bool                        checkDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice) const;

        /* variables */

        QueueFamilyIndices          m_queueFamily;

        VkPhysicalDevice            m_physicalDevice;
};
