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

    m_commandPool.freeCommandbuffers(m_commandbuffers);
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
    sceneInfo.name                  = createInfo.name;
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

int Scene::createCommandbuffers()
{
    m_commandbuffers.resize(m_components.pCoreComponents->getSwapchain()->getImageViews().size());

    if (m_commandPool.allocateCommandbuffers(m_commandbuffers) != 0)
    {
        Logger::printError("Scene::createCommandbuffers", "allocateCommandbuffers failed!");

        return 1;
    } else
    {
        Logger::printSuccess("Scene::createCommandbuffers", "allocateCommandbuffers succeed!");
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
            scene->createCommandbuffers() +
            scene->endCreation();
}
