#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/Shader.h"
#include "core/rendering/RenderPass.h"

class Pipeline
{
    public:

        /* private structures */

        struct RasterizationInfo
        {
            VkPolygonMode   polygonMode;
            VkCullModeFlags cullMode;
            VkFrontFace     frontFace;

            float           lineWidth;
        };

        struct VertexInputInfo
        {
            VkVertexInputBindingDescription                 vertexBindingDescription;
            std::vector<VkVertexInputAttributeDescription>  vertexAttributeDescriptions;
        };

        struct DescriptorSetLayoutInfo
        {
            std::vector<VkDescriptorSetLayoutBinding>       descriptors;
        };

    private:

        struct PipelineInfo
        {
            RasterizationInfo       rasterizationInfo;
            VertexInputInfo         vertexInputInfo;
            DescriptorSetLayoutInfo descriptorsInfo;
        };

        struct PipelineComponents
        {
            LogicalDevice*          pLogicalDevice  = nullptr;
            Swapchain*              pSwapchain      = nullptr;
            Shader*                 pShader         = nullptr;
            RenderPass*             pRenderPass     = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct PipelineCreateInfo
        {
            LogicalDevice*          pLogicalDevice  = nullptr;
            Swapchain*              pSwapchain      = nullptr;
            Shader*                 pShader         = nullptr;
            RenderPass*             pRenderPass     = nullptr;

            RasterizationInfo       rasterizationInfo;
            VertexInputInfo         vertexInputInfo;
            DescriptorSetLayoutInfo descriptorsInfo;
        };

        /* functions */

        Pipeline();
        ~Pipeline();

        void                      clean();
        void                      setData(const PipelineCreateInfo& createInfo);

        static int                createPipeline(Pipeline* pipeline, const PipelineCreateInfo& createInfo);

        inline VkPipeline&            getPipeline           () { return m_pipeline; }
        inline VkPipelineLayout&      getPipelineLayout     () { return m_pipelineLayout; }
        inline VkDescriptorSetLayout& getDescriptorSetLayout() {return m_descriptorSetLayout; }

        /* variables */

        PipelineInfo                pipelineInfo;

    private:

        /* functions */

        int                         createPipeline();

        /* variables */

        VkPipeline                  m_pipeline;
        VkPipelineLayout            m_pipelineLayout;
        VkDescriptorSetLayout       m_descriptorSetLayout;
};
