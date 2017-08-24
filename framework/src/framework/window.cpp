#include "window.hpp"

#include <plog/Log.h>
#include <map>
#include <unordered_map>
#include <set>

vk::Format fw::Window::DEFAULT_DEPTH_FORMAT(vk::Format::eD32SfloatS8Uint);

fw::Window::Window(uint32_t width, uint32_t height, const char* title,
	vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device,
	vk::Queue graphicsQueue, vk::Queue presentQueue)
{
	m_vkInstance = instance;
	m_physicalDevice = physicalDevice;
	m_device = device;
	m_graphicsQueue = graphicsQueue;
	m_presentQueue = presentQueue;
	m_depthFormat = DEFAULT_DEPTH_FORMAT;
	_createWindow(width, height, title);
	_createSurface();
	_createSwapchain();
	_createSwapchainImageViews();
	_createCommandPool();
	_createRenderPass();
	_createDepthResources();
	_createFramebuffers();
}

fw::Window::Window(GLFWwindow *pWindow, vk::SurfaceKHR surface,
	vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device,
	vk::Queue graphicsQueue, vk::Queue presentQueue)
{
	m_pWindow = pWindow;
	m_surface = surface;
	m_vkInstance = instance;
	m_physicalDevice = physicalDevice;
	m_device = device;
	m_graphicsQueue = graphicsQueue;
	m_presentQueue = presentQueue;
	m_depthFormat = DEFAULT_DEPTH_FORMAT;
	glfwSetWindowUserPointer(pWindow, this);
	glfwSetWindowSizeCallback(pWindow, fw::onWindowResized);
	_createSwapchain();
	_createSwapchainImageViews();
	_createCommandPool();
	_createRenderPass();
	_createDepthResources();
	_createFramebuffers();
}

fw::Window::~Window()
{
	_destroyFramebuffers();
	_destroyDepthResources();
	_destroyRenderPass();
	_destroyCommandPool();
	_destroySwapchainImageViews();
	_destroySwapchain();
	_destroySurface();
	_destroyWindow();
}

GLFWwindow *fw::Window::getGLFWWindow() const
{
	return m_pWindow;
}

void fw::Window::_createWindow(uint32_t width, uint32_t height, const char* title)
{
	m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, fw::onWindowResized);
}

void fw::Window::_destroyWindow()
{
	if (m_pWindow != nullptr)
		glfwDestroyWindow(m_pWindow);
}

void fw::Window::_createSurface()
{
	VkSurfaceKHR surface;
	auto result = static_cast<vk::Result > (glfwCreateWindowSurface(m_vkInstance, m_pWindow, nullptr, &surface));
	if ( result != vk::Result::eSuccess)
	{
		throw std::system_error(result, "fw::Context::_createSurface");
	}
	m_surface = surface;
	LOG(plog::debug) << "Create successfully surface.";
}

void fw::Window::_destroySurface()
{
	if (m_surface != vk::SurfaceKHR(nullptr))
	{
		m_vkInstance.destroySurfaceKHR(m_surface);
	}
}

void fw::Window::_createSwapchain()
{
	SwapChainSupportDetails details = SwapChainSupportDetails::querySwapChainSupport(m_physicalDevice, m_surface);
	vk::SurfaceFormatKHR surfaceFormat = details.chooseSurfaceFormat();
	vk::PresentModeKHR presentMode = details.choosePresentMode();
	vk::Extent2D extent = details.chooseExtent(m_pWindow);

	//LOG(plog::debug) << "Swapchain surface format: " << surfaceFormat.format

	uint32_t minImageCount = details.capabilities.minImageCount + 1;
	if (details.capabilities.maxImageCount > 0 && minImageCount > details.capabilities.maxImageCount)
	{
		minImageCount = details.capabilities.maxImageCount;
	}


	vk::SwapchainCreateInfoKHR createInfo = {
		vk::SwapchainCreateFlagsKHR(),
		m_surface,
		minImageCount,
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		extent,
		uint32_t(1),
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive,
		uint32_t(0),
		nullptr,
		details.capabilities.currentTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		presentMode,
		VkBool32(VK_TRUE),
		vk::SwapchainKHR(nullptr)
	};

	UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(m_physicalDevice, m_surface);
	if (usedQueueFamily.graphicsFamily != usedQueueFamily.presentFamily)
	{
		std::vector<uint32_t> queueFamilyIndices = { 
			(uint32_t)usedQueueFamily.graphicsFamily, 
			(uint32_t)usedQueueFamily.presentFamily 
		};
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}

	m_swapchain = m_device.createSwapchainKHR(createInfo);
	LOG(plog::debug) << "Create successfully swapchain.";

	m_swapchainImages =  m_device.getSwapchainImagesKHR(m_swapchain);
	m_swapchainImageFormat = surfaceFormat.format;
	m_swapchainExtent = extent;
}

