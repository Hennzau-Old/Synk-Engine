#include "core/rendering/CommandPool.h"

CommandPool::CommandPool()
{

}

CommandPool::~CommandPool()
{

}

void CommandPool::clean()
{
    vkDestroyCommandPool(m_components.pLogicalDevice->getLogicalDevice(), m_commandPool, nullptr);

    Logger::printInfo("CommandPool::clean", "vkDestroyCommandPool!");
}

void CommandPool::setData(const CommandPoolCreateInfo& createInfo)
{
    m_components.pPhysicalDevice  = createInfo.pPhysicalDevice;
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;

    commandPoolInfo.queueFamilyIndex  = createInfo.queueFamilyIndex;
}

int CommandPool::allocateCommandbuffers(std::vector<Commandbuffer>& commandbuffers)
{
    VkCommandBufferAllocateInfo commandBufferAllocInfo  = {};
    commandBufferAllocInfo.sType                        = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool                  = m_commandPool;
    commandBufferAllocInfo.level                        = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount           = (uint32_t) commandbuffers.size();

    if (vkAllocateCommandBuffers(m_components.pLogicalDevice->getLogicalDevice(), &commandBufferAllocInfo, &commandbuffers.data()->getCommandbuffer()) != VK_SUCCESS)
    {
        Logger::printError("CommandBuffers::createCommandBuffers", "vkAllocateCommandBuffers failed!");

        return 1;
    }

    return 0;
}

void CommandPool::freeCommandbuffers(std::vector<Commandbuffer>& commandbuffers)
{
    vkFreeCommandBuffers(m_components.pLogicalDevice->getLogicalDevice(), m_commandPool, static_cast<uint32_t>(commandbuffers.size()), &commandbuffers.data()->getCommandbuffer());
}

int CommandPool::createCommandPool()
{
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.queueFamilyIndex        = commandPoolInfo.queueFamilyIndex;
    commandPoolCreateInfo.flags                   = 0;

    if (vkCreateCommandPool(m_components.pLogicalDevice->getLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        Logger::printError("CommandPool::createCommandPool", "vkCreateCommandPool failed!");

        return 1;
    }

    return 0;
}

int CommandPool::createCommandPool(CommandPool* commandPool, const CommandPoolCreateInfo& createInfo)
{
    commandPool->setData(createInfo);

    return commandPool->createCommandPool();
}
