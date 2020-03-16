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

int Scene::endCreation()
{
    Logger::exit("_____SCENE[" + sceneInfo.name + "]_____");

    return 0;
}

int Scene::createScene(Scene* scene, const SceneCreateInfo& createInfo)
{
    scene->setData(createInfo);

    Logger::init("_____SCENE[" + createInfo.name + "]_____");

    return  scene->createRenderPass()   +
            scene->createFramebuffers() +
            scene->createShader()       +
            scene->endCreation();
}
