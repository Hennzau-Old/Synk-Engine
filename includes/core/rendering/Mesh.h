/*=============================================
   Author: Hennzau on Sat Apr 11 16:07:14 2020
  =============================================*/ 

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/PhysicalDevice.h"
#include "core/LogicalDevice.h"

#include "core/rendering/CommandPool.h"
#include "core/rendering/buffers/Buffer.h"

class Mesh
{
    private:

        /* private structures */

        struct MeshInfo
        {
            std::vector<float>      vertices;
            std::vector<uint16_t>   indices;
        };

        struct MeshComponents
        {
            PhysicalDevice*         pPhysicalDevice = nullptr;
            LogicalDevice*          pLogicalDevice  = nullptr;
            CommandPool*            pCommandPool    = nullptr;
        } m_components;

    public:

        /* static variables */



        /* structure */

        struct MeshCreateInfo
        {
            PhysicalDevice*         pPhysicalDevice = nullptr;
            LogicalDevice*          pLogicalDevice  = nullptr;
            CommandPool*            pCommandPool    = nullptr;

            std::vector<float>      vertices;
            std::vector<uint16_t>   indices;
        };

        /* functions */

        Mesh();
        ~Mesh();

        void                    clean();
        void                    setData(const MeshCreateInfo& createInfo);

        static int              createMesh(Mesh* mesh, const MeshCreateInfo& createInfo);

        inline Buffer&          getVertexBuffer () { return m_vertexBuffer; }
        inline Buffer&          getIndexBuffer  () { return m_indexBuffer;  }

        /* variables */

        MeshInfo                meshInfo;

    private:

        /* functions */

        int                     createVertexBuffer();
        int                     createIndexBuffer();
        int                     createMesh();

        /* variables */

        Buffer                  m_vertexBuffer;
        Buffer                  m_indexBuffer;

};
