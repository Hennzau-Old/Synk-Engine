#include "core/rendering/Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::clean()
{
    Logger::init("_____CLEAN_RENDER_____");

    m_submit.clean();

    m_commandBuffers.clean();
    m_commandPool.clean();

    m_pipeline.clean();
    m_shader.clean();

    for (auto framebuffer : m_framebuffers)
    {
        framebuffer.clean();
    }

    m_renderPass.clean();

    Logger::exit("_____CLEAN_RENDER_____");
}

void Scene::setData(const SceneCreateInfo& createInfo)
{
    m_components.pCoreComponents    = createInfo.pCoreComponents;

    sceneInfo.renderPassCreateInfo  = createInfo.renderPassCreateInfo;
    sceneInfo.shaderCreateInfo      = createInfo.shaderCreateInfo;
    sceneInfo.pipelineCreateInfo    = createInfo.pipelineCreateInfo;
    sceneInfo.drawFunction          = createInfo.drawFunction;
    sceneInfo.name                  = createInfo.name;
}

void Scene::resize()
{
    m_components.pCoreComponents->getLogicalDevice()->wait();

    for (auto framebuffer : m_framebuffers)
    {
        framebuffer.clean();
    }

    m_commandBuffers.clean();
    m_pipeline.clean();
    m_renderPass.clean();

    m_components.pCoreComponents->getSwapchain()->clean();

    Swapchain::SwapchainCreateInfo swapchainCreateInfo  = {};
    swapchainCreateInfo.pWindow                         = m_components.pCoreComponents->getWindow();
    swapchainCreateInfo.pSurface                        = m_components.pCoreComponents->getSurface();
    swapchainCreateInfo.pPhysicalDevice                 = m_components.pCoreComponents->getPhysicalDevice();
    swapchainCreateInfo.pLogicalDevice                  = m_components.pCoreComponents->getLogicalDevice();
    swapchainCreateInfo.imageUsage                      = m_components.pCoreComponents->getSwapchain()->swapchainInfo.imageUsage;

    if (Swapchain::createSwapchain(m_components.pCoreComponents->getSwapchain(), swapchainCreateInfo) != 0)
    {
        Logger::printError("Scene::resize", "createSwapchain failed!");
    }

    if (createRenderPass() + createPipeline() + createFramebuffers() + createCommandBuffers() != 0)
    {
        Logger::printError("Scene::resize", "reCreateComponents failed!");
    }

    sceneInfo.drawFunction(&m_commandBuffers);
}

void Scene::render()
{
    m_submit.submit(&m_commandBuffers);
}

int Scene::createRenderPass()
{
    sceneInfo.renderPassCreateInfo.pLogicalDevice  = m_components.pCoreComponents->getLogicalDevice();

    if (RenderPass::createRenderPass(&m_renderPass, sceneInfo.renderPassCreateInfo) != 0)
    {
        Logger::printError("Scene::createRenderPass", "createRenderPass failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createRenderPass", "createRenderPass succeed!");
    }

    return 0;
}

int Scene::createFramebuffers()
{
    Logger::init("_____FRAMEBUFFERS_____");

    size_t imageViewCount = m_components.pCoreComponents->getSwapchain()->getImageViews().size();

    m_framebuffers.resize(imageViewCount);

    Framebuffer::FramebufferCreateInfo framebufferCreateInfo  = {};
    framebufferCreateInfo.pLogicalDevice                      = m_components.pCoreComponents->getLogicalDevice();
    framebufferCreateInfo.pSwapchain                          = m_components.pCoreComponents->getSwapchain();
    framebufferCreateInfo.pRenderPass                         = &m_renderPass;

    for (size_t i = 0; i < imageViewCount; i++)
    {
        std::vector<VkImageView> attachments =
        {
            m_components.pCoreComponents->getSwapchain()->getImageViews()[i]
        };

        framebufferCreateInfo.imageViewCount                      = static_cast<uint32_t>(attachments.size());
        framebufferCreateInfo.pImageViews                         = attachments.data();

        if (Framebuffer::createFramebuffer(&m_framebuffers[i], framebufferCreateInfo) != 0)
        {
            Logger::printError("Scene::createFramebuffers", "createFramebuffer[" + std::to_string(i) + "] failed!");

            return 1;
        } else
        {
            Logger::printSuccess("Scene::createFramebuffers", "createFramebuffer[" + std::to_string(i) + "] succeed!");
        }
    }

    Logger::exit("_____FRAMEBUFFERS_____");

    return 0;
}

