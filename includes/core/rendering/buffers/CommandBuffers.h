#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/rendering/RenderPass.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/CommandPool.h"

class CommandBuffers
{
    private:

        /* private structures */

        struct CommandBuffersInfo
        {
            std::vector<Framebuffer>*         pFramebuffers   = nullptr;
        };

        struct CommandBuffersComponents
        {
            LogicalDevice*                    pLogicalDevice  = nullptr;
            Swapchain*                        pSwapchain      = nullptr;
            CommandPool*                      pCommandPool    = nullptr;

        }   m_components;

    public:

        /* static variables */



        /* structure */

        struct CommandBuffersCreateInfo
        {
            LogicalDevice*                    pLogicalDevice  = nullptr;
            Swapchain*                        pSwapchain      = nullptr;
            CommandPool*                      pCommandPool    = nullptr;

            std::vector<Framebuffer>*         pFramebuffers   = nullptr;
        };

        /* functions */

        CommandBuffers();
        ~CommandBuffers();

        void                                  clean();
        void                                  setData(const CommandBuffersCreateInfo& createInfo);

        void                                  beginCommandBuffers(const uint32_t& index);
        void                                  beginRenderPass(const uint32_t& index, RenderPass* renderPass);

        void                                  bindPipeline(const uint32_t& index, Pipeline* pipeline);
        void                                  bindVertexBuffer(const uint32_t& index, std::vector<VkBuffer>& vertexBuffers);
        void                                  bindIndexBuffer(const uint32_t& index, VkBuffer& indexBuffer);

        void                                  draw(const uint32_t& index, const size_t& drawCount);

        void                                  endCommandBuffers(const uint32_t& index);
        void                                  endRenderPass(const uint32_t& index);

        static int                            createCommandBuffers(CommandBuffers* commandBuffers, const CommandBuffersCreateInfo& createInfo);

        inline std::vector<VkCommandBuffer>&  getCommandBuffers() { return m_commandBuffers; }

        /* variables */

        CommandBuffersInfo                    commandBuffersInfo;

    private:

        /* functions */

        int                                   createCommandBuffers();

        /* variables */

        std::vector<VkCommandBuffer>          m_commandBuffers;
};
