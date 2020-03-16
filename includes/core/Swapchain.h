#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/Surface.h"
#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

class Swapchain
{
    private:

        /* private structures */

        struct SwapchainInfo
        {
            VkImageUsageFlags               imageUsage;
        };

        struct SwapchainComponents
        {
            Window*                         pWindow         = nullptr;
            Surface*                        pSurface        = nullptr;
            PhysicalDevice*                 pPhysicalDevice = nullptr;
            LogicalDevice*                  pLogicalDevice  = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct SwapchainCreateInfo
        {
            Window*                         pWindow         = nullptr;
            Surface*                        pSurface        = nullptr;
            PhysicalDevice*                 pPhysicalDevice = nullptr;
            LogicalDevice*                  pLogicalDevice  = nullptr;

            VkImageUsageFlags               imageUsage;
        };

        struct SwapchainSupportDetails
        {
            VkSurfaceCapabilitiesKHR        capabilities;

            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR>   presentModes;
        };

        /* functions */

        Swapchain();
        ~Swapchain();

        void                      clean();
        void                      setData(const SwapchainCreateInfo& createInfo);

        static int                      createSwapchain(Swapchain* swapchain, const SwapchainCreateInfo& createInfo);
        static SwapchainSupportDetails  querySwapchainSupportDetails(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

        inline VkSwapchainKHR&          getSwapChain() { return m_swapchain; }

        inline VkSurfaceFormatKHR&      getSurfaceFormat() { return m_surfaceFormat; }
        inline VkFormat&                getImageFormat  () { return m_imageFormat; }
        inline VkPresentModeKHR&        getPresentMode  () { return m_presentMode; }
        inline VkExtent2D&              getExtent       () { return m_extent; }

        inline std::vector<VkImage>&      getImages     () { return m_images; }
        inline std::vector<VkImageView>&  getImageViews () { return m_imageViews; }

        /* variables */

        SwapchainInfo              swapchainInfo;

    private:

        /* functions */

        int                       createSwapChain();
        int                       createImageViews();

        VkSurfaceFormatKHR        chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR          chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D                chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        /* variables */

        VkSwapchainKHR            m_swapchain;

        VkSurfaceFormatKHR        m_surfaceFormat;
        VkFormat                  m_imageFormat;
        VkPresentModeKHR          m_presentMode;
        VkExtent2D                m_extent;

        std::vector<VkImage>      m_images;
        std::vector<VkImageView>  m_imageViews;
};
