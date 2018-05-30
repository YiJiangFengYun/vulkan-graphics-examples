#include "graphics/util/single_time_command.hpp"
#include "graphics/app/app.hpp"

namespace vg
{
    std::shared_ptr<vk::CommandBuffer> beginSingleTimeCommands() {
        auto pDevice = pApp->getDevice();
        auto pCommandPool = pApp->getCommandPoolForTransientBuffer();
        vk::CommandBufferAllocateInfo allocateInfo = {
            *pCommandPool,
            vk::CommandBufferLevel::ePrimary,
            uint32_t(1)
        };

        VG_LOG(plog::debug) << "Pre allocate command buffer from pool." << std::endl;
        auto pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool, allocateInfo);
        VG_LOG(plog::debug) << "Post allocate command buffer from pool." << std::endl;

        vk::CommandBufferBeginInfo beginInfo = {
            vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
        };
        pCommandBuffer->begin(beginInfo);

        return pCommandBuffer;
    }

    void endSingleTimeCommands(const std::shared_ptr<vk::CommandBuffer> &pCommandBuffer) {
        auto pDevice = pApp->getDevice();
        vk::Queue queue;
        uint32_t queueIndex;
        pApp->allocateGaphicsQueue(queueIndex, queue);
        auto commandPool = pApp->getCommandPoolForTransientBuffer();
        pCommandBuffer->end();
        vk::SubmitInfo submitInfo = { 0, nullptr, nullptr, 1, pCommandBuffer.get(), 0, nullptr };
        queue.submit(submitInfo, nullptr);
        queue.waitIdle();

        pApp->freeGraphicsQueue(queueIndex);
    }
} //namespace kgs
