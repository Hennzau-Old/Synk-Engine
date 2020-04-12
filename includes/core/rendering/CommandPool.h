/*=============================================
   Author: Hennzau on Sat Apr 11 12:48:41 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

class CommandPool
{
    private:

        /* private structures */

        struct CommandPoolInfo
        {
            uint32_t              queueFamilyIndex  = 0;
        };

        struct CommandPoolComponents
        {
            PhysicalDevice*       pPhysicalDevice   = nullptr;
            LogicalDevice*        pLogicalDevice    = nullptr;
        } m_components;

    public:

        /* static variables */



        /* structure */

        struct CommandPoolCreateInfo
        {
            PhysicalDevice*       pPhysicalDevice   = nullptr;
            LogicalDevice*        pLogicalDevice    = nullptr;

            uint32_t              queueFamilyIndex  = 0;
        };

        /* functions */

        CommandPool();
        ~CommandPool();

        void                    clean();
        void                    setData(const CommandPoolCreateInfo& createInfo);

        static int              createCommandPool(CommandPool* commandpool, const CommandPoolCreateInfo& createInfo);

        inline VkCommandPool&   getCommandPool() { return m_commandPool; }

        /* variables */

        CommandPoolInfo         commandPoolInfo;

    private:

        /* functions */

        int                     createCommandPool();

        /* variables */

        VkCommandPool           m_commandPool;
};
