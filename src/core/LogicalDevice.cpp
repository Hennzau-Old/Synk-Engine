#include "core/LogicalDevice.h"

LogicalDevice::LogicalDevice()
{

}

LogicalDevice::~LogicalDevice()
{

}

void LogicalDevice::clean()
{
    vkDestroyDevice(m_logicalDevice, nullptr);

    Logger::printInfo("LogicalDevice::clean", "vkDestroyDevice!");
}

void LogicalDevice::setData(const LogicalDeviceCreateInfo& createInfo)
{
    m_components.instance       = createInfo.instance;
    m_components.physicalDevice = createInfo.physicalDevice;
}

void LogicalDevice::wait() const
{
    vkDeviceWaitIdle(m_logicalDevice);
}

int LogicalDevice::createLogicalDevice(const LogicalDeviceCreateInfo& createInfo)
{
    auto indices = createInfo.physicalDevice->getQueueFamilies();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies =
    {
        indices.graphicsFamily.value(),
        indices.presentFamily.value(),
        indices.transferFamily.value()
    };

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex        = queueFamily;
        queueCreateInfo.queueCount              = 1;
        queueCreateInfo.pQueuePriorities        = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo logicalDeviceCreateInfo      = {};
    logicalDeviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logicalDeviceCreateInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
    logicalDeviceCreateInfo.pQueueCreateInfos       = queueCreateInfos.data();
    logicalDeviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
    logicalDeviceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>(createInfo.physicalDevice->deviceExtensions.size());
    logicalDeviceCreateInfo.ppEnabledExtensionNames = createInfo.physicalDevice->deviceExtensions.data();

    if (Instance::enabledValidationLayers)
    {
        logicalDeviceCreateInfo.enabledLayerCount   = static_cast<uint32_t>(createInfo.instance->validationLayers.size());
        logicalDeviceCreateInfo.ppEnabledLayerNames = createInfo.instance->validationLayers.data();
    } else
    {
        logicalDeviceCreateInfo.enabledLayerCount   = 0;
    }

    if (vkCreateDevice(createInfo.physicalDevice->getPhysicalDevice(), &logicalDeviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        Logger::printError("LogicalDevice::createLogicalDevice", "vkCreateDevice failed!");

        return 1;
    }

    vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.presentFamily.value(), 0, &m_presentQueue);
    vkGetDeviceQueue(m_logicalDevice, indices.transferFamily.value(), 0, &m_transferQueue);

    return 0;
}

int LogicalDevice::createLogicalDevice(LogicalDevice* logicalDevice, const LogicalDeviceCreateInfo& createInfo)
{
    logicalDevice->setData(createInfo);

    return logicalDevice->createLogicalDevice(createInfo);
}
