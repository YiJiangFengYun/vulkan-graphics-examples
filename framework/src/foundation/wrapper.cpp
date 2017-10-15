#include "foundation/wrapper.hpp"

#include <exception>

namespace fd
{
	std::shared_ptr<vk::Instance> createInstance(const vk::InstanceCreateInfo & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto instance = vk::createInstance(createInfo);
		return std::shared_ptr<vk::Instance>(new vk::Instance(instance),
			[](vk::Instance *p) {
			p->destroy();
		});
	}

	std::shared_ptr<vk::Device> createDevice(const std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		const vk::DeviceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto device = pPhysicalDevice->createDevice(createInfo);
		return std::shared_ptr<vk::Device>(new vk::Device(device),
			[](vk::Device *p) {
			p->destroy();
		});
	}

	std::shared_ptr<GLFWwindow> createGLFWWindow(uint32_t width,
		uint32_t height, const char* title)
	{
		auto pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
		return std::shared_ptr<GLFWwindow>(pWindow,
			[](GLFWwindow *p)
		{
			glfwDestroyWindow(p);
		}
		);
	}

	std::shared_ptr<vk::SurfaceKHR> createSurface(std::shared_ptr<vk::Instance> pInstance,
		std::shared_ptr<GLFWwindow> pWindow)
	{
		VkSurfaceKHR surface;
		auto result = static_cast<vk::Result> (glfwCreateWindowSurface(*pInstance,
			pWindow.get(), nullptr, &surface));
		if (result != vk::Result::eSuccess)
		{
			throw std::system_error(result, "gfw::Context::_createSurface");
		}

		return std::shared_ptr<vk::SurfaceKHR>(new vk::SurfaceKHR(surface),
			[pInstance](vk::SurfaceKHR *p) {
			pInstance->destroySurfaceKHR(*p);
		});
	}

	std::shared_ptr<vk::SwapchainKHR> createSwapchainKHR(
		const std::shared_ptr<vk::Device> pDevice,
		const vk::SwapchainCreateInfoKHR & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto swapchain = pDevice->createSwapchainKHR(createInfo, allocator);
		return std::shared_ptr<vk::SwapchainKHR>(new vk::SwapchainKHR(swapchain),
			[pDevice](vk::SwapchainKHR *p)
		{
			pDevice->destroySwapchainKHR(*p);
		}
		);
	}


	std::shared_ptr<vk::ImageView> createImageView(const std::shared_ptr<vk::Device> pDevice,
		const vk::ImageViewCreateInfo & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto imageView = pDevice->createImageView(createInfo, allocator);
		return std::shared_ptr<vk::ImageView>(new vk::ImageView(imageView),
			[pDevice](vk::ImageView *p)
		{
			pDevice->destroyImageView(*p);
		});
	}

	std::shared_ptr<vk::CommandPool> createCommandPool(const std::shared_ptr<vk::Device> pDevice,
		const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto commandPool = pDevice->createCommandPool(createInfo, allocator);
		return std::shared_ptr<vk::CommandPool>(new vk::CommandPool(commandPool),
			[pDevice](vk::CommandPool *p) {
			pDevice->destroyCommandPool(*p);
		});
	}

	std::shared_ptr<vk::RenderPass> createRenderPass(const std::shared_ptr<vk::Device> pDevice,
		const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto renderPass = pDevice->createRenderPass(createInfo, allocator);
		return std::shared_ptr<vk::RenderPass>(new vk::RenderPass(renderPass),
			[pDevice](vk::RenderPass *p) {
			pDevice->destroyRenderPass(*p);
		});
	}

	std::shared_ptr<vk::Buffer> createBuffer(const std::shared_ptr<vk::Device> pDevice,
		const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto buffer = pDevice->createBuffer(createInfo, allocator);
		return std::shared_ptr<vk::Buffer>(new vk::Buffer(buffer),
			[pDevice](vk::Buffer *p) {
			pDevice->destroyBuffer(*p);
		});
	}

	std::shared_ptr<vk::Image> createImage(const std::shared_ptr<vk::Device> pDevice,
		const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto image = pDevice->createImage(createInfo, allocator);
		return std::shared_ptr<vk::Image>(new vk::Image(image),
			[pDevice](vk::Image *p) {
			pDevice->destroyImage(*p);
		});
	}

	std::shared_ptr<vk::DeviceMemory> allocateMemory(const std::shared_ptr<vk::Device> pDevice,
		const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto memory = pDevice->allocateMemory(allocateInfo, allocator);
		return std::shared_ptr<vk::DeviceMemory>(new vk::DeviceMemory(memory),
			[pDevice](vk::DeviceMemory *p) {
			pDevice->freeMemory(*p);
		});
	}

	std::shared_ptr<vk::Framebuffer> createFrameBuffer(const std::shared_ptr<vk::Device> pDevice,
		const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto frameBuffer = pDevice->createFramebuffer(createInfo, allocator);
		return std::shared_ptr<vk::Framebuffer>(new vk::Framebuffer(frameBuffer),
			[pDevice](vk::Framebuffer *p) {
			pDevice->destroyFramebuffer(*p);
		});
	}

