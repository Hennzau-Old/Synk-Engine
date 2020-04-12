/*=============================================
   Author: Hennzau on Sat Apr 11 10:08:47 2020
  =============================================*/ 

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
            std::string         vertexShaderCode;
            std::string         fragmentShaderCode;
        };

        struct ShaderComponents
        {
            LogicalDevice*      pLogicalDevice  = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct ShaderCreateInfo
        {
            LogicalDevice*      pLogicalDevice  = nullptr;

            std::string         vertexShaderCode;
            std::string         fragmentShaderCode;                
        };

        /* functions */

        Shader();
        ~Shader();

        void                    clean();
        void                    setData(const ShaderCreateInfo& createInfo);

        static int              createShader(Shader* shader, const ShaderCreateInfo& createInfo);

        inline VkShaderModule&  getVertexShader     () { return m_vertexShaderModule; }
        inline VkShaderModule&  getFragmentSahder   () { return m_fragmentShaderModule; }
        inline std::vector<VkPipelineShaderStageCreateInfo>&            getShaderStages     () { return m_pipelineShaderStages; }

        /* variables */

        ShaderInfo              shaderInfo;

    private:

        /* functions */

        int                     createShader();

        /* variables */

        VkShaderModule          m_vertexShaderModule;
        VkShaderModule          m_fragmentShaderModule;

        std::vector<VkPipelineShaderStageCreateInfo> m_pipelineShaderStages;
};
