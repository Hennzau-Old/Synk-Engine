/*=============================================
   Author: Hennzau on Sat Apr 11 13:51:57 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"
#include "core/rendering/CommandPool.h"

class Buffer
{
    private:

        /* private structures */

        struct BufferInfo
        {
            VkDeviceSize            size;
            VkBufferUsageFlags      usage;
            VkMemoryPropertyFlags   properties;
        };

        struct BufferComponents
        {
            PhysicalDevice*         pPhysicalDevice   = nullptr;
            LogicalDevice*          pLogicalDevice    = nullptr;
            CommandPool*            pCommandPool      = nullptr;
        }   m_components;

    public:

        /* static variables */



        /* structure */

        struct BufferCreateInfo
        {
            PhysicalDevice*         pPhysicalDevice   = nullptr;
            LogicalDevice*          pLogicalDevice    = nullptr;
            CommandPool*            pCommandPool      = nullptr;

            VkDeviceSize            size;
            VkBufferUsageFlags      usage;
            VkMemoryPropertyFlags   properties;
        };

        /* functions */

        Buffer();
        ~Buffer();

        void                        clean();
        void                        setData(const BufferCreateInfo& createInfo);

        template<typename T>
        void                        copyData(T data)
        {
            void* memData;

            vkMapMemory(m_components.pLogicalDevice->getLogicalDevice(), m_bufferMemory, 0, bufferInfo.size, 0, &memData);

                memcpy(memData, data, (size_t) bufferInfo.size);

            vkUnmapMemory(m_components.pLogicalDevice->getLogicalDevice(), m_bufferMemory);
        }


        void                        copyToBuffer(Buffer* dstBuffer);

        static int                  createBuffer(Buffer* buffer, const BufferCreateInfo& createInfo);

        inline VkBuffer             getBuffer       () { return m_buffer; }
        inline VkDeviceMemory       getBufferMemory () { return m_bufferMemory; }

        /* variables */

        BufferInfo                  bufferInfo;

    private:

        /* functions */

        int                         createBuffer();
        uint32_t                    findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        /* variables */

        VkBuffer                    m_buffer;
        VkDeviceMemory              m_bufferMemory;
};
