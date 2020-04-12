#include "core/Instance.h"

Instance::Instance()
{

}

Instance::~Instance()
{

}

void Instance::clean()
{
    vkDestroyInstance(m_instance, nullptr);

    Logger::printInfo("Instance::clean", "vkDestroyInstance!");
}

void Instance::setData(const InstanceCreateInfo& createInfo)
{
    instanceInfo.appName        = createInfo.appName;
    instanceInfo.engineName     = createInfo.engineName;
    instanceInfo.appVersion     = createInfo.appVersion;
    instanceInfo.engineVersion  = createInfo.engineVersion;
}

int Instance::createInstance()
{
    if (enabledValidationLayers && !checkValidationLayerSupport())
    {
        Logger::printError("Instance::createInstance", "validatioinLayers are enabled but not supported!");
    }

    VkApplicationInfo appInfo   = {};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = instanceInfo.appName.c_str();
    appInfo.applicationVersion  = instanceInfo.appVersion;
    appInfo.pEngineName         = instanceInfo.engineName.c_str();
    appInfo.engineVersion       = instanceInfo.engineVersion;
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    auto        extensionCount  { (uint32_t) 0 };
    const auto  extensions      { getExtensions(extensionCount) };

    VkInstanceCreateInfo instanceCreateInfo     = {};
    instanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo         = &appInfo;
    instanceCreateInfo.enabledExtensionCount    = extensionCount;
    instanceCreateInfo.ppEnabledExtensionNames  = extensions;

    if (enabledValidationLayers)
    {
        instanceCreateInfo.enabledLayerCount    = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames  = validationLayers.data();
    } else
    {
        instanceCreateInfo.enabledLayerCount    = 0;
    }

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        Logger::printError("Instance::createInstance", "vkCreateInstance failed!");

        return 1;
    }

    return 0;
}

const char** Instance::getExtensions(uint32_t& extensionCount) const
{
    return glfwGetRequiredInstanceExtensions(&extensionCount);
}

bool Instance::checkValidationLayerSupport() const
{
    auto layerCount { (uint32_t) 0};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto layerName : validationLayers)
    {
        auto layerFound { false };

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

int Instance::createInstance(Instance* instance, const InstanceCreateInfo& createInfo)
{
    instance->setData(createInfo);

    return instance->createInstance();
}
