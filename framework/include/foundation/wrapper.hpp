#ifndef FD_VK_WRAPPER_H
#define FD_VK_WRAPPER_H

#include <memory>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace fd
{
	extern std::shared_ptr<vk::Instance> createInstance(const vk::InstanceCreateInfo & createInfo, 
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Device> createDevice(const std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		const vk::DeviceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<GLFWwindow> createGLFWWindow(uint32_t width, uint32_t height, const char* title);

	extern std::shared_ptr<vk::SurfaceKHR> createSurface(std::shared_ptr<vk::Instance> pInstance,
		std::shared_ptr<GLFWwindow> pWindow);

	extern std::shared_ptr<vk::SwapchainKHR> createSwapchainKHR(const std::shared_ptr<vk::Device> pDevice,
		const vk::SwapchainCreateInfoKHR & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ImageView> createImageView(const std::shared_ptr<vk::Device> pDevice,
		const vk::ImageViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandPool> createCommandPool(const std::shared_ptr<vk::Device> pDevice,
		const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::RenderPass> createRenderPass(const std::shared_ptr<vk::Device> pDevice,
		const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Buffer> createBuffer(const std::shared_ptr<vk::Device> pDevice,
		const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Image> createImage(const std::shared_ptr<vk::Device> pDevice,
		const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::DeviceMemory> allocateMemory(const std::shared_ptr<vk::Device> pDevice,
		const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Framebuffer> createFrameBuffer(const std::shared_ptr<vk::Device> pDevice,
		const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::Semaphore> createSemaphore(const std::shared_ptr<vk::Device> pDevice,
		const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::CommandBuffer> allocateCommandBuffer(const std::shared_ptr<vk::Device> pDevice,
		const std::shared_ptr<vk::CommandPool> pCommandPool,
		vk::CommandBufferAllocateInfo & allocateInfo);

	extern std::shared_ptr<vk::Sampler> createSampler(const std::shared_ptr<vk::Device> pDevice,
		const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);

	extern std::shared_ptr<vk::ShaderModule> createShaderModule(const std::shared_ptr<vk::Device> pDevice,
		const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr);
}

#endif // !FD_VK_WRAPPER_H
