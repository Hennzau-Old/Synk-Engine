/*=============================================
   Author: Hennzau on Sat Apr 11 12:45:01 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/RenderPass.h"

class Framebuffer
{
    private:

        /* private structures */

        struct FramebufferInfo
        {
            uint32_t            imageViewCount  = 0;
            VkImageView*        pImageViews     = nullptr;
        };

        struct FramebufferComponents
        {
            LogicalDevice*      pLogicalDevice  = nullptr;
            Swapchain*          pSwapchain      = nullptr;
            RenderPass*         pRenderPass     = nullptr;
        } m_components;

    public:

        /* static variables */



        /* structure */

        struct FramebufferCreateInfo
        {
            LogicalDevice*      pLogicalDevice  = nullptr;
            Swapchain*          pSwapchain      = nullptr;
            RenderPass*         pRenderPass     = nullptr;

            uint32_t            imageViewCount  = 0;
            VkImageView*        pImageViews     = nullptr;
        };

        /* functions */

        Framebuffer();
        ~Framebuffer();

        void                    clean();
        void                    setData(const FramebufferCreateInfo& createInfo);

        static int              createFramebuffer(Framebuffer* framebuffer, const FramebufferCreateInfo& createInfo);

        inline VkFramebuffer&   getFramebuffer() { return m_framebuffer; }

        /* variables */

        FramebufferInfo         framebufferInfo;

    private:

        /* functions */

        int                     createFramebuffer();

        /* variables */

        VkFramebuffer           m_framebuffer;
};
