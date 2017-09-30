#include "graphics/util/single_time_command.hpp"

namespace kgs
{
	std::shared_ptr<vk::CommandBuffer> beginSingleTimeCommands() {
		auto pDevice = pContext->getNativeDevice();
		auto pCommandPool = pContext->getCommandPool();
		vk::CommandBufferAllocateInfo allocateInfo = {
			*pCommandPool,
			vk::CommandBufferLevel::ePrimary,
			uint32_t(1)
		};

		auto pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool, allocateInfo);

		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
		};
		pCommandBuffer->begin(beginInfo);

		return pCommandBuffer;
	}

	void endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer) {
		auto pDevice = pContext->getNativeDevice();
		auto graphicsQueue = pContext->getGraphicsQueue();
		auto commandPool = pContext->getCommandPool();
		pCommandBuffer->end();
		vk::SubmitInfo submitInfo = { 0, nullptr, nullptr, 1, pCommandBuffer.get(), 0, nullptr };
		graphicsQueue.submit(submitInfo, nullptr);
		graphicsQueue.waitIdle();
	}
} //namespace kgs
