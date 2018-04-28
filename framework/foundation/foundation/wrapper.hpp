#ifndef FD_VK_WRAPPER_H
#define FD_VK_WRAPPER_H

#include <memory>
#include <vulkan/vulkan.hpp>

namespace fd
{
	extern std::shared_ptr<vk::Instance> createInstance(const vk::InstanceCreateInfo & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Device> createDevice(vk::PhysicalDevice *pPhysicalDevice,
		const vk::DeviceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::SwapchainKHR> createSwapchainKHR(vk::Device *pDevice,
		const vk::SwapchainCreateInfoKHR & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ImageView> createImageView(vk::Device *pDevice,
		const vk::ImageViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandPool> createCommandPool(vk::Device *pDevice,
		const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::RenderPass> createRenderPass(vk::Device *pDevice,
		const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Buffer> createBuffer(vk::Device *pDevice,
		const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Image> createImage(vk::Device *pDevice,
		const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DeviceMemory> allocateMemory(vk::Device *pDevice,
		const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Framebuffer> createFrameBuffer(vk::Device *pDevice,
		const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Semaphore> createSemaphore(vk::Device *pDevice,
		const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandBuffer> allocateCommandBuffer(vk::Device *pDevice,
		vk::CommandPool *pCommandPool,
		vk::CommandBufferAllocateInfo & allocateInfo);

	extern std::shared_ptr<vk::Sampler> createSampler(vk::Device *pDevice,
		const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ShaderModule> createShaderModule(vk::Device *pDevice,
		const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorSetLayout> createDescriptorSetLayout(vk::Device *pDevice,
		const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorPool> createDescriptorPool(vk::Device *pDevice,
		const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional < const vk::AllocationCallbacks > allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorSet> allocateDescriptorSet(vk::Device *pDevice,
		vk::DescriptorPool *pDescriptorPool, vk::DescriptorSetAllocateInfo & allocateInfo);

	extern std::shared_ptr<vk::PipelineLayout> createPipelineLayout(vk::Device *pDevice,
		const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Pipeline> createGraphicsPipeline(vk::Device *pDevice,
		vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo, 
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::PipelineCache> createPipelineCache(vk::Device *pDevice,
	    const vk::PipelineCacheCreateInfo &createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Fence> createFence(vk::Device *pDevice,
		const vk::FenceCreateInfo &createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);
}

#endif // !FD_VK_WRAPPER_H
