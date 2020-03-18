#include "core/rendering/buffers/Buffer.h"

Buffer::Buffer()
{

}

Buffer::~Buffer()
{

}

void Buffer::clean()
{
    vkDestroyBuffer(m_components.pLogicalDevice->getLogicalDevice(), m_buffer, nullptr);

    Logger::printInfo("Buffer::clean", "vkDestroyBuffer!");

    vkFreeMemory(m_components.pLogicalDevice->getLogicalDevice(), m_bufferMemory, nullptr);

    Logger::printInfo("Buffer::clean", "vkFreeMemory!");
}

void Buffer::setData(const BufferCreateInfo& createInfo)
{
    m_components.pPhysicalDevice  = createInfo.pPhysicalDevice;
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;
    m_components.pCommandPool     = createInfo.pCommandPool;

    bufferInfo.size       = createInfo.size;
    bufferInfo.usage      = createInfo.usage;
    bufferInfo.properties = createInfo.properties;
}

void Buffer::copyToBuffer(Buffer* dstBuffer)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool                 = m_components.pCommandPool->getCommandPool();
    allocInfo.commandBufferCount          = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_components.pLogicalDevice->getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo  = {};
    beginInfo.sType                     = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                     = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset    = 0;
    copyRegion.dstOffset    = 0;
    copyRegion.size         = bufferInfo.size;

    vkCmdCopyBuffer(commandBuffer, m_buffer, dstBuffer->getBuffer(), 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo       = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(m_components.pLogicalDevice->getTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_components.pLogicalDevice->getTransferQueue());

    vkFreeCommandBuffers(m_components.pLogicalDevice->getLogicalDevice(), m_components.pCommandPool->getCommandPool(), 1, &commandBuffer);
}

int Buffer::createBuffer()
{
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size               = bufferInfo.size;
    bufferCreateInfo.usage              = bufferInfo.usage;
    bufferCreateInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_components.pLogicalDevice->getLogicalDevice(), &bufferCreateInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        Logger::printError("Buffer::createBuffer", "vkCreateBuffer failed!");

        return 1;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_components.pLogicalDevice->getLogicalDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo  = {};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize        = memRequirements.size;
    allocInfo.memoryTypeIndex       = findMemoryType(memRequirements.memoryTypeBits, bufferInfo.properties);

    if (vkAllocateMemory(m_components.pLogicalDevice->getLogicalDevice(), &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
    {
        Logger::printError("Buffer::createBuffer", "vkAllocateMemory failed!");

        return 1;
    }

    vkBindBufferMemory(m_components.pLogicalDevice->getLogicalDevice(), m_buffer, m_bufferMemory, 0);

    return 0;
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_components.pPhysicalDevice->getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties))
        {
            return i;
        }
    }

    Logger::printError("Buffer::findMemoryType", "No memory type can be used for the buffer!");

    return 0;
}

int Buffer::createBuffer(Buffer* buffer, const BufferCreateInfo& createInfo)
{
    buffer->setData(createInfo);

    return buffer->createBuffer();
}