int Scene::createShader()
{
    sceneInfo.shaderCreateInfo.pLogicalDevice = m_components.pCoreComponents->getLogicalDevice();

    if (Shader::createShader(&m_shader, sceneInfo.shaderCreateInfo) != 0)
    {
        Logger::printError("Scene::createShader", "createShader failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createShader", "createShader succeed!");
    }

    return 0;
}

int Scene::createPipeline()
{
    sceneInfo.pipelineCreateInfo.pLogicalDevice = m_components.pCoreComponents->getLogicalDevice();
    sceneInfo.pipelineCreateInfo.pSwapchain     = m_components.pCoreComponents->getSwapchain();
    sceneInfo.pipelineCreateInfo.pShader        = &m_shader;
    sceneInfo.pipelineCreateInfo.pRenderPass    = &m_renderPass;

    if (Pipeline::createPipeline(&m_pipeline, sceneInfo.pipelineCreateInfo) != 0)
    {
        Logger::printError("Scene::createPipeline", "createPipeline failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createPipeline", "createPipeline succeed!");
    }

    return 0;
}

int Scene::createCommandPool()
{
    CommandPool::CommandPoolCreateInfo commandPoolCreateInfo  = {};
    commandPoolCreateInfo.pPhysicalDevice                     = m_components.pCoreComponents->getPhysicalDevice();
    commandPoolCreateInfo.pLogicalDevice                      = m_components.pCoreComponents->getLogicalDevice();
    commandPoolCreateInfo.queueFamilyIndex                    = m_components.pCoreComponents->getPhysicalDevice()->getQueueFamilies().graphicsFamily.value();

    if (CommandPool::createCommandPool(&m_commandPool, commandPoolCreateInfo) != 0)
    {
        Logger::printError("Scene::createCommandPool", "createCommandPool failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createCommandPool", "createCommandPool succeed!");
    }

    return 0;
}

int Scene::createCommandBuffers()
{
    CommandBuffers::CommandBuffersCreateInfo commandBuffersCreateInfo = {};
    commandBuffersCreateInfo.pFramebuffers                            = &m_framebuffers;
    commandBuffersCreateInfo.pLogicalDevice                           = m_components.pCoreComponents->getLogicalDevice();
    commandBuffersCreateInfo.pSwapchain                               = m_components.pCoreComponents->getSwapchain();
    commandBuffersCreateInfo.pCommandPool                             = &m_commandPool;

    if (CommandBuffers::createCommandBuffers(&m_commandBuffers, commandBuffersCreateInfo) != 0)
    {
        Logger::printError("Scene::createCommandBuffers", "createCommandBuffers failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createCommandBuffers", "createCommandBuffers succeed!");
    }

    return 0;
}

int Scene::createSubmit()
{
    Submit::SubmitCreateInfo submitCreateInfo = {};
    submitCreateInfo.pWindow                  = m_components.pCoreComponents->getWindow();
    submitCreateInfo.pLogicalDevice           = m_components.pCoreComponents->getLogicalDevice();
    submitCreateInfo.pSwapchain               = m_components.pCoreComponents->getSwapchain();
    submitCreateInfo.pScene                   = this;

    if (Submit::createSubmit(&m_submit, submitCreateInfo) != 0)
    {
        Logger::printError("Scene::createSubmit", "createSubmit failed!");
    } else
    {
        Logger::printSuccess("Scene::createSubmit", "createSubmit succeed!");
    }

    return 0;
}

int Scene::endCreation()
{
    Logger::exit("_____SCENE[" + sceneInfo.name + "]_____");

    return 0;
}

int Scene::createScene(Scene* scene, const SceneCreateInfo& createInfo)
{
    scene->setData(createInfo);

    Logger::init("_____SCENE[" + createInfo.name + "]_____");

    return  scene->createRenderPass()     +
            scene->createFramebuffers()   +
            scene->createShader()         +
            scene->createPipeline()       +
            scene->createCommandPool()    +
            scene->createCommandBuffers() +
            scene->createSubmit()         +
            scene->endCreation();
}
