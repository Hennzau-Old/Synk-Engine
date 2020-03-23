#include "core/rendering/Framebuffer.h"

Framebuffer::Framebuffer()
{

}

Framebuffer::~Framebuffer()
{

}

void Framebuffer::clean()
{
    vkDestroyFramebuffer(m_components.pLogicalDevice->getLogicalDevice(), m_framebuffer, nullptr);

    Logger::printInfo("Framebuffer::clean", "vkDestroyFramebuffer!");
}

void Framebuffer::setData(const FramebufferCreateInfo& createInfo)
{
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;
    m_components.pSwapchain       = createInfo.pSwapchain;
    m_components.pRenderPass      = createInfo.pRenderPass;

    framebufferInfo.imageViewCount  = createInfo.imageViewCount;
    framebufferInfo.pImageViews     = createInfo.pImageViews;
}

int Framebuffer::createFramebuffer()
{
    VkImageView attachments[framebufferInfo.imageViewCount];

    for (size_t i = 0; i < framebufferInfo.imageViewCount; i++)
    {
        attachments[i] = framebufferInfo.pImageViews[i];
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass              = m_components.pRenderPass->getRenderPass();
    framebufferCreateInfo.attachmentCount         = framebufferInfo.imageViewCount;
    framebufferCreateInfo.pAttachments            = attachments;
    framebufferCreateInfo.width                   = m_components.pSwapchain->getExtent().width;
    framebufferCreateInfo.height                  = m_components.pSwapchain->getExtent().height;
    framebufferCreateInfo.layers                  = 1;

    if (vkCreateFramebuffer(m_components.pLogicalDevice->getLogicalDevice(), &framebufferCreateInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
    {
        Logger::printError("Framebuffer::createFramebuffer", "vkCreateFramebuffer failed!");

        return 1;
    }
    
    return 0;
}

int Framebuffer::createFramebuffer(Framebuffer* framebuffer, const FramebufferCreateInfo& createInfo)
{
    framebuffer->setData(createInfo);

    return framebuffer->createFramebuffer();
}
