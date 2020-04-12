/*=============================================
   Author: Hennzau on Sat Apr 11 12:50:49 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/Framebuffer.h"

class CommandBuffers
{
    private:

        /* private structures */

        struct CommandBuffersInfo
        {
            std::vector<Framebuffer>*           pFramebuffers   = nullptr;
        };

        struct CommandBuffersComponents
        {
            LogicalDevice*                      pLogicalDevice  = nullptr;
            Swapchain*                          pSwapchain      = nullptr;
            CommandPool*                        pCommandPool    = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct CommandBuffersCreateInfo
        {
            LogicalDevice*                      pLogicalDevice  = nullptr;
            Swapchain*                          pSwapchain      = nullptr;
            CommandPool*                        pCommandPool    = nullptr;

            std::vector<Framebuffer>*           pFramebuffers   = nullptr;
        };

        /* functions */

        CommandBuffers();
        ~CommandBuffers();

        void                                    clean();
        void                                    setData(const CommandBuffersCreateInfo& createInfo);

        void                                    beginCommandBuffers(const uint32_t& index);
        void                                    beginRenderPass(const uint32_t& index, const VkRenderPass& renderPass);

        void                                    bindPipeline(const uint32_t& index, const VkPipeline& pipeline);
        void                                    bindVertexBuffer(const uint32_t& index, const std::vector<VkBuffer>& vertexBuffers);
        void                                    bindIndexBuffer(const uint32_t& index, const VkBuffer& indexBuffer);

        void                                    draw(const uint32_t& index, const size_t& drawCount);

        void                                    endCommandBuffers(const uint32_t& index);
        void                                    endRenderPass(const uint32_t& index);

        static int                              createCommandBuffers(CommandBuffers* commandbuffers, const CommandBuffersCreateInfo& createInfo);

        inline std::vector<VkCommandBuffer>&    getCommandBuffers() { return m_commandBuffers; }

        /* variables */

        CommandBuffersInfo                      commandbuffersInfo;

    private:

        /* functions */

        int                                     createCommandBuffers();

        /* variables */

        std::vector<VkCommandBuffer>            m_commandBuffers;
};
