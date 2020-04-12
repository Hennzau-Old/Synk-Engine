#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/utils/File.h"

#include "core/CoreComponents.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Shader.h"
#include "core/rendering/descriptors/DescriptorSetLayout.h"
#include "core/rendering/PipelineLayout.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/buffers/Framebuffer.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/CommandBuffers.h"
#include "core/rendering/Submit.h"

#include "core/rendering/Mesh.h"

const auto 				FRAME_CAP   { 6666 };
const auto				TICK_CAP    { 60 };

CoreComponents  coreComponents;

RenderPass      renderPass;
Shader          shader;

DescriptorSetLayout descriptorSetLayout;
std::vector<VkDescriptorSetLayout> descriptorSetLayouts;

PipelineLayout  pipelineLayout;
Pipeline        pipeline;
std::vector<Framebuffer> framebuffers;
CommandPool     commandPool;
CommandPool transferCommandPool;

CommandBuffers  commandBuffers;

Mesh test;

Submit          submit;

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

void createTransferCommandPool()
{
    CommandPool::CommandPoolCreateInfo transferCommandPoolcreateInfo  = {};
    transferCommandPoolcreateInfo.pPhysicalDevice                     = &coreComponents.getPhysicalDevice();
    transferCommandPoolcreateInfo.pLogicalDevice                      = &coreComponents.getLogicalDevice();
    transferCommandPoolcreateInfo.queueFamilyIndex                    = coreComponents.getPhysicalDevice().getQueueFamilies().transferFamily.value();

    if (CommandPool::createCommandPool(&transferCommandPool, transferCommandPoolcreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandPool failed!");
    }
}

void draw()
{
    std::vector<VkBuffer> vertexBuffers
    {
        test.getVertexBuffer().getBuffer()  
    };

    for (int i { 0 }; i < coreComponents.getSwapchain().getImageViews().size(); i++)
    {
        commandBuffers.beginCommandBuffers(i);
            commandBuffers.beginRenderPass(i, renderPass.getRenderPass());

                commandBuffers.bindPipeline(i, pipeline.getPipeline());

                commandBuffers.bindVertexBuffer(i, vertexBuffers);
                commandBuffers.bindIndexBuffer(i, test.getIndexBuffer().getBuffer());
                commandBuffers.draw(i, 6);

            commandBuffers.endRenderPass(i);
        commandBuffers.endCommandBuffers(i);
    }
}

void createRenderPass()
{
    RenderPass::RenderPassCreateInfo renderPassCreateInfo   = {};
    renderPassCreateInfo.pLogicalDevice                     = &coreComponents.getLogicalDevice();
    renderPassCreateInfo.format                             = coreComponents.getSwapchain().getImageFormat();
    renderPassCreateInfo.samples                            = VK_SAMPLE_COUNT_1_BIT;
    renderPassCreateInfo.loadOp                             = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassCreateInfo.storeOp                            = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassCreateInfo.stencilLoadOp                      = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassCreateInfo.stencilStoreOp                     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassCreateInfo.initialLayout                      = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassCreateInfo.finalLayout                        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    if (RenderPass::createRenderPass(&renderPass, renderPassCreateInfo) != 0)
    {
        Logger::printError("main::init", "createRenderPass failed!");
    }
}

void createShader()
{
    Shader::ShaderCreateInfo shaderCreateInfo   = {};
    shaderCreateInfo.pLogicalDevice             = &coreComponents.getLogicalDevice();
    shaderCreateInfo.vertexShaderCode           = File::readFile("res/shaders/spir-v/main.vert");
    shaderCreateInfo.fragmentShaderCode         = File::readFile("res/shaders/spir-v/main.frag");

    if (Shader::createShader(&shader, shaderCreateInfo) != 0)
    {
        Logger::printError("main::init", "createShader failed!");
    }
}

void createDescriptorSetLayouts()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding   = {};
    uboLayoutBinding.binding                        = 0;
    uboLayoutBinding.descriptorType                 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount                = 1;
    uboLayoutBinding.stageFlags                     = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers             = nullptr;

    DescriptorSetLayout::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.pLogicalDevice    = &coreComponents.getLogicalDevice();
    descriptorSetLayoutCreateInfo.bindingCount      = 1;
    descriptorSetLayoutCreateInfo.pBindings         = &uboLayoutBinding;

    if (DescriptorSetLayout::createDescriptorSetLayout(&descriptorSetLayout, descriptorSetLayoutCreateInfo) != 0)
    {
        Logger::printError("main::init", "createDescriptorSetLayout failed!");
    }

    descriptorSetLayouts.push_back(descriptorSetLayout.getDescriptorSetLayout());
}

