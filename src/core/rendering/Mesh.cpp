#include "core/rendering/Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::clean()
{
    m_vertexBuffer.clean();
    m_indexBuffer.clean();
}

void Mesh::setData(const MeshCreateInfo& createInfo)
{
    m_components.pPhysicalDevice  = createInfo.pPhysicalDevice;
    m_components.pLogicalDevice   = createInfo.pLogicalDevice;
    m_components.pCommandPool     = createInfo.pCommandPool;

    meshInfo.vertices             = createInfo.vertices;
    meshInfo.indices              = createInfo.indices;
}

int Mesh::createVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(meshInfo.vertices[0]) * meshInfo.vertices.size();

    Buffer stagingBuffer;

    Buffer::BufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.pPhysicalDevice          = m_components.pPhysicalDevice;
    stagingBufferCreateInfo.pLogicalDevice           = m_components.pLogicalDevice;
    stagingBufferCreateInfo.pCommandPool             = m_components.pCommandPool;
    stagingBufferCreateInfo.size                     = bufferSize;
    stagingBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (Buffer::createBuffer(&stagingBuffer, stagingBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createVertexBuffer", "createBuffer[stagingBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyData(meshInfo.vertices);

    Buffer::BufferCreateInfo vertexBufferCreateInfo = stagingBufferCreateInfo;
    vertexBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vertexBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (Buffer::createBuffer(&m_vertexBuffer, vertexBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createVertexBuffer", "createBuffer[vertexBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyToBuffer(&m_vertexBuffer);

    stagingBuffer.clean();

    return 0;
}

int Mesh::createIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(meshInfo.indices[0]) * meshInfo.indices.size();

    Buffer stagingBuffer;

    Buffer::BufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.pPhysicalDevice          = m_components.pPhysicalDevice;
    stagingBufferCreateInfo.pLogicalDevice           = m_components.pLogicalDevice;
    stagingBufferCreateInfo.pCommandPool             = m_components.pCommandPool;
    stagingBufferCreateInfo.size                     = bufferSize;
    stagingBufferCreateInfo.usage                    = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.properties               = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (Buffer::createBuffer(&stagingBuffer, stagingBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createIndexBuffer", "createBuffer[stagingBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyData(meshInfo.indices);

    Buffer::BufferCreateInfo indexBufferCreateInfo  = stagingBufferCreateInfo;
    indexBufferCreateInfo.usage                     = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    indexBufferCreateInfo.properties                = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (Buffer::createBuffer(&m_indexBuffer, indexBufferCreateInfo) != 0)
    {
        Logger::printError("Mesh::createIndexBuffer", "createBuffer[indexBuffer] failed!");

        return 1;
    }

    stagingBuffer.copyToBuffer(&m_indexBuffer);

    stagingBuffer.clean();

    return 0;
}

int Mesh::createMesh(Mesh* mesh, const MeshCreateInfo& createInfo)
{
    mesh->setData(createInfo);

    return  mesh->createVertexBuffer()  +
            mesh->createIndexBuffer();
}
