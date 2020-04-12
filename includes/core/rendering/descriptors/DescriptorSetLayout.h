/*=============================================
   Author: Hennzau on Sun Apr 12 15:54:52 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/LogicalDevice.h"

class DescriptorSetLayout
{
    private:

        /* private structures */

        struct DescriptorSetLayoutInfo
        {
            uint32_t                                    bindingCount;
            VkDescriptorSetLayoutBinding*               pBindings;
        };

        struct DescriptorSetLayoutComponents
        {
            LogicalDevice*                              pLogicalDevice;
        } m_components;

    public:

        /* static variables */



        /* structure */

        struct DescriptorSetLayoutCreateInfo
        {
            LogicalDevice*                              pLogicalDevice;

            uint32_t                                    bindingCount;
            VkDescriptorSetLayoutBinding*               pBindings;
        };

        /* functions */

        DescriptorSetLayout();
        ~DescriptorSetLayout();

        void                    clean();
        void                    setData(const DescriptorSetLayoutCreateInfo& createInfo);

        static int              createDescriptorSetLayout(DescriptorSetLayout* descriptorSetLayout, const DescriptorSetLayoutCreateInfo& createInfo);

        inline VkDescriptorSetLayout&   getDescriptorSetLayout() { return m_descriptorSetLayout; }

        /* variables */

        DescriptorSetLayoutInfo descriptorSetLayoutInfo;

    private:

        /* functions */

        int                     createDescriptorSetLayout();

        /* variables */

        VkDescriptorSetLayout   m_descriptorSetLayout;
};
