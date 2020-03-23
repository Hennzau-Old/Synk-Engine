#include <iostream>

#include "core/utils/File.h"

#include "core/CoreComponents.h"
#include "core/rendering/Scene.h"
#include "core/rendering/Mesh.h"

const int 				FRAME_CAP = 6666;
const int 				TICK_CAP = 60;

CoreComponents  coreComponents;
Scene           scene;

Mesh test;

void render(CommandBuffers* commandBuffers)
{
    CommandPool transferCommandPool;

    CommandPool::CommandPoolCreateInfo transferCommandPoolcreateInfo  = {};
    transferCommandPoolcreateInfo.pPhysicalDevice                     = coreComponents.getPhysicalDevice();
    transferCommandPoolcreateInfo.pLogicalDevice                      = coreComponents.getLogicalDevice();
    transferCommandPoolcreateInfo.queueFamilyIndex                    = coreComponents.getPhysicalDevice()->getQueueFamilies().transferFamily.value();

    if (CommandPool::createCommandPool(&transferCommandPool, transferCommandPoolcreateInfo) != 0)
    {
        Logger::printError("adazdazdazda", "azudadhaidhaidaidhai");
    }

    std::vector<float> vertices =
    {
        -1.0f, +0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        +0.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
        +0.0f, +0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,

        +1.0f, +0.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
        -0.0f, +1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        +1.0f, +1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
        +0.0f, +0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,
    };

    std::vector<uint16_t> indices =
    {
        0, 1, 2,
        0, 3, 1,

        4, 5, 6,
        4, 7, 5,
    };

    Mesh::MeshCreateInfo meshCreateInfo = {};
    meshCreateInfo.pPhysicalDevice      = coreComponents.getPhysicalDevice();
    meshCreateInfo.pLogicalDevice       = coreComponents.getLogicalDevice();
    meshCreateInfo.pCommandPool         = &transferCommandPool;
    meshCreateInfo.vertices             = vertices;
    meshCreateInfo.indices              = indices;

    if (Mesh::createMesh(&test, meshCreateInfo) != 0)
    {
        Logger::printError("Cazdj", "izauda");
    }

    transferCommandPool.clean();

    std::vector<VkBuffer> vertexBuffers =
    {
        test.getVertexBuffer().getBuffer()
    };

    for (size_t i = 0; i < coreComponents.getSwapchain()->getImageViews().size(); i++)
    {
        commandBuffers->beginCommandBuffers(i);
            commandBuffers->beginRenderPass(i, scene.getRenderPass());

                commandBuffers->bindPipeline(i, scene.getPipeline());

                commandBuffers->bindVertexBuffer(i, vertexBuffers);
                commandBuffers->bindIndexBuffer(i, test.getIndexBuffer().getBuffer());
                commandBuffers->draw(i, 12);

            commandBuffers->endRenderPass(i);
        commandBuffers->endCommandBuffers(i);
    }
}