void createPipelineLayout()
{
    PipelineLayout::PipelineLayoutCreateInfo pipelineLayoutCreateInfo   = {};
    pipelineLayoutCreateInfo.pLogicalDevice                             = &coreComponents.getLogicalDevice();        
    pipelineLayoutCreateInfo.descriptorSetLayoutCount                   = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutCreateInfo.pDescriptorSetLayout                       = descriptorSetLayouts.data();

    if (PipelineLayout::createPipelineLayout(&pipelineLayout, pipelineLayoutCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPipelineLayout failed!");
    }
}

void createPipeline()
{
  /*vertex binding */

    VkVertexInputBindingDescription vertexBindingDescription    = {};
    vertexBindingDescription.binding                            = 0;
    vertexBindingDescription.stride                             = 7 * sizeof(float);
    vertexBindingDescription.inputRate                          = VK_VERTEX_INPUT_RATE_VERTEX;

    /* attributs description */

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding    = 0;
    attributeDescriptions[0].location   = 0;
    attributeDescriptions[0].format     = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset     = 0.0f;     //offset of vertex positions

    attributeDescriptions[1].binding    = 0;
    attributeDescriptions[1].location   = 1;
    attributeDescriptions[1].format     = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset     = 3 * sizeof(float);     //offset of colors

    /* pipeline structs */

    Pipeline::RasterizationInfo rasterizationInfo   = {};
    rasterizationInfo.polygonMode                   = VK_POLYGON_MODE_FILL;
    rasterizationInfo.cullMode                      = VK_CULL_MODE_FRONT_BIT;
    rasterizationInfo.frontFace                     = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.lineWidth                     = 1.0f;

    Pipeline::VertexInputInfo vertexInputInfo           = {};
    vertexInputInfo.vertexBindingDescriptionCount       = 1;
    vertexInputInfo.pVertexBindingDescription           = &vertexBindingDescription;
    vertexInputInfo.vertexAttributeDescriptionsCount    = 2;
    vertexInputInfo.pVertexAttributeDescriptions        = attributeDescriptions.data();

    /* Pipeline Layout Creation */

 
    /* create the graphics pipeline */

    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.pLogicalDevice               = &coreComponents.getLogicalDevice();
    pipelineCreateInfo.pSwapchain                   = &coreComponents.getSwapchain();
    pipelineCreateInfo.pShader                      = &shader;
    pipelineCreateInfo.pRenderPass                  = &renderPass;
    pipelineCreateInfo.pPipelineLayout              = &pipelineLayout;
    pipelineCreateInfo.rasterizationInfo            = rasterizationInfo;
    pipelineCreateInfo.vertexInputInfo              = vertexInputInfo;

    if (Pipeline::createPipeline(&pipeline, pipelineCreateInfo) != 0)
    {
        Logger::printError("main::init", "createPipeline failed!");
    }
}

void createFramebuffers()
{
    const auto imageViewCount { coreComponents.getSwapchain().getImageViews().size() };

    framebuffers.resize(imageViewCount);

    Framebuffer::FramebufferCreateInfo framebufferCreateInfo  = {};
    framebufferCreateInfo.pLogicalDevice                      = &coreComponents.getLogicalDevice();
    framebufferCreateInfo.pSwapchain                          = &coreComponents.getSwapchain();
    framebufferCreateInfo.pRenderPass                         = &renderPass;

    for (auto i { 0 }; i < imageViewCount; i++)
    {
        std::vector<VkImageView> attachments =
        {
            coreComponents.getSwapchain().getImageViews()[i]
        };

        framebufferCreateInfo.imageViewCount                      = static_cast<uint32_t>(attachments.size());
        framebufferCreateInfo.pImageViews                         = attachments.data();

        if (Framebuffer::createFramebuffer(&framebuffers[i], framebufferCreateInfo) != 0)
        {
            Logger::printError("main::init", "createFramebuffer[" + std::to_string(i) + "] failed!");

        } 
    }     
}

void createCommandPool()
{
    CommandPool::CommandPoolCreateInfo commandPoolCreateInfo  = {};
    commandPoolCreateInfo.pPhysicalDevice                     = &coreComponents.getPhysicalDevice();
    commandPoolCreateInfo.pLogicalDevice                      = &coreComponents.getLogicalDevice();
    commandPoolCreateInfo.queueFamilyIndex                    = coreComponents.getPhysicalDevice().getQueueFamilies().graphicsFamily.value();

    if (CommandPool::createCommandPool(&commandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandPool failed!");
    } 
}

void createCommandBuffers()
{
    CommandBuffers::CommandBuffersCreateInfo commandBuffersCreateInfo = {};
    commandBuffersCreateInfo.pFramebuffers                            = &framebuffers;
    commandBuffersCreateInfo.pLogicalDevice                           = &coreComponents.getLogicalDevice();
    commandBuffersCreateInfo.pSwapchain                               = &coreComponents.getSwapchain();
    commandBuffersCreateInfo.pCommandPool                             = &commandPool;

    if (CommandBuffers::createCommandBuffers(&commandBuffers, commandBuffersCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCommandBuffers failed!");
    } 
}

void createSubmit()
{
    Submit::SubmitCreateInfo submitCreateInfo   = {};
    submitCreateInfo.pLogicalDevice             = &coreComponents.getLogicalDevice();
    submitCreateInfo.pSwapchain                 = &coreComponents.getSwapchain();
    submitCreateInfo.pWindow                    = &coreComponents.getWindow();

    if (Submit::createSubmit(&submit, submitCreateInfo) != 0)
    {
        Logger::printError("main::init", "createSubmit failed!");
    }
}

void resize()
{
    coreComponents.getLogicalDevice().wait();

    for (auto framebuffer : framebuffers)
    {
        framebuffer.clean();
    }

    commandBuffers.clean();

    pipeline.clean();
    pipelineLayout.clean();
    renderPass.clean();

    coreComponents.getSwapchain().clean();

    coreComponents.createSwapchain();
    createRenderPass();
    createDescriptorSetLayouts();
    createPipelineLayout();
    createPipeline();
    createFramebuffers();
    createCommandBuffers();

    draw();
}

void init()
{
    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.initialWidth             = 1280;
    windowCreateInfo.initialHeight            = 720;
    windowCreateInfo.resizable                = GLFW_TRUE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    CoreComponents::CoreComponentsCreateInfo coreComponentsCreateInfo = {};
    coreComponentsCreateInfo.windowCreateInfo                         = windowCreateInfo;
    coreComponentsCreateInfo.imageUsage                               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (CoreComponents::createCoreComponents(&coreComponents, coreComponentsCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCoreComponents failed!");
    }

    /* rendering part */

    createRenderPass();
    createShader();
    createDescriptorSetLayouts();
    createPipelineLayout();
    createPipeline();
    createFramebuffers();  
    createCommandPool();
    createCommandBuffers();
    createSubmit();

    /* mesh */

    createTransferCommandPool();

    std::vector<float> vertices =
    {
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
    };

    std::vector<uint16_t> indices =
    {
        0, 1, 2,
        2, 3, 0,
    };

    Mesh::MeshCreateInfo meshCreateInfo = {};
    meshCreateInfo.pPhysicalDevice      = &coreComponents.getPhysicalDevice();
    meshCreateInfo.pLogicalDevice       = &coreComponents.getLogicalDevice();
    meshCreateInfo.pCommandPool         = &transferCommandPool;
    meshCreateInfo.vertices             = vertices;
    meshCreateInfo.indices              = indices;

    if (Mesh::createMesh(&test, meshCreateInfo) != 0)
    {
        Logger::printError("main::init", "createMesh failed!");
    }

    transferCommandPool.clean();

    /* render */

    draw();    
}

void update()
{
    coreComponents.getWindow().update();
}

void render()
{
    if (submit.submit(&commandBuffers) == 2)
    {
        resize();
    }
}

void clean()
{
    coreComponents.getLogicalDevice().wait();

    test.clean();

    submit.clean();

    commandBuffers.clean();
    commandPool.clean();

    descriptorSetLayout.clean();

    pipelineLayout.clean();
    pipeline.clean();
    shader.clean();

    for (auto framebuffer : framebuffers)
    {
        framebuffer.clean();
    }

    renderPass.clean();

    coreComponents.clean();
}

int main()
{
    init();

    Logger::init("_____GAME__ENGINE_____");

    const auto  tickTime    { 1.0 / TICK_CAP };
    const auto  renderTime  { 1.0 / FRAME_CAP };

    auto    lastTickTime    { glfwGetTime() };
    auto    timer           { lastTickTime  };
    auto    lastRenderTime  { glfwGetTime() };
    auto    deltaTime       { 0.0 };
    auto    nowTime         { 0.0 };

    auto    frames          { 0 };
    auto    ticks           { 0 };

    while(!coreComponents.getWindow().isClosed())
    {
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTickTime) / tickTime;
        lastTickTime = nowTime;

        while(deltaTime >= 1.0)
        {
            ticks++;
            update();
            deltaTime--;
        }

        if(nowTime - lastRenderTime >= renderTime)
        {
            lastRenderTime = nowTime;
            render();
            frames++;
        }

        if(glfwGetTime() - timer > 1.0)
        {
            timer++;

            Logger::printText("main", "Synk - Vulkan Engine : FPS = " + std::to_string(frames) + " | TPS = " + std::to_string(ticks));

            frames = 0;
            ticks = 0;
        }
    }

    Logger::exit("_____GAME__ENGINE_____");

    clean();

    return 0;
}
