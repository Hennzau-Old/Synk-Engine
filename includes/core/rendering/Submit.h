#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Window.h"
#include "core/LogicalDevice.h"
#include "core/Swapchain.h"
#include "core/rendering/buffers/CommandBuffers.h"

class Scene;

class Submit
{
    private:

        /* private structures */

        struct SubmitInfo
        {

        };

        struct SubmitComponents
        {
            Window*               pWindow               = nullptr;
            LogicalDevice*        pLogicalDevice        = nullptr;
            Swapchain*            pSwapchain            = nullptr;
            Scene*                pScene                = nullptr;

        } m_components;

    public:

        /* static variables */

        const int MAX_FRAMES_IN_FLIGHT  = 2;

        /* structure */

        struct SubmitCreateInfo
        {
            Window*               pWindow               = nullptr;
            LogicalDevice*        pLogicalDevice        = nullptr;
            Swapchain*            pSwapchain            = nullptr;
            Scene*                pScene                = nullptr;
        };

        /* functions */

        Submit();
        ~Submit();

        void                      clean();
        void                      setData(const SubmitCreateInfo& createInfo);

        void                      submit(CommandBuffers* commandBuffers);

        static int                createSubmit(Submit* submit, const SubmitCreateInfo& createInfo);

        /* variables */

        SubmitInfo                submitInfo;

    private:

        /* functions */

        int                       createSyncObjects();

        /* variables */

        std::vector<VkSemaphore>  m_imageAvailableSemaphores;
        std::vector<VkSemaphore>  m_renderFinishedSemaphores;
        std::vector<VkFence>      m_inFlightFences;
        std::vector<VkFence>      m_inFlightImages;

        size_t                    m_currentFrame = 0;

};
