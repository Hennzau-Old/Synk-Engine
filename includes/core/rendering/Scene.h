#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/CoreComponents.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/Shader.h"
#include "core/rendering/Pipeline.h"
#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/CommandBuffers.h"
#include "core/rendering/Submit.h"

class Scene
{
    public:

        /* static variables */

        typedef void (*FnPtr) (CommandBuffers* commandBuffers);

    private:

        /* private structures */

        struct SceneInfo
        {
            RenderPass::RenderPassCreateInfo  renderPassCreateInfo;
            Shader::ShaderCreateInfo          shaderCreateInfo;
            Pipeline::PipelineCreateInfo      pipelineCreateInfo;

            FnPtr                             drawFunction;

            std::string                       name;
        };

        struct SceneComponents
        {
            CoreComponents*       pCoreComponents;

        } m_components;

    public:

        /* static variables */

        /* structure */

        struct SceneCreateInfo
        {
            CoreComponents*                   pCoreComponents;

            RenderPass::RenderPassCreateInfo  renderPassCreateInfo;
            Shader::ShaderCreateInfo          shaderCreateInfo;
            Pipeline::PipelineCreateInfo      pipelineCreateInfo;

            FnPtr                             drawFunction;

            std::string                       name;
        };

        /* functions */

        Scene();
        ~Scene();

        void                                clean();
        void                                setData(const SceneCreateInfo& createInfo);

        void                                resize();

        void                                render();

        static int                          createScene(Scene* scene, const SceneCreateInfo& createInfo);

        inline RenderPass*                  getRenderPass     () { return &m_renderPass;      }
        inline std::vector<Framebuffer>*    getFramebuffers   () { return &m_framebuffers;    }
        inline Shader*                      getShader         () { return &m_shader;          }
        inline Pipeline*                    getPipeline       () { return &m_pipeline;        }
        inline CommandPool*                 getCommandPool    () { return &m_commandPool;     }
        inline CommandBuffers*              getCommandBuffers () { return &m_commandBuffers;  }
        inline Submit*                      getSubmit         () { return &m_submit;          }

        /* variables */

        SceneInfo                           sceneInfo;

    private:

        /* functions */

        int                                 createRenderPass();
        int                                 createFramebuffers();
        int                                 createShader();
        int                                 createPipeline();
        int                                 createCommandPool();
        int                                 createCommandBuffers();
        int                                 createSubmit();
        int                                 endCreation();

        /* variables */

        RenderPass                  m_renderPass;
        std::vector<Framebuffer>    m_framebuffers;
        Shader                      m_shader;
        Pipeline                    m_pipeline;
        CommandPool                 m_commandPool;
        CommandBuffers              m_commandBuffers;
        Submit                      m_submit;
};