void fw::Window::_destroySwapchain()
{
	if (m_swapchain != vk::SwapchainKHR(nullptr))
	{
		m_device.destroySwapchainKHR(m_swapchain);
	}
}

void fw::Window::_createSwapchainImageViews()
{
	size_t num = m_swapchainImages.size();
	m_swapchainImageViews.resize(m_swapchainImages.size());

	for (size_t i = 0; i < num; ++i)
	{
		_createImageView(m_swapchainImages[i], m_swapchainImageFormat, vk::ImageAspectFlagBits::eColor, m_swapchainImageViews[i]);
	}
}

void fw::Window::_destroySwapchainImageViews()
{
	for (const auto& imageView : m_swapchainImageViews)
	{
		m_device.destroyImageView(imageView);
	}
}

void fw::Window::_createCommandPool()
{
	UsedQueueFamily queueFamilyIndices = UsedQueueFamily::findQueueFamilies(m_physicalDevice, m_surface);

	vk::CommandPoolCreateInfo createInfo = {
		vk::CommandPoolCreateFlags(),
		static_cast<uint32_t>(queueFamilyIndices.graphicsFamily)
	};
	m_commandPool = m_device.createCommandPool(createInfo, nullptr);
}

void fw::Window::_destroyCommandPool()
{
	if (m_commandPool != vk::CommandPool(nullptr))
	{
		m_device.destroyCommandPool(m_commandPool);
	}
}

