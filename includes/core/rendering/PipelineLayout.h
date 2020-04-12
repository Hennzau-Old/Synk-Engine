/*=============================================
   Author: Hennzau on Sat Apr 11 11:06:41 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class PipelineLayout
{
    private:

        /* private structures */

        struct PipelineLayoutInfo
        {
            uint32_t                descriptorSetLayoutCount;
            VkDescriptorSetLayout*  pDescriptorSetLayout;
        };

        struct PipelineLayoutComponents
        {
            LogicalDevice*          pLogicalDevice  = nullptr;
        } m_components;

    public:

        /* static variables */



        /* structure */

        struct PipelineLayoutCreateInfo
        {
            LogicalDevice*          pLogicalDevice  = nullptr;

            uint32_t                descriptorSetLayoutCount;
            VkDescriptorSetLayout*  pDescriptorSetLayout;
        };

        /* functions */

        PipelineLayout();
        ~PipelineLayout();

        void                        clean();
        void                        setData(const PipelineLayoutCreateInfo& createInfo);

        static int                  createPipelineLayout(PipelineLayout* pipelinelayout, const PipelineLayoutCreateInfo& createInfo);

        inline VkPipelineLayout&    getPipelineLayout() { return m_pipelineLayout; }

        /* variables */

        PipelineLayoutInfo          pipelineLayoutInfo;

    private:

        /* functions */

        int                         createPipelineLayout();

        /* variables */

        VkPipelineLayout            m_pipelineLayout;
};
