#include "graphics/buffer_data/util.hpp"

#include <boost/format.hpp>
#include "graphics/app/app.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"

namespace vg
{
    void createBufferForBufferData(fd::ArrayProxy<MemorySlice> memories
        , uint32_t bufferSize
        , Bool32 isDeviceMemoryLocal
        , vk::BufferUsageFlags targetUsage
        , vk::MemoryPropertyFlags memoryPropertyFlags
        , uint32_t &resultBufferSize
        , std::shared_ptr<vk::Buffer> &resultBuffer
        , uint32_t &resultBufferMemorySize
        , std::shared_ptr<vk::DeviceMemory> &resultBufferMemory
        , void **resultMemoryForHostVisible
        )
    {
        Bool32 isCoherent = (memoryPropertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent) == vk::MemoryPropertyFlagBits::eHostCoherent;
#ifdef DEBUG
        //check arguments
        if (isDeviceMemoryLocal == VG_FALSE && isCoherent == VG_FALSE)
        {
            auto pPhysicalDevice = pApp->getPhysicalDevice();
            auto prop = pPhysicalDevice->getProperties();
            auto nonCoherentAtomSize = prop.limits.nonCoherentAtomSize;

            if (bufferSize % nonCoherentAtomSize != 0)
            {
                throw std::invalid_argument(
                    boost::str(boost::format(
                        "Size of buffer is %1%, which is not a multiple of VkPhysicalDeviceLimits::nonCoherentAtomSize(%2%).") %
                        bufferSize %
                        nonCoherentAtomSize)
                );
            }

            for (const auto &memory : memories)
            {
                if (memory.offset % nonCoherentAtomSize != 0)
                {
                    throw std::invalid_argument(
                        boost::str(boost::format(
                            "Offset of memory slice is %1%, which is not a multiple of VkPhysicalDeviceLimits::nonCoherentAtomSize(%2%).") %
                            memory.offset %
                            nonCoherentAtomSize)
                    );
                }
                if (memory.size % nonCoherentAtomSize != 0)
                {
                    throw std::invalid_argument(
                        boost::str(boost::format(
                            "Size of memory slice is %1%, which is not a multiple of VkPhysicalDeviceLimits::nonCoherentAtomSize(%2%).") %
                            memory.size %
                            nonCoherentAtomSize)
                    );
                }
            }
        }

#endif // DEBUG

        if (isDeviceMemoryLocal == VG_TRUE)
        {
            //create staging buffer.
            vk::BufferCreateInfo createInfo = {
                vk::BufferCreateFlags(),
                bufferSize,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::SharingMode::eExclusive
            };
    
            auto pPhysicalDevice = pApp->getPhysicalDevice();
            auto pDevice = pApp->getDevice();
            auto pStagingBuffer = fd::createBuffer(pDevice, createInfo);
            vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*pStagingBuffer);
            vk::MemoryAllocateInfo allocateInfo = {
                memReqs.size,
                vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible)
            };
    
            auto pStagingBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
    
            pDevice->bindBufferMemory(*pStagingBuffer, *pStagingBufferMemory, 0u);
    
            void* data;
            pDevice->mapMemory(*pStagingBufferMemory, 0u, static_cast<vk::DeviceSize>(bufferSize), vk::MemoryMapFlags(), &data);
            uint32_t count = memories.size();
            uint32_t offset = 0;
            uint32_t size = 0;
            std::vector<vk::MappedMemoryRange> ranges(count);
            for (uint32_t i = 0; i < count; ++i) {
                offset = (*(memories.data() + i)).offset;
                size = (*(memories.data() + i)).size;
                memcpy(((char*)data + offset), (*(memories.data() + i)).pMemory, size);
                ranges[i].memory = *pStagingBufferMemory;
                ranges[i].offset = offset;
                ranges[i].size = size;
            }
            if (count)
            {
                pDevice->flushMappedMemoryRanges(ranges);                
            }
            pDevice->unmapMemory(*pStagingBufferMemory);
    