void init()
{
    Window::WindowCreateInfo windowCreateInfo = {};
    windowCreateInfo.initialWidth             = 1280;
    windowCreateInfo.initialHeight            = 720;
    windowCreateInfo.resizable                = GLFW_FALSE;
    windowCreateInfo.title                    = "Synk Vulkan Engine";

    CoreComponents::CoreComponentsCreateInfo coreComponentsCreateInfo = {};
    coreComponentsCreateInfo.windowCreateInfo                         = windowCreateInfo;
    coreComponentsCreateInfo.imageUsage                               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (CoreComponents::createCoreComponents(&coreComponents, coreComponentsCreateInfo) != 0)
    {
        Logger::printError("main::init", "createCoreComponents failed!");
    }

    RenderPass::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.format                           = coreComponents.getSwapchain()->getImageFormat();
    renderPassCreateInfo.samples                          = VK_SAMPLE_COUNT_1_BIT;
    renderPassCreateInfo.loadOp                           = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassCreateInfo.storeOp                          = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassCreateInfo.stencilLoadOp                    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassCreateInfo.stencilStoreOp                   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassCreateInfo.initialLayout                    = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassCreateInfo.finalLayout                      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    Shader::ShaderCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.vertexShaderCode         = File::readFile("res/shaders/spir-v/main.vert");
    shaderCreateInfo.fragmentShaderCode       = File::readFile("res/shaders/spir-v/main.frag");


    /*vertex binding */

    VkVertexInputBindingDescription vertexBindingDescription  = {};
    vertexBindingDescription.binding                          = 0;
    vertexBindingDescription.stride                           = 7 * sizeof(float);
    vertexBindingDescription.inputRate                        = VK_VERTEX_INPUT_RATE_VERTEX;

    /* attributs description */

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding                                          = 0;
    attributeDescriptions[0].location                                         = 0;
    attributeDescriptions[0].format                                           = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset                                           = 0.0f;     //offset of vertex positions

    attributeDescriptions[1].binding                                          = 0;
    attributeDescriptions[1].location                                         = 1;
    attributeDescriptions[1].format                                           = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset                                           = 3 * sizeof(float);     //offset of colors

    /* shaders */

    Pipeline::RasterizationInfo rasterizationInfo = {};
    rasterizationInfo.polygonMode                 = VK_POLYGON_MODE_FILL;
    rasterizationInfo.cullMode                    = VK_CULL_MODE_FRONT_BIT;
    rasterizationInfo.frontFace                   = VK_FRONT_FACE_CLOCKWISE;
    rasterizationInfo.lineWidth                   = 1.0f;

    Pipeline::VertexInputInfo vertexInputInfo   = {};
    vertexInputInfo.vertexBindingDescription    = vertexBindingDescription;
    vertexInputInfo.vertexAttributeDescriptions = attributeDescriptions;

    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings(1);

    /* UBO */

    descriptorSetLayoutBindings[0].binding            = 0;
    descriptorSetLayoutBindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorSetLayoutBindings[0].descriptorCount    = 1;
    descriptorSetLayoutBindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    descriptorSetLayoutBindings[0].pImmutableSamplers = nullptr;

    Pipeline::DescriptorSetLayoutInfo descriptorsInfo = {};
    descriptorsInfo.descriptors                       = descriptorSetLayoutBindings;

    Pipeline::PipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.rasterizationInfo            = rasterizationInfo;
    pipelineCreateInfo.vertexInputInfo              = vertexInputInfo;
    pipelineCreateInfo.descriptorsInfo              = descriptorsInfo;

    /* scene */

    Scene::SceneCreateInfo sceneCreateInfo  = {};
    sceneCreateInfo.pCoreComponents         = &coreComponents;
    sceneCreateInfo.renderPassCreateInfo    = renderPassCreateInfo;
    sceneCreateInfo.shaderCreateInfo        = shaderCreateInfo;
    sceneCreateInfo.pipelineCreateInfo      = pipelineCreateInfo;
    sceneCreateInfo.drawFunction            = render;
    sceneCreateInfo.name                    = "main";

    if (Scene::createScene(&scene, sceneCreateInfo) != 0)
    {
        Logger::printError("main::init", "createScene failed!");
    }

    render(scene.getCommandBuffers());
}

void update()
{
    coreComponents.getWindow()->update();
}

void render()
{
    scene.render();
}

void clean()
{
    coreComponents.getLogicalDevice()->wait();
    
    test.clean();

    scene.clean();
    coreComponents.clean();
}

int main()
{
    init();

    Logger::init("_____GAME__ENGINE_____");

    double  tickTime    = 1.0 / TICK_CAP;
    double  renderTime  = 1.0 / FRAME_CAP;

    double  lastTickTime    = glfwGetTime();
    double  timer = lastTickTime;
    double  lastRenderTime  = glfwGetTime();
    double  deltaTime       = 0,
            nowTime         = 0;

    int     frames          = 0,
            ticks           = 0;

    while(!coreComponents.getWindow()->isClosed())
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
