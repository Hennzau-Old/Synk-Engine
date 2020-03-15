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

}

int Instance::createInstance(const InstanceCreateInfo& createInfo)
{
    if (enabledValidationLayers && !checkValidationLayerSupport())
    {
        Logger::printError("Instance::createInstance", "validatioinLayers are enabled but not supported!");
    }

    VkApplicationInfo appInfo   = {};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = createInfo.appName.c_str();
    appInfo.applicationVersion  = createInfo.appVersion;
    appInfo.pEngineName         = createInfo.engineName.c_str();
    appInfo.engineVersion       = createInfo.engineVersion;
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    uint32_t extensionCount = 0;
    const char** extensions = getExtensions(extensionCount);

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

const char** Instance::getExtensions(uint32_t& extensionCount)
{
    return glfwGetRequiredInstanceExtensions(&extensionCount);
}

bool Instance::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

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

    return instance->createInstance(createInfo);
}
