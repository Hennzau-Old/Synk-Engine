#include "core/rendering/buffers/Commandbuffer.h"

Commandbuffer::Commandbuffer()
{

}

Commandbuffer::~Commandbuffer()
{

}

void Commandbuffer::setData(const CommandbufferCreateInfo& createInfo)
{
    m_components.pLogicalDevice = createInfo.pLogicalDevice;
    m_components.pSwapchain     = createInfo.pSwapchain;
    m_components.pFramebuffer   = createInfo.pFramebuffer;
}

void Commandbuffer::beginCommandbuffer()
{
    VkCommandBufferBeginInfo commandBufferBeginInfo = {};
    commandBufferBeginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags                    = 0;
    commandBufferBeginInfo.pInheritanceInfo         = nullptr;

    if (vkBeginCommandBuffer(m_commandbuffer, &commandBufferBeginInfo) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffer::beginCommandBuffer", "vkBeginCommandBuffer failed!");
    }
}

void Commandbuffer::beginRenderPass(RenderPass& renderPass)
{
    VkClearValue clearColor               = { 0.0f, 0.0f, 0.0f, 1.0f };

    VkRenderPassBeginInfo renderPassInfo  = {};
    renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass             = renderPass.getRenderPass();
    renderPassInfo.framebuffer            = m_components.pFramebuffer->getFramebuffer();
    renderPassInfo.renderArea.offset      = {0, 0};
    renderPassInfo.renderArea.extent      = m_components.pSwapchain->getExtent();

    renderPassInfo.clearValueCount        = 1;
    renderPassInfo.pClearValues           = &clearColor;

    vkCmdBeginRenderPass(m_commandbuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Commandbuffer::bindPipeline(Pipeline& pipeline)
{
    vkCmdBindPipeline(m_commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());
}


void Commandbuffer::bindVertexBuffer(std::vector<VkBuffer>& vertexBuffers)
{
    VkDeviceSize offsets[] =
    {
        0
    };

    uint32_t binding = static_cast<uint32_t>(vertexBuffers.size());

    vkCmdBindVertexBuffers(m_commandbuffer, 0, binding, vertexBuffers.data(), offsets);
}

void Commandbuffer::bindIndexBuffer(VkBuffer& indexBuffer)
{
    vkCmdBindIndexBuffer(m_commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void Commandbuffer::draw(const size_t& drawCount)
{
    vkCmdDrawIndexed(m_commandbuffer, static_cast<uint32_t>(drawCount), 1, 0, 0, 0);
}

void Commandbuffer::endRenderPass()
{
    vkCmdEndRenderPass(m_commandbuffer);
}

void Commandbuffer::endCommandbuffer()
{
    if (vkEndCommandBuffer(m_commandbuffer) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffer::vkEndCommandBuffer", "vkEndCommandBuffer failed!");
    }
}

int Commandbuffer::createCommandbuffer(Commandbuffer* commandbuffer, const CommandbufferCreateInfo& createInfo)
{
    commandbuffer->setData(createInfo);

    return 0;
}
