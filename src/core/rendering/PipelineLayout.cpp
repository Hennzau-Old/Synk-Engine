/*=============================================
   Author: Hennzau on Sat Apr 11 11:06:41 2020
  =============================================*/ 

#include "core/rendering/PipelineLayout.h"

PipelineLayout::PipelineLayout()
{

}

PipelineLayout::~PipelineLayout()
{

}

void PipelineLayout::clean()
{
    vkDestroyPipelineLayout(m_components.pLogicalDevice->getLogicalDevice(), m_pipelineLayout, nullptr);

    Logger::printInfo("Pipeline::clean", "vkDestroyPipelineLayout!");
}

void PipelineLayout::setData(const PipelineLayoutCreateInfo& createInfo)
{
    m_components.pLogicalDevice                 = createInfo.pLogicalDevice;

    pipelineLayoutInfo.descriptorSetLayoutCount = createInfo.descriptorSetLayoutCount;
    pipelineLayoutInfo.pDescriptorSetLayout     = createInfo.pDescriptorSetLayout;
}

int PipelineLayout::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount             = pipelineLayoutInfo.descriptorSetLayoutCount;      
    pipelineLayoutCreateInfo.pSetLayouts                = pipelineLayoutInfo.pDescriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount     = 0;    
    pipelineLayoutCreateInfo.pPushConstantRanges        = nullptr;

    if (vkCreatePipelineLayout(m_components.pLogicalDevice->getLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) 
    {
        Logger::printError("PipelineLayout::createPipelineLayout", "createPipelineLayout failed!");
        
        return 1;
    }

    return 0;
}

int PipelineLayout::createPipelineLayout(PipelineLayout* pipelinelayout, const PipelineLayoutCreateInfo& createInfo)
{
    pipelinelayout->setData(createInfo);

    return pipelinelayout->createPipelineLayout();
}
