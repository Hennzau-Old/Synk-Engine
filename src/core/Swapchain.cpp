#include "core/Swapchain.h"

Swapchain::Swapchain()
{

}

Swapchain::~Swapchain()
{

}

void Swapchain::clean()
{
    for (auto imageView : m_imageViews)
    {
        vkDestroyImageView(m_components.pLogicalDevice->getLogicalDevice(), imageView, nullptr);
    }

    Logger::printInfo("Swapchain::clean", "vkDestroyImageView!");

    vkDestroySwapchainKHR(m_components.pLogicalDevice->getLogicalDevice(), m_swapchain, nullptr);

    Logger::printInfo("Swapchain::clean", "vkDestroySwapchainKHR!");
}

void Swapchain::setData(const SwapchainCreateInfo& createInfo)
{
    m_components.pWindow         = createInfo.pWindow;
    m_components.pSurface        = createInfo.pSurface;
    m_components.pPhysicalDevice = createInfo.pPhysicalDevice;
    m_components.pLogicalDevice  = createInfo.pLogicalDevice;

    swapchainInfo.imageUsage = createInfo.imageUsage;
}

Swapchain::SwapchainSupportDetails Swapchain::querySwapchainSupportDetails(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
    Swapchain::SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    } else
    {
        Logger::printError("Swapchain::querySwapCainSupport", "vkGetPhysicalDeviceSurfaceFormatsKHR failed!");
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    } else
    {
        Logger::printError("Swapchain::querySwapCainSupport", "vkGetPhysicalDeviceSurfacePresentModesKHR failed!");
    }

    return details;
}

int Swapchain::createSwapChain()
{
    SwapchainSupportDetails swapchainSupport = querySwapchainSupportDetails(m_components.pPhysicalDevice->getPhysicalDevice(), m_components.pSurface->getSurface());

    m_surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
    m_presentMode   = chooseSwapPresentMode(swapchainSupport.presentModes);
    m_extent        = chooseSwapExtent(swapchainSupport.capabilities);

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;

    if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
    {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                  = m_components.pSurface->getSurface();
    createInfo.minImageCount            = imageCount;
    createInfo.imageFormat              = m_surfaceFormat.format;
    createInfo.imageColorSpace          = m_surfaceFormat.colorSpace;
    createInfo.imageExtent              = m_extent;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = swapchainInfo.imageUsage;

    auto indices  = m_components.pPhysicalDevice->getQueueFamilies();

    uint32_t queueFamilyIndices[] =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount  = 2;
        createInfo.pQueueFamilyIndices    = queueFamilyIndices;
    } else
    {
        createInfo.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount  = 0;
        createInfo.pQueueFamilyIndices    = nullptr;
    }

    createInfo.preTransform     = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode      = m_presentMode;
    createInfo.clipped          = VK_TRUE;
    createInfo.oldSwapchain     = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_components.pLogicalDevice->getLogicalDevice(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
    {
        Logger::printError("SwapChain::createSwapChain", "vkCreateSwapchainKHR failed!");

        return 1;
    }

    vkGetSwapchainImagesKHR(m_components.pLogicalDevice->getLogicalDevice(), m_swapchain, &imageCount, nullptr);
    m_images.resize(imageCount);

    vkGetSwapchainImagesKHR(m_components.pLogicalDevice->getLogicalDevice(), m_swapchain, &imageCount, m_images.data());

    m_imageFormat = m_surfaceFormat.format;

    return 0;
}

int Swapchain::createImageViews()
{
    m_imageViews.resize(m_images.size());

    VkImageViewCreateInfo createInfo  = {};
    for (size_t i = 0; i < m_images.size(); i++)
    {
        createInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                  = m_images[i];
        createInfo.viewType               = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                 = m_imageFormat;
        createInfo.components.r           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b           = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a           = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel    = 0;
        createInfo.subresourceRange.levelCount      = 1;
        createInfo.subresourceRange.baseArrayLayer  = 0;
        createInfo.subresourceRange.layerCount      = 1;

        if (vkCreateImageView(m_components.pLogicalDevice->getLogicalDevice(), &createInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS)
        {
            Logger::printError("SwapChain::createImageViews", "vkCreateImageView failed!");
        }
    }

    return 0;
}

int Swapchain::createSwapchain(Swapchain* swapChain, const SwapchainCreateInfo& createInfo)
{
    swapChain->setData(createInfo);

    return  swapChain->createSwapChain() +
            swapChain->createImageViews();
}

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    // return VK_PRESENT_MODE_FIFO_KHR;
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    } else
    {
        VkExtent2D actualExtent =
        {
            m_components.pWindow->windowInfo.width,
            m_components.pWindow->windowInfo.height
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}
