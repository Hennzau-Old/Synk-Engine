#include "core/PhysicalDevice.h"

PhysicalDevice::PhysicalDevice()
{

}

PhysicalDevice::~PhysicalDevice()
{

}

void PhysicalDevice::clean()
{

}

void PhysicalDevice::setData(const PhysicalDeviceCreateInfo& createInfo)
{
    m_components.instance = createInfo.instance;
    m_components.surface  = createInfo.surface;
}

uint32_t PhysicalDevice::ratePhysicalDeviceSuitability(const VkPhysicalDevice& physicalDevice)
{
    VkPhysicalDeviceProperties  deviceProperties;
    VkPhysicalDeviceFeatures    deviceFeatures;
    QueueFamilyIndices          indices;

    bool                        extensionsSupported   = false;
    bool                        swapChainAdequate     = false;

    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    indices               = findQueueFamilies(physicalDevice);

    extensionsSupported   = checkDeviceExtensionSupport(physicalDevice);

    // if (extensionsSupported)
    // {
    //     SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::querySwapCainSupport(physicalDevice, m_info.surface->getSurface());
    //
    //     swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    // }

    uint32_t score = 0;

    /* really good */

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)  score += 1000;

    /* Not important */

        score += deviceProperties.limits.maxImageDimension2D;

    /* Necessary */

        if (!deviceFeatures.geometryShader)   score = 0;
        if (!indices.isComplete())            score = 0;
        if (!extensionsSupported)             score = 0;
        // if (!swapChainAdequate)               score = 0;

    return score;
}

PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies(const VkPhysicalDevice& physicalDevice)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    unsigned int i = 0;

    for (const auto& queueFamily : queueFamilies)
    {
        if (indices.isComplete())
        {
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            indices.transferFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_components.surface->getSurface(), &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        i++;
    }

    if (!indices.transferFamily.has_value())
    {
        indices.transferFamily = indices.graphicsFamily;
    }

    return indices;
}

bool PhysicalDevice::checkDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

int PhysicalDevice::createPhysicalDevice(const PhysicalDeviceCreateInfo& createInfo)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_components.instance->getInstance(), &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        Logger::printError("PhysicalDevice::createPhysicalDevice", "No graphics card support Vulkan!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_components.instance->getInstance(), &deviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> candidates;

    for (const auto& device : devices)
    {
        uint32_t score = ratePhysicalDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
    {
        m_physicalDevice  = candidates.rbegin()->second;
        m_queueFamily     = findQueueFamilies(m_physicalDevice);
    } else
    {
        Logger::printError("PhysicalDevice::createPhysicalDevice", "No graphics cards can execute this program!");

        return 1;
    }

    return 0;
}

int PhysicalDevice::createPhysicalDevice(PhysicalDevice* physicalDevice, const PhysicalDeviceCreateInfo& createInfo)
{
    physicalDevice->setData(createInfo);

    return physicalDevice->createPhysicalDevice(createInfo);
}
