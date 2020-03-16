#include "core/rendering/Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::clean()
{
    vkDestroyShaderModule(m_components.pLogicalDevice->getLogicalDevice(), m_vertexShader, nullptr);
    vkDestroyShaderModule(m_components.pLogicalDevice->getLogicalDevice(), m_fragmentShader, nullptr);

    Logger::printInfo("Shader::clean", "vkDestroyShaderModule x2!");
}

void Shader::setData(const ShaderCreateInfo& createInfo)
{
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;

    shaderInfo.vertexShaderCode   = createInfo.vertexShaderCode;
    shaderInfo.fragmentShaderCode = createInfo.fragmentShaderCode;
}

int Shader::createShaders()
{
    std::vector<char> vertexShaderCode  (shaderInfo.vertexShaderCode.begin(), shaderInfo.vertexShaderCode.end());
    std::vector<char> fragmentShaderCode(shaderInfo.fragmentShaderCode.begin(), shaderInfo.fragmentShaderCode.end());

    m_vertexShader    = createShaderModule(vertexShaderCode);
    m_fragmentShader  = createShaderModule(fragmentShaderCode);

    if (m_vertexShader == VK_NULL_HANDLE)
    {
        Logger::printError("Shader::createShader", "createVertexShaderModule failed!");

        return 1;
    }

    if (m_fragmentShader == VK_NULL_HANDLE)
    {
        Logger::printError("Shader::createShader", "createFragmentShaderModule failed!");

        return 1;
    }

    return 0;
}

int Shader::createShaderStages()
{
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = {};
    vertexShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module                          = m_vertexShader;
    vertexShaderStageInfo.pName                           = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
    fragmentShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module                          = m_fragmentShader;
    fragmentShaderStageInfo.pName                           = "main";

    m_shaderStages.push_back(vertexShaderStageInfo);
    m_shaderStages.push_back(fragmentShaderStageInfo);

    return 0;
}

VkShaderModule Shader::createShaderModule(const std::vector<char>& code)
{
    VkShaderModule shaderModule;

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize                 = code.size();
    createInfo.pCode                    = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(m_components.pLogicalDevice->getLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        Logger::printError("Shader::createShaderModule", "vkCreateShaderModule failed!");

        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

int Shader::createShader(Shader* shader, const ShaderCreateInfo& createInfo)
{
    shader->setData(createInfo);

    return  shader->createShaders() +
            shader->createShaderStages();
}