            //create vertex buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (resultBufferSize < bufferSize) {
                resultBufferSize = bufferSize;
                createInfo.usage = vk::BufferUsageFlagBits::eTransferDst | targetUsage;
                resultBuffer = fd::createBuffer(pDevice, createInfo);
                memReqs = pDevice->getBufferMemoryRequirements(*resultBuffer);
                resultBufferMemorySize = static_cast<uint32_t>(memReqs.size);        
                allocateInfo.allocationSize = memReqs.size;
                allocateInfo.memoryTypeIndex = vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, memoryPropertyFlags);
                resultBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
                pDevice->bindBufferMemory(*resultBuffer, *resultBufferMemory, 0u);
            }
            
            {
                uint32_t offset = 0u;            
                //copy buffer from staging buffer to vertex buffer.
                auto pCommandBuffer = beginSingleTimeCommands();

                std::vector<vk::BufferCopy> regions(count);
                for (uint32_t i = 0; i < count; ++i)
                {
                    regions[i].dstOffset = ranges[i].offset;
                    regions[i].srcOffset = ranges[i].offset;                    
                    regions[i].size = ranges[i].size;
                }

                pCommandBuffer->copyBuffer(*pStagingBuffer, *resultBuffer, regions);
    
                endSingleTimeCommands(pCommandBuffer);
            }
        }
        else
        {
             //create vertex buffer
            // if old buffer size is same as required buffer size, we don't to create a new buffer for it.
            if (resultBufferSize < bufferSize)
            {
                resultBufferSize = bufferSize;
                //create staging buffer.
                vk::BufferCreateInfo createInfo = {
                    vk::BufferCreateFlags(),
                    bufferSize,
                    targetUsage,
                    vk::SharingMode::eExclusive
                };
        
                auto pPhysicalDevice = pApp->getPhysicalDevice();
                auto pDevice = pApp->getDevice();
                resultBuffer = fd::createBuffer(pDevice, createInfo);
                vk::MemoryRequirements memReqs = pDevice->getBufferMemoryRequirements(*resultBuffer);
                resultBufferMemorySize = static_cast<uint32_t>(memReqs.size);  
                vk::MemoryAllocateInfo allocateInfo = {
                    memReqs.size,
                    vg::findMemoryType(pPhysicalDevice, memReqs.memoryTypeBits, memoryPropertyFlags)
                };
        
                resultBufferMemory = fd::allocateMemory(pDevice, allocateInfo);
        
                pDevice->bindBufferMemory(*resultBuffer, *resultBufferMemory, 0u);
        
                pDevice->mapMemory(*resultBufferMemory, 0u, static_cast<vk::DeviceSize>(bufferSize), vk::MemoryMapFlags(), resultMemoryForHostVisible);
            }
            uint32_t count = memories.size();
            uint32_t offset = 0;
            uint32_t size = 0;
            std::vector<vk::MappedMemoryRange> ranges(count);
            for (uint32_t i = 0; i < count; ++i) {
                offset = (*(memories.data() + i)).offset;
                size = (*(memories.data() + i)).size;
                memcpy(((char*)(*resultMemoryForHostVisible) + offset), (*(memories.data() + i)).pMemory, size);
                ranges[i].memory = *resultBufferMemory;
                ranges[i].offset = offset;
                ranges[i].size = size;
            }
            if (isCoherent == VG_FALSE)
            {
                auto pDevice = pApp->getDevice();
                if (count)
                {
                    pDevice->flushMappedMemoryRanges(ranges);
                }
            }
        }
    }

    void vertexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, const VertexData *pVertexData, uint32_t subIndex)
    {
        const auto &subVertexDatas = pVertexData->getSubVertexDatas();
        const auto &subVertexData = subVertexDatas[subIndex];
        uint32_t bindingDescCount = subVertexData.vertexInputStateInfo.vertexBindingDescriptionCount;
        const auto &bindingDescs = subVertexData.vertexInputStateInfo.pVertexBindingDescriptions;
        std::vector<vk::Buffer> vertexBuffers(bindingDescCount);
        std::vector<vk::DeviceSize> offsets(bindingDescCount);
        uint32_t offset = 0u;        
        for (uint32_t i = 0; i < subIndex; ++i) {
            offset += subVertexDatas[i].bufferSize;
        }

        uint32_t count = static_cast<uint32_t>(bindingDescCount);
        for (uint32_t i = 0; i < count; ++i) {
            vertexBuffers[i] = *(pVertexData->getBufferData().getBuffer());
            offsets[i] = offset;
            offset += bindingDescs[i].stride * subVertexData.vertexCount;
        }
        
        commandBuffer.bindVertexBuffers(0u, vertexBuffers, offsets);
    }

    void indexDataToCommandBuffer(vk::CommandBuffer &commandBuffer, 
        const IndexData *pIndexData, 
        uint32_t subIndex
        )
    {
        uint32_t offset = 0u;
        const auto &subIndexDatas = pIndexData->getSubIndexDatas();
        for (uint32_t i = 0; i < subIndex; ++i)
        {
            offset += subIndexDatas[i].bufferSize;
        }
        commandBuffer.bindIndexBuffer(*(pIndexData->getBufferData().getBuffer()), static_cast<vk::DeviceSize>(offset), subIndexDatas[subIndex].indexType);
    }
}