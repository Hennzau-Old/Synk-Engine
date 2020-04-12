/*=============================================
   Author: Hennzau on Sun Apr 12 15:54:52 2020
  =============================================*/ 

#include "core/rendering/descriptors/DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout()
{

}

DescriptorSetLayout::~DescriptorSetLayout()
{

}

void DescriptorSetLayout::clean()
{
    vkDestroyDescriptorSetLayout(m_components.pLogicalDevice->getLogicalDevice(), m_descriptorSetLayout, nullptr);
}

void DescriptorSetLayout::setData(const DescriptorSetLayoutCreateInfo& createInfo)
{
    descriptorSetLayoutInfo.bindingCount    = createInfo.bindingCount;
    descriptorSetLayoutInfo.pBindings       = createInfo.pBindings;

    m_components.pLogicalDevice            = createInfo.pLogicalDevice;
}

int DescriptorSetLayout::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo   = {};
    descriptorSetLayoutCreateInfo.sType                             = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount                      = descriptorSetLayoutInfo.bindingCount;
    descriptorSetLayoutCreateInfo.pBindings                         = descriptorSetLayoutInfo.pBindings;             

    if (vkCreateDescriptorSetLayout(m_components.pLogicalDevice->getLogicalDevice(), &descriptorSetLayoutCreateInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    {
        Logger::printError("DescriptorSetLayout::createDescriptorSetLayout", "vkCreateDescriptorSetLayout failed!");

        return 1;
    }

    return 0;
}

int DescriptorSetLayout::createDescriptorSetLayout(DescriptorSetLayout* descriptorSetLayout, const DescriptorSetLayoutCreateInfo& createInfo)
{
    descriptorSetLayout->setData(createInfo);

    return descriptorSetLayout->createDescriptorSetLayout();
}
