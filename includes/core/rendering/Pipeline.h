/*=============================================
   Author: Hennzau on Sat Apr 11 10:26:17 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Shader.h"
#include "core/rendering/PipelineLayout.h"
#include "core/rendering/descriptors/DescriptorSetLayout.h"

class Pipeline
{
    public:

        /* public structures */

        struct RasterizationInfo
        {
            VkPolygonMode   polygonMode;
            VkCullModeFlags cullMode;
            VkFrontFace     frontFace;

            float           lineWidth;
        };

        struct VertexInputInfo
        {
            uint32_t                            vertexBindingDescriptionCount;
            VkVertexInputBindingDescription*    pVertexBindingDescription;

            uint32_t                            vertexAttributeDescriptionsCount;
            VkVertexInputAttributeDescription*  pVertexAttributeDescriptions;
        };

    private:

        /* private structues */

        struct PipelineInfo
        {
            RasterizationInfo       rasterizationInfo;
            VertexInputInfo         vertexInputInfo;
        };

        struct PipelineComponents
        {
            LogicalDevice*          pLogicalDevice  = nullptr;
            Swapchain*              pSwapchain      = nullptr;
            Shader*                 pShader         = nullptr;
            RenderPass*             pRenderPass     = nullptr;
            PipelineLayout*         pPipelineLayout = nullptr;

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
            PipelineLayout*         pPipelineLayout = nullptr;

            RasterizationInfo       rasterizationInfo;
            VertexInputInfo         vertexInputInfo;
        };

        /* functions */

        Pipeline();
        ~Pipeline();

        void                    clean();
        void                    setData(const PipelineCreateInfo& createInfo);

        static int              createPipeline(Pipeline* pipeline, const PipelineCreateInfo& createInfo);

        inline VkPipeline&      getPipeline() { return m_pipeline; }

        /* variables */

        PipelineInfo            pipelineInfo;

    private:

        /* functions */

        int                     createPipeline();

        /* variables */ 

        VkPipeline              m_pipeline;
};
