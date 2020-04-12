/*=============================================
   Author: Hennzau on Sat Apr 11 13:20:18 2020
  =============================================*/ 

#include "core/rendering/Submit.h"

Submit::Submit()
{

}

Submit::~Submit()
{

}

void Submit::clean()
{
    for (auto i { 0 }; i <  MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(m_components.pLogicalDevice->getLogicalDevice(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_components.pLogicalDevice->getLogicalDevice(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_components.pLogicalDevice->getLogicalDevice(), m_inFlightFences[i], nullptr);
    }
}

void Submit::setData(const SubmitCreateInfo& createInfo)
{
    m_components.pLogicalDevice     = createInfo.pLogicalDevice;
    m_components.pSwapchain         = createInfo.pSwapchain;
    m_components.pWindow            = createInfo.pWindow;
}

int Submit::submit(CommandBuffers* commandBuffers)
{
    vkWaitForFences(m_components.pLogicalDevice->getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_components.pLogicalDevice->getLogicalDevice(), m_components.pSwapchain->getSwapchain(), UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return 2;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        Logger::printError("Submit::submit", "vkAcquireNextImageKHR failed!");

        return 1;
    }

    if (m_inFlightImages[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(m_components.pLogicalDevice->getLogicalDevice(), 1, &m_inFlightImages[m_currentFrame], VK_TRUE, UINT64_MAX);
    }

    m_inFlightImages[imageIndex] = m_inFlightFences[m_currentFrame];

    VkSemaphore waitSemaphores[] =
    {
        m_imageAvailableSemaphores[m_currentFrame]
    };

    VkSemaphore signalSemaphores[] =
    {
        m_renderFinishedSemaphores[m_currentFrame]
    };

    VkPipelineStageFlags waitStages[] =
    {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo submitInfo           = {};
    submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount     = 1;
    submitInfo.pWaitSemaphores        = waitSemaphores;
    submitInfo.pWaitDstStageMask      = waitStages;
    submitInfo.commandBufferCount     = 1;
    submitInfo.pCommandBuffers        = &commandBuffers->getCommandBuffers()[imageIndex];
    submitInfo.signalSemaphoreCount   = 1;
    submitInfo.pSignalSemaphores      = signalSemaphores;

    vkResetFences(m_components.pLogicalDevice->getLogicalDevice(), 1, &m_inFlightFences[m_currentFrame]);

    if (vkQueueSubmit(m_components.pLogicalDevice->getGraphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
    {
        Logger::printError("Submit::Submit", "vkQueueSubmit failed!");

        return 1;
    }

    VkSwapchainKHR swapChains[] =
    {
        m_components.pSwapchain->getSwapchain()
    };

    VkPresentInfoKHR presentInfo    = {};
    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;
    presentInfo.swapchainCount      = 1;
    presentInfo.pSwapchains         = swapChains;
    presentInfo.pImageIndices       = &imageIndex;
    presentInfo.pResults            = nullptr;

    result = vkQueuePresentKHR(m_components.pLogicalDevice->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_components.pWindow->windowInfo.resizedStatus)
    {
        m_components.pWindow->windowInfo.resizedStatus = false;
        
        return 2;
    } else if (result != VK_SUCCESS)
    {
        Logger::printError("Submit::submit", "vkQueuePresentKHR failed!");
        return 1;
    }

    m_currentFrame++;
    if (m_currentFrame >= MAX_FRAMES_IN_FLIGHT) m_currentFrame = 0;

    return 0;
}

int Submit::createSubmit()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightImages.resize(m_components.pSwapchain->getImages().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(m_components.pLogicalDevice->getLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS)
        {
            Logger::printError("Submit::createSyncObjects", "vkCreateSemaphore[imageAvailable][" + std::to_string(i) + "] failed!");

            return 1;
        }

        if (vkCreateSemaphore(m_components.pLogicalDevice->getLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS)
        {
            Logger::printError("Submit::createSyncObjects", "vkCreateSemaphore[renderFinished][" + std::to_string(i) + "] failed!");

            return 1;
        }

        if (vkCreateFence(m_components.pLogicalDevice->getLogicalDevice(), &fenceCreateInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
        {
            Logger::printError("Submit::createSyncObjects", "vkCreateFence[" + std::to_string(i) + "] failed!");

            return 1;
        }
    }

    return 0;
}

int Submit::createSubmit(Submit* submit, const SubmitCreateInfo& createInfo)
{
    submit->setData(createInfo);

    return submit->createSubmit();
}
