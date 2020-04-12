/*=============================================
   Author: Hennzau on Sat Apr 11 10:08:47 2020
  =============================================*/ 

#include "core/rendering/Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::clean()
{
    vkDestroyShaderModule(m_components.pLogicalDevice->getLogicalDevice(), m_vertexShaderModule, nullptr);
    vkDestroyShaderModule(m_components.pLogicalDevice->getLogicalDevice(), m_fragmentShaderModule, nullptr);
}

void Shader::setData(const ShaderCreateInfo& createInfo)
{
    shaderInfo.vertexShaderCode     = createInfo.vertexShaderCode;
    shaderInfo.fragmentShaderCode   = createInfo.fragmentShaderCode;

    m_components.pLogicalDevice     = createInfo.pLogicalDevice;
}

int Shader::createShader()
{
    auto createShader { [](const std::string& code, VkDevice logicalDevice) -> VkShaderModule
    {
        VkShaderModule shader;

        VkShaderModuleCreateInfo shaderCreateInfo   = {};
        shaderCreateInfo.sType                      = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize                   = code.size();
        shaderCreateInfo.pCode                      = reinterpret_cast<const uint32_t*> (code.data());

        if (vkCreateShaderModule(logicalDevice, &shaderCreateInfo, nullptr, &shader) != VK_SUCCESS)
        {
            Logger::printError("Shader::createShader[lambda=createShader]", "vkCreateShaderModule failed!");   
        }

        return shader;
    }};

    m_vertexShaderModule = createShader(shaderInfo.vertexShaderCode, m_components.pLogicalDevice->getLogicalDevice());
    m_fragmentShaderModule = createShader(shaderInfo.fragmentShaderCode, m_components.pLogicalDevice->getLogicalDevice());

    VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {};
    vertexShaderStageCreateInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageCreateInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageCreateInfo.module                          = m_vertexShaderModule;
    vertexShaderStageCreateInfo.pName                           = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo   = {};
    fragmentShaderStageCreateInfo.sType                             = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageCreateInfo.stage                             = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageCreateInfo.module                            = m_fragmentShaderModule;
    fragmentShaderStageCreateInfo.pName                             = "main";

    m_pipelineShaderStages.push_back(vertexShaderStageCreateInfo);
    m_pipelineShaderStages.push_back(fragmentShaderStageCreateInfo);
    
    return 0;
}

int Shader::createShader(Shader* shader, const ShaderCreateInfo& createInfo)
{
    shader->setData(createInfo);

    return shader->createShader();
}