	std::shared_ptr<vk::Semaphore> createSemaphore(const std::shared_ptr<vk::Device> pDevice,
		const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto semaphore = pDevice->createSemaphore(createInfo);
		return std::shared_ptr<vk::Semaphore>(new vk::Semaphore(semaphore),
			[pDevice](vk::Semaphore *p) {
			pDevice->destroySemaphore(*p);
		});
	}

	std::shared_ptr<vk::CommandBuffer> allocateCommandBuffer(const std::shared_ptr<vk::Device> pDevice,
		const std::shared_ptr<vk::CommandPool> pCommandPool,
		vk::CommandBufferAllocateInfo & allocateInfo)
	{
		//allocate only one command buffer.
		allocateInfo.setCommandBufferCount(1U);
		allocateInfo.setCommandPool(*pCommandPool);
		auto commandBuffer = pDevice->allocateCommandBuffers(allocateInfo)[0];
		return std::shared_ptr<vk::CommandBuffer>(new vk::CommandBuffer(commandBuffer),
			[pDevice, pCommandPool](vk::CommandBuffer *p) {
			pDevice->freeCommandBuffers(*pCommandPool, *p);
		});
	}

	std::shared_ptr<vk::Sampler> createSampler(const std::shared_ptr<vk::Device> pDevice,
		const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto sampler = pDevice->createSampler(createInfo, allocator);
		return std::shared_ptr<vk::Sampler>(new vk::Sampler(sampler),
			[pDevice](vk::Sampler *p) {
			pDevice->destroySampler(*p);
		});
	}

	std::shared_ptr<vk::ShaderModule> createShaderModule(const std::shared_ptr<vk::Device> pDevice,
		const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto shaderModule = pDevice->createShaderModule(createInfo, allocator);
		return std::shared_ptr<vk::ShaderModule>(new vk::ShaderModule(shaderModule),
			[pDevice](vk::ShaderModule *p) {
			pDevice->destroyShaderModule(*p);
		});
	}

	std::shared_ptr<vk::DescriptorSetLayout> createDescriptorSetLayout(const std::shared_ptr<vk::Device> pDevice,
		const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto descriptorSetLayout = pDevice->createDescriptorSetLayout(createInfo, allocator);
		return std::shared_ptr<vk::DescriptorSetLayout>(new vk::DescriptorSetLayout(descriptorSetLayout),
			[pDevice](vk::DescriptorSetLayout *p) {
			pDevice->destroyDescriptorSetLayout(*p);
		});
	}

	std::shared_ptr<vk::DescriptorPool> createDescriptorPool(const std::shared_ptr<vk::Device> pDevice,
		const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional < const vk::AllocationCallbacks > allocator)
	{
		auto descriptorPool = pDevice->createDescriptorPool(createInfo, allocator);
		return std::shared_ptr<vk::DescriptorPool>(new vk::DescriptorPool(descriptorPool), 
			[pDevice](vk::DescriptorPool *p) {
			pDevice->destroyDescriptorPool(*p);
		});
	}

	std::shared_ptr<vk::DescriptorSet> allocateDescriptorSet(const std::shared_ptr<vk::Device> pDevice,
		const std::shared_ptr<vk::DescriptorPool> pDescriptorPool, vk::DescriptorSetAllocateInfo & allocateInfo)
	{
		allocateInfo.setDescriptorPool(*pDescriptorPool);
		allocateInfo.setDescriptorSetCount(1u);
		auto descriptorSet = pDevice->allocateDescriptorSets(allocateInfo)[0];
		return std::shared_ptr<vk::DescriptorSet>(new vk::DescriptorSet(descriptorSet), 
			[pDevice, pDescriptorPool](vk::DescriptorSet *p) {
			pDevice->freeDescriptorSets(*pDescriptorPool, *p);
		});
	}

	std::shared_ptr<vk::PipelineLayout> createPipelineLayout(const std::shared_ptr<vk::Device> pDevice,
		const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto pipelineLayout = pDevice->createPipelineLayout(createInfo, allocator);
		return std::shared_ptr<vk::PipelineLayout>(new vk::PipelineLayout(pipelineLayout),
			[pDevice](vk::PipelineLayout *p) {
			pDevice->destroyPipelineLayout(*p);
		});
	}

	std::shared_ptr<vk::Pipeline> createGraphicsPipeline(const std::shared_ptr<vk::Device> pDevice,
		vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo,
		vk::Optional<const vk::AllocationCallbacks> allocator)
	{
		auto pipeline = pDevice->createGraphicsPipeline(pipelineCache, createInfo, allocator);
		return std::shared_ptr<vk::Pipeline>(new vk::Pipeline(pipeline),
			[pDevice](vk::Pipeline *p) {
			pDevice->destroyPipeline(*p);
		});
	}
}
