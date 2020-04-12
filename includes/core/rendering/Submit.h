/*=============================================
   Author: Hennzau on Sat Apr 11 13:20:18 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/buffers/CommandBuffers.h"

class Submit
{
    private:

        /* private structures */

        struct SubmitInfo
        {

        };

        struct SubmitComponents
        {
            LogicalDevice*          pLogicalDevice      = nullptr;
            Swapchain*              pSwapchain          = nullptr;
            Window*                 pWindow             = nullptr;
        } m_components;

    public:

        /* static variables */

        const int MAX_FRAMES_IN_FLIGHT  = 2;

        /* structure */

        struct SubmitCreateInfo
        {
            LogicalDevice*          pLogicalDevice      = nullptr;
            Swapchain*              pSwapchain          = nullptr;
            Window*                 pWindow             = nullptr;
        };

        /* functions */

        Submit();
        ~Submit();

        void                        clean();
        void                        setData(const SubmitCreateInfo& createInfo);

        int                         submit(CommandBuffers* commandBuffers);

        static int                  createSubmit(Submit* submit, const SubmitCreateInfo& createInfo);

        /* variables */

        SubmitInfo                  submitInfo;

    private:

        /* functions */

        int                         createSubmit();

        /* variables */

        std::vector<VkSemaphore>    m_imageAvailableSemaphores;
        std::vector<VkSemaphore>    m_renderFinishedSemaphores;
        std::vector<VkFence>        m_inFlightFences;
        std::vector<VkFence>        m_inFlightImages;

        size_t                      m_currentFrame = 0;
};