void fw::Window::_createRenderPass()
{
	vk::AttachmentDescription colorAttachment = {
		vk::AttachmentDescriptionFlags(),
		m_swapchainImageFormat,
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	};

	vk::AttachmentDescription depthAttachment = {
		vk::AttachmentDescriptionFlags(),
		m_depthFormat,
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

	vk::AttachmentReference colorAttachmentRef = {
		uint32_t(0),
		vk::ImageLayout::eColorAttachmentOptimal
	};

	vk::AttachmentReference depthAttachmentRef = {
		uint32_t(1),
		vk::ImageLayout::eDepthStencilAttachmentOptimal
	};

	vk::SubpassDescription subpass = {
		vk::SubpassDescriptionFlags(),
		vk::PipelineBindPoint::eGraphics,
		0,
		nullptr,
		1,
		&colorAttachmentRef,
		nullptr,
		&depthAttachmentRef,
		0,
		nullptr
	};

	vk::SubpassDependency dependency = {
		VK_SUBPASS_EXTERNAL,
		0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::PipelineStageFlagBits::eColorAttachmentOutput,
		vk::AccessFlags(),
		vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
		vk::DependencyFlags()
	};

	std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
	vk::RenderPassCreateInfo renderPassCreateInfo = {
		vk::RenderPassCreateFlags(),
		static_cast<uint32_t>(attachments.size()),
		attachments.data(),
		static_cast<uint32_t>(1),
		&subpass,
		static_cast<uint32_t>(1),
		&dependency
	};

	m_renderPass = m_device.createRenderPass(renderPassCreateInfo);
}

void fw::Window::_destroyRenderPass()
{
	if (m_renderPass != vk::RenderPass(nullptr))
	{
		m_device.destroyRenderPass(m_renderPass);
		m_renderPass = nullptr;
	}
}

void fw::Window::_createDepthResources()
{
	_checkDepthFormat();
	_createImage(m_swapchainExtent.width, 
		m_swapchainExtent.height, 
		m_depthFormat, 
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		m_depthImage,
		m_depthImageMemory);
	_createImageView(m_depthImage, m_depthFormat, vk::ImageAspectFlagBits::eDepth, m_depthImageView);
	_transitionImageLayout(m_depthImage, m_depthFormat, vk::ImageLayout::eUndefined,
		vk::ImageLayout::eDepthStencilAttachmentOptimal);
}

void fw::Window::_destroyDepthResources()
{
	if (m_depthImageView != vk::ImageView(nullptr))
	{
		m_device.destroyImageView(m_depthImageView);
		m_depthImageView = nullptr;
	}
	if (m_depthImage != vk::Image(nullptr))
	{
		m_device.destroyImage(m_depthImage);
		m_depthImage = nullptr;
	}
	if (m_depthImageMemory != vk::DeviceMemory(nullptr))
	{
		m_device.freeMemory(m_depthImageMemory);
		m_depthImageMemory = nullptr;
	}
}

void fw::Window::_createFramebuffers()
{
	size_t count = m_swapchainImageViews.size();
	m_swapchainFramebuffers.resize(count);
	for (size_t i = 0; i < count; ++i)
	{
		std::array<vk::ImageView, 2> attachments = { m_swapchainImageViews[i], m_depthImageView };
		vk::FramebufferCreateInfo createInfo = {
			vk::FramebufferCreateFlags(),
			m_renderPass,
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			m_swapchainExtent.width,
			m_swapchainExtent.height,
			static_cast<uint32_t>(1) 
		};

		m_swapchainFramebuffers[i] = m_device.createFramebuffer(createInfo);
	}
}

void fw::Window::_destroyFramebuffers()
{
	size_t num = m_swapchainFramebuffers.size();
	for (size_t i = 0; i < num; ++i)
	{
		if (m_swapchainFramebuffers[i] != vk::Framebuffer(nullptr))
		{
			m_device.destroyFramebuffer(m_swapchainFramebuffers[i]);
			m_swapchainFramebuffers[i] = nullptr;
		}
	}
}

void fw::Window::_createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
	vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory)
{
	vk::ImageCreateInfo createInfo = {
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		format,
		{
			width,
			height,
			uint32_t(1)
		},
		1,
		1,
		vk::SampleCountFlagBits::e1,
		tiling,
		usage,
		vk::SharingMode::eExclusive,
		0,
		nullptr,
		vk::ImageLayout::eUndefined
	};

	UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(m_physicalDevice, m_surface);
	if (usedQueueFamily.graphicsFamily != usedQueueFamily.presentFamily)
	{
		std::vector<uint32_t> queueFamilyIndices = {
			(uint32_t)usedQueueFamily.graphicsFamily,
			(uint32_t)usedQueueFamily.presentFamily
		};
		createInfo.sharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}
	image = m_device.createImage(createInfo, nullptr);

	auto memRequirements = m_device.getImageMemoryRequirements(image);

	vk::MemoryAllocateInfo allocInfo = {
		memRequirements.size,
		_findMemoryType(memRequirements.memoryTypeBits, properties)
	};

	imageMemory = m_device.allocateMemory(allocInfo, nullptr);

	m_device.bindImageMemory(image, imageMemory, vk::DeviceSize(0));
}

void fw::Window::_createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, vk::ImageView& imageView)
{
	vk::ImageViewCreateInfo createInfo = {
		vk::ImageViewCreateFlags(),
		image,
		vk::ImageViewType::e2D,
		format,
		{
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity,
			vk::ComponentSwizzle::eIdentity
		},
		{
			aspectFlags,
			uint32_t(0),
			uint32_t(1),
			uint32_t(0),
			uint32_t(1)
         }
	};

	imageView = m_device.createImageView(createInfo);
}

