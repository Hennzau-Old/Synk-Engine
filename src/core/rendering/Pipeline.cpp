#include "core/rendering/Pipeline.h"

Pipeline::Pipeline()
{

}

Pipeline::~Pipeline()
{

}

void Pipeline::clean()
{
    vkDestroyPipeline(m_components.pLogicalDevice->getLogicalDevice(), m_pipeline, nullptr);

    Logger::printInfo("Pipeline::clean", "vkDestroyPipeline!");

    vkDestroyPipelineLayout(m_components.pLogicalDevice->getLogicalDevice(), m_pipelineLayout, nullptr);

    Logger::printInfo("Pipeline::clean", "vkDestroyPipelineLayout!");

    vkDestroyDescriptorSetLayout(m_components.pLogicalDevice->getLogicalDevice(), m_descriptorSetLayout, nullptr);

    Logger::printInfo("Pipeline::clean", "vkDestroyDescriptorSetLayout!");
}

void Pipeline::setData(const PipelineCreateInfo& createInfo)
{
    m_components.pLogicalDevice = createInfo.pLogicalDevice;
    m_components.pSwapchain     = createInfo.pSwapchain;
    m_components.pShader        = createInfo.pShader;
    m_components.pRenderPass    = createInfo.pRenderPass;

    pipelineInfo.rasterizationInfo  = createInfo.rasterizationInfo;
    pipelineInfo.vertexInputInfo    = createInfo.vertexInputInfo;
    pipelineInfo.descriptorsInfo    = createInfo.descriptorsInfo;
}

int Pipeline::createPipeline()
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo  = {};
    vertexInputInfo.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount         = 1;
    vertexInputInfo.vertexAttributeDescriptionCount       = static_cast<uint32_t>(pipelineInfo.vertexInputInfo.vertexAttributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions            = &pipelineInfo.vertexInputInfo.vertexBindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions          = pipelineInfo.vertexInputInfo.vertexAttributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly  = {};
    inputAssembly.sType                                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology                                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable                  = VK_FALSE;

    VkViewport viewport = {};
    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = (float) m_components.pSwapchain->getExtent().width;
    viewport.height     = (float) m_components.pSwapchain->getExtent().height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    VkRect2D scissor    = {};
    scissor.offset      = { 0, 0 };
    scissor.extent      = m_components.pSwapchain->getExtent();

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount                     = 1;
    viewportState.pViewports                        = &viewport;
    viewportState.scissorCount                      = 1;
    viewportState.pScissors                         = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable                       = VK_FALSE;
    rasterizer.rasterizerDiscardEnable                = VK_FALSE;
    rasterizer.polygonMode                            = pipelineInfo.rasterizationInfo.polygonMode;
    rasterizer.cullMode                               = pipelineInfo.rasterizationInfo.cullMode;
    rasterizer.frontFace                              = pipelineInfo.rasterizationInfo.frontFace;
    rasterizer.lineWidth                              = pipelineInfo.rasterizationInfo.lineWidth;
    rasterizer.depthBiasEnable                        = VK_FALSE;
    rasterizer.depthBiasConstantFactor                = 0.0f;
    rasterizer.depthBiasClamp                         = 0.0f;
    rasterizer.depthBiasSlopeFactor                   = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling  = {};
    multisampling.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable                   = VK_FALSE;
    multisampling.rasterizationSamples                  = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading                      = 1.0f;
    multisampling.pSampleMask                           = nullptr;
    multisampling.alphaToCoverageEnable                 = VK_FALSE;
    multisampling.alphaToOneEnable                      = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment  = {};
    colorBlendAttachment.colorWriteMask                       = VK_COLOR_COMPONENT_R_BIT |
                                                                VK_COLOR_COMPONENT_G_BIT |
                                                                VK_COLOR_COMPONENT_B_BIT |
                                                                VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable                          = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor                  = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor                  = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp                         = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor                  = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor                  = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp                         = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable                       = VK_FALSE;
    colorBlending.logicOp                             = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount                     = 1;
    colorBlending.pAttachments                        = &colorBlendAttachment;
    colorBlending.blendConstants[0]                   = 0.0f;
    colorBlending.blendConstants[1]                   = 0.0f;
    colorBlending.blendConstants[2]                   = 0.0f;
    colorBlending.blendConstants[3]                   = 0.0f;

    VkDynamicState dynamicStates[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount                = 2;
    dynamicState.pDynamicStates                   = dynamicStates;

    uint32_t bindingCount = static_cast<uint32_t>(pipelineInfo.descriptorsInfo.descriptors.size());

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
    descriptorSetLayoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount                    = bindingCount;
    descriptorSetLayoutInfo.pBindings                       = pipelineInfo.descriptorsInfo.descriptors.data();

    if (vkCreateDescriptorSetLayout(m_components.pLogicalDevice->getLogicalDevice(), &descriptorSetLayoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
    {
        Logger::printError("Pipeline::createPipeline", "vkCreateDescriptorSetLayout failed!");

        return 1;
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount             = 1;
    pipelineLayoutCreateInfo.pSetLayouts                = &m_descriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount     = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges        = nullptr;

    if (vkCreatePipelineLayout(m_components.pLogicalDevice->getLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        Logger::printError("Pipeline::createPipeline", "vkCreatePipelineLayout failed!");

        return 1;
    }

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount                   = 2;
    pipelineCreateInfo.pStages                      = m_components.pShader->getShaderStages().data();
    pipelineCreateInfo.pVertexInputState            = &vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState          = &inputAssembly;
    pipelineCreateInfo.pViewportState               = &viewportState;
    pipelineCreateInfo.pRasterizationState          = &rasterizer;
    pipelineCreateInfo.pMultisampleState            = &multisampling;
    pipelineCreateInfo.pDepthStencilState           = nullptr;
    pipelineCreateInfo.pColorBlendState             = &colorBlending;
    pipelineCreateInfo.pDynamicState                = nullptr;
    pipelineCreateInfo.layout                       = m_pipelineLayout;
    pipelineCreateInfo.renderPass                   = m_components.pRenderPass->getRenderPass();
    pipelineCreateInfo.subpass                      = 0;
    pipelineCreateInfo.basePipelineHandle           = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex            = -1;

    if (vkCreateGraphicsPipelines(m_components.pLogicalDevice->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline) != VK_SUCCESS)
    {
        Logger::printError("Pipeline::createPipeline", "vkCreateGraphicsPipeline failed!");

        return 1;
    }

    return 0;
}

int Pipeline::createPipeline(Pipeline* pipeline, const PipelineCreateInfo& createInfo)
{
    pipeline->setData(createInfo);

    return pipeline->createPipeline();
}
