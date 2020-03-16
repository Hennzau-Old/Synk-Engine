#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class RenderPass
{
    private:

        /* private structures */

        struct RenderPassInfo
        {
            VkFormat                format;
            VkSampleCountFlagBits   samples;
            VkAttachmentLoadOp      loadOp;
            VkAttachmentStoreOp     storeOp;
            VkAttachmentLoadOp      stencilLoadOp;
            VkAttachmentStoreOp     stencilStoreOp;
            VkImageLayout           initialLayout;
            VkImageLayout           finalLayout;
        };

        struct RenderPassComponents
        {
            LogicalDevice*          pLogicalDevice  = nullptr;

        } m_components;

    public:

        /* static variables */



        /* structure */

        struct RenderPassCreateInfo
        {
            LogicalDevice*          pLogicalDevice  = nullptr;

            VkFormat                format;
            VkSampleCountFlagBits   samples;
            VkAttachmentLoadOp      loadOp;
            VkAttachmentStoreOp     storeOp;
            VkAttachmentLoadOp      stencilLoadOp;
            VkAttachmentStoreOp     stencilStoreOp;
            VkImageLayout           initialLayout;
            VkImageLayout           finalLayout;
        };

        /* functions */

        RenderPass();
        ~RenderPass();

        void                      clean();
        void                      setData(const RenderPassCreateInfo& createInfo);

        static int                createRenderPass(RenderPass* renderPass, const RenderPassCreateInfo& createInfo);

        inline VkRenderPass&      getRenderPass() { return m_renderPass; }

        /* variables */

        RenderPassInfo            renderPassInfo;

    private:

        /* functions */

        int                       createRenderPass();

        /* variables */

        VkRenderPass              m_renderPass;
};