void fw::Window::_transitionImageLayout(vk::Image image, vk::Format format,
	vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	vk::CommandBuffer commandBuffer = _beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier = {};
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;

	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
	}
	else
	{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	}
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlags();
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlags();
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | 
			vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	}
	else
	{
		throw std::invalid_argument("Unsupported layout transition!");
	}

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eTopOfPipe,
		vk::DependencyFlags(),
		nullptr,
		nullptr,
		barrier
		);

	_endSingleTimeCommands(commandBuffer);
}

uint32_t fw::Window::_findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
	vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");

}

vk::CommandBuffer fw::Window::_beginSingleTimeCommands() {
	vk::CommandBufferAllocateInfo allocateInfo = {
		m_commandPool,
		vk::CommandBufferLevel::ePrimary,
		uint32_t(1)
	};
	vk::CommandBuffer commandBuffer = m_device.allocateCommandBuffers(allocateInfo)[0];

	vk::CommandBufferBeginInfo beginInfo = {
		vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
	};
	commandBuffer.begin(beginInfo);

	return commandBuffer;
}

void fw::Window::_endSingleTimeCommands(vk::CommandBuffer commandBuffer) {
	commandBuffer.end();
	vk::SubmitInfo submitInfo = { 0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr };
	m_graphicsQueue.submit(submitInfo, nullptr);
	m_graphicsQueue.waitIdle();
	m_device.freeCommandBuffers(m_commandPool, commandBuffer);
}

void fw::Window::_checkDepthFormat()
{
	auto props = m_physicalDevice.getFormatProperties(m_depthFormat);
	if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
		vk::FormatFeatureFlagBits::eDepthStencilAttachment)
	{
		throw std::runtime_error("Depth format is be supported!");
	}
}

void fw::Window::_onWindowResized(int32_t width, int32_t height)
{
	LOG(plog::debug) << "Context resize.";

	//clear up old datas.
	_destroyFramebuffers();
	_destroyDepthResources();
	_destroySwapchainImageViews();
	_destroySwapchain();


	//recreate.
	_createSwapchain();
	_createSwapchainImageViews();
	_createDepthResources();
	_createFramebuffers();
}

void fw::onWindowResized(GLFWwindow* window, int32_t width, int32_t height)
{
	fw::Window* const instance = (fw::Window*)glfwGetWindowUserPointer(window);
	instance->_onWindowResized(width, height);
}


fw::SwapChainSupportDetails fw::SwapChainSupportDetails::querySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
	SwapChainSupportDetails details;
	details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
	return details;
}

vk::SurfaceFormatKHR fw::SwapChainSupportDetails::chooseSurfaceFormat()
{
	if (this->formats.size() == 1 && this->formats[0].format == vk::Format::eUndefined)
	{
		return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}

	for (const auto& format : this->formats)
	{
		if (format.format == vk::Format::eB8G8R8A8Unorm &&
			format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			return format;
		}
	}

	return this->formats[0];
}

vk::PresentModeKHR fw::SwapChainSupportDetails::choosePresentMode()
{
	std::unordered_map<vk::PresentModeKHR, uint32_t> presentModeAndCores = {
		{ vk::PresentModeKHR::eMailbox, 0 },
		{ vk::PresentModeKHR::eFifo, 1 },
		{ vk::PresentModeKHR::eImmediate, 2 }
	};

	vk::PresentModeKHR currPresentMode = vk::PresentModeKHR::eImmediate;
	uint32_t currCore = presentModeAndCores[currPresentMode];
	for (const auto& presentMode : this->presentModes)
	{
		auto iterator = presentModeAndCores.find(presentMode);

		if (iterator != presentModeAndCores.end() && iterator->second < currCore)
		{
			currPresentMode = iterator->first;
			currCore = iterator->second;
		}
	}

	return currPresentMode;
}

vk::Extent2D fw::SwapChainSupportDetails::chooseExtent(GLFWwindow* window)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		actualExtent.width = std::max(capabilities.minImageExtent.width,
			std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height,
			std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

