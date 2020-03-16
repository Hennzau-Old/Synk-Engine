#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/CoreComponents.h"
#include "core/rendering/RenderPass.h"
#include "core/rendering/Framebuffer.h"
#include "core/rendering/Shader.h"

class Scene
{
    private:

        /* private structures */

        struct SceneInfo
        {
            RenderPass::RenderPassCreateInfo  renderPassCreateInfo;
            Shader::ShaderCreateInfo          shaderCreateInfo;

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

            std::string                       name;
        };

        /* functions */

        Scene();
        ~Scene();

        void                      clean();
        void                      setData(const SceneCreateInfo& createInfo);

        static int                createScene(Scene* scene, const SceneCreateInfo& createInfo);

        inline RenderPass&                getRenderPass   () { return m_renderPass;   }
        inline std::vector<Framebuffer>&  getFramebuffers () { return m_framebuffers; }
        inline Shader&                    getShader       () { return m_shader;       }

        /* variables */

        SceneInfo                 sceneInfo;

    private:

        /* functions */

        int                       createRenderPass();
        int                       createFramebuffers();
        int                       createShader();
        int                       endCreation();

        /* variables */

        RenderPass                m_renderPass;
        std::vector<Framebuffer>  m_framebuffers;
        Shader                    m_shader;
};
