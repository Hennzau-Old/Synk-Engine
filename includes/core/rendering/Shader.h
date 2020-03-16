#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class Shader
{
    private:

        /* private structures */

        struct ShaderInfo
        {
            std::string           vertexShaderCode;
            std::string           fragmentShaderCode;
        };

        struct ShaderComponents
        {
            LogicalDevice*        pLogicalDevice  = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct ShaderCreateInfo
        {
            LogicalDevice*        pLogicalDevice  = nullptr;

            std::string           vertexShaderCode;
            std::string           fragmentShaderCode;
        };

        /* functions */

        Shader();
        ~Shader();

        void                      clean();
        void                      setData(const ShaderCreateInfo& createInfo);

        static int                createShader(Shader* shader, const ShaderCreateInfo& createInfo);

        inline std::vector<VkPipelineShaderStageCreateInfo>& getShaderStages() { return m_shaderStages; }

        /* variables */

        ShaderInfo                shaderInfo;

    private:

        /* functions */

        int                       createShaders();
        int                       createShaderStages();

        VkShaderModule            createShaderModule(const std::vector<char>& code);

        /* variables */

        VkShaderModule            m_vertexShader;
        VkShaderModule            m_fragmentShader;

        std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};
