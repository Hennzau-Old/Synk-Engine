#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/Pipeline.h"

class Commandbuffer
{
    private:

        /* private structures */

        struct CommandbufferInfo
        {

        };

        struct CommandbufferComponents
        {
            LogicalDevice*        pLogicalDevice  = nullptr;
            Swapchain*            pSwapchain      = nullptr;
            Framebuffer*          pFramebuffer    = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct CommandbufferCreateInfo
        {
            LogicalDevice*        pLogicalDevice  = nullptr;
            Swapchain*            pSwapchain      = nullptr;
            Framebuffer*          pFramebuffer    = nullptr;
        };

        /* functions */

        Commandbuffer();
        ~Commandbuffer();

        void                      setData(const CommandbufferCreateInfo& createInfo);

        void                      beginCommandbuffer();
        void                      beginRenderPass(RenderPass& renderPass);

        void                      bindPipeline(Pipeline& pipeline);
        void                      bindVertexBuffer(std::vector<VkBuffer>& vertexBuffers);
        void                      bindIndexBuffer(VkBuffer& indexBuffer);

        void                      draw(const size_t& drawCount);

        void                      endCommandbuffer();
        void                      endRenderPass();

        static int                createCommandbuffer(Commandbuffer* commandbuffer, const CommandbufferCreateInfo& createInfo);

        inline VkCommandBuffer&   getCommandbuffer() { return m_commandbuffer; }

        /* variables */

        CommandbufferInfo         commandbufferInfo;

    private:

        /* functions */

        /* variables */

        VkCommandBuffer           m_commandbuffer;
};
