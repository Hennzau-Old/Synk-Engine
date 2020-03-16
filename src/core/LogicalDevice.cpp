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
    m_components.pInstance       = createInfo.pInstance;
    m_components.pPhysicalDevice = createInfo.pPhysicalDevice;
}

void LogicalDevice::wait() const
{
    vkDeviceWaitIdle(m_logicalDevice);
}

int LogicalDevice::createLogicalDevice()
{
    auto indices = m_components.pPhysicalDevice->getQueueFamilies();

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
    logicalDeviceCreateInfo.enabledExtensionCount   = static_cast<uint32_t>(m_components.pPhysicalDevice->deviceExtensions.size());
    logicalDeviceCreateInfo.ppEnabledExtensionNames = m_components.pPhysicalDevice->deviceExtensions.data();

    if (Instance::enabledValidationLayers)
    {
        logicalDeviceCreateInfo.enabledLayerCount   = static_cast<uint32_t>(m_components.pInstance->validationLayers.size());
        logicalDeviceCreateInfo.ppEnabledLayerNames = m_components.pInstance->validationLayers.data();
    } else
    {
        logicalDeviceCreateInfo.enabledLayerCount   = 0;
    }

    if (vkCreateDevice(m_components.pPhysicalDevice->getPhysicalDevice(), &logicalDeviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
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

    return logicalDevice->createLogicalDevice();
}
