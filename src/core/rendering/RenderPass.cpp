#include "core/rendering/RenderPass.h"

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::clean()
{
    vkDestroyRenderPass(m_components.pLogicalDevice->getLogicalDevice(), m_renderPass, nullptr);

    Logger::printInfo("RenderPass::clean", "vkDestroyRenderPass!");
}

void RenderPass::setData(const RenderPassCreateInfo& createInfo)
{
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;

    renderPassInfo.format         = createInfo.format;
    renderPassInfo.samples        = createInfo.samples;
    renderPassInfo.loadOp         = createInfo.loadOp;
    renderPassInfo.storeOp        = createInfo.storeOp;
    renderPassInfo.stencilLoadOp  = createInfo.stencilLoadOp;
    renderPassInfo.stencilStoreOp = createInfo.stencilStoreOp;
    renderPassInfo.initialLayout  = createInfo.initialLayout;
    renderPassInfo.finalLayout    = createInfo.finalLayout;
}

int RenderPass::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format                  = renderPassInfo.format;
    colorAttachment.samples                 = renderPassInfo.samples;
    colorAttachment.loadOp                  = renderPassInfo.loadOp;
    colorAttachment.storeOp                 = renderPassInfo.storeOp;
    colorAttachment.stencilLoadOp           = renderPassInfo.stencilLoadOp;
    colorAttachment.stencilStoreOp          = renderPassInfo.stencilStoreOp;
    colorAttachment.initialLayout           = renderPassInfo.initialLayout;
    colorAttachment.finalLayout             = renderPassInfo.finalLayout;

    VkAttachmentReference colorAttachmentReference  = {};
    colorAttachmentReference.attachment             = 0;
    colorAttachmentReference.layout                 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass  = {};
    subpass.pipelineBindPoint     = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount  = 1;
    subpass.pColorAttachments     = &colorAttachmentReference;

    VkSubpassDependency dependency  = {};
    dependency.srcSubpass           = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass           = 0;
    dependency.srcStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask        = 0;
    dependency.dstStageMask         = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask        = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount        = 1;
    renderPassCreateInfo.pAttachments           = &colorAttachment;
    renderPassCreateInfo.subpassCount           = 1;
    renderPassCreateInfo.pSubpasses             = &subpass;
    renderPassCreateInfo.dependencyCount        = 1;
    renderPassCreateInfo.pDependencies          = &dependency;

    if (vkCreateRenderPass(m_components.pLogicalDevice->getLogicalDevice(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        Logger::printError("RenderPass::createRenderPass", "vkCreateRenderpass failed!");

        return 1;
    }

    return 0;
}

int RenderPass::createRenderPass(RenderPass* renderPass, const RenderPassCreateInfo& createInfo)
{
    renderPass->setData(createInfo);

    return renderPass->createRenderPass();
}
