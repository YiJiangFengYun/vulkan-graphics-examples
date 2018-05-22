#ifndef FD_VK_WRAPPER_H
#define FD_VK_WRAPPER_H

#include <memory>
#include <vulkan/vulkan.hpp>

namespace fd
{
	extern std::shared_ptr<vk::Instance> createInstance(const vk::InstanceCreateInfo & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Device> createDevice(const vk::PhysicalDevice *pPhysicalDevice,
		const vk::DeviceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::SwapchainKHR> createSwapchainKHR(const vk::Device *pDevice,
		const vk::SwapchainCreateInfoKHR & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ImageView> createImageView(const vk::Device *pDevice,
		const vk::ImageViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandPool> createCommandPool(const vk::Device *pDevice,
		const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::RenderPass> createRenderPass(const vk::Device *pDevice,
		const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Buffer> createBuffer(const vk::Device *pDevice,
		const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Image> createImage(const vk::Device *pDevice,
		const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DeviceMemory> allocateMemory(const vk::Device *pDevice,
		const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Framebuffer> createFrameBuffer(const vk::Device *pDevice,
		const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Semaphore> createSemaphore(const vk::Device *pDevice,
		const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandBuffer> allocateCommandBuffer(const vk::Device *pDevice,
		const vk::CommandPool *pCommandPool,
		const vk::CommandBufferAllocateInfo & allocateInfo);

	extern std::shared_ptr<vk::Sampler> createSampler(const vk::Device *pDevice,
		const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ShaderModule> createShaderModule(const vk::Device *pDevice,
		const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorSetLayout> createDescriptorSetLayout(const vk::Device *pDevice,
		const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorPool> createDescriptorPool(const vk::Device *pDevice,
		const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional <const vk::AllocationCallbacks > allocator = nullptr);

	extern std::shared_ptr<vk::DescriptorSet> allocateDescriptorSet(const vk::Device *pDevice,
		const vk::DescriptorPool *pDescriptorPool, const vk::DescriptorSetAllocateInfo & allocateInfo);

	extern std::shared_ptr<vk::PipelineLayout> createPipelineLayout(const vk::Device *pDevice,
		const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Pipeline> createGraphicsPipeline(const vk::Device *pDevice,
		const vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo, 
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::PipelineCache> createPipelineCache(const vk::Device *pDevice,
	    const vk::PipelineCacheCreateInfo &createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Fence> createFence(const vk::Device *pDevice,
		const vk::FenceCreateInfo &createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);
}

#endif // !FD_VK_WRAPPER_H
