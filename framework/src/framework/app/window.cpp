#include "framework/app/window.hpp"

#include <plog/Log.h>
#include <map>
#include <unordered_map>
#include <set>

namespace gfw {
	vk::Format Window::DEFAULT_DEPTH_FORMAT(vk::Format::eD32SfloatS8Uint);

	Window::Window(uint32_t width, uint32_t height, const char* title,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice, 
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue)
	{
		m_pVKInstance = pInstance;
		m_pPhysicalDevice = pPhysicalDevice;
		m_pDevice = device;
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
		_createSemaphores();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow, std::shared_ptr<vk::SurfaceKHR> pSurface,
		std::shared_ptr<vk::Instance> pInstance, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> device, vk::Queue graphicsQueue, vk::Queue presentQueue)
	{
		m_pWindow = pWindow;
		m_pSurface = pSurface;
		m_pVKInstance = pInstance;
		m_pPhysicalDevice = pPhysicalDevice;
		m_pDevice = device;
		m_graphicsQueue = graphicsQueue;
		m_presentQueue = presentQueue;
		m_depthFormat = DEFAULT_DEPTH_FORMAT;
		glfwSetWindowUserPointer(pWindow.get(), this);
		glfwSetWindowSizeCallback(pWindow.get(), onWindowResized);
		_createSwapchain();
		_createSwapchainImageViews();
		_createCommandPool();
		_createRenderPass();
		_createDepthResources();
		_createFramebuffers();
		_createSemaphores();
	}

	Window::~Window()
	{
	}

	void Window::run()
	{
		LOG(plog::debug) << "Window run" << std::endl;
		_update();
		_render();
	}

	void Window::windowSetShouldClose(Bool32 value)
	{
		//{
		//	//synchronization
		//	std::lock_guard<std::mutex> lg(m_windowMutex);
			glfwSetWindowShouldClose(m_pWindow.get(), value);
		//}
	}

	Bool32 Window::windowShouldClose()
	{
		//{
		//	//synchronization
		//	std::lock_guard<std::mutex> lg(m_windowMutex);
			return glfwWindowShouldClose(m_pWindow.get());
		//}
	}

	std::shared_ptr<GLFWwindow> Window::getGLFWWindow() const
	{
		return m_pWindow;
	}

	void Window::_createWindow(uint32_t width, uint32_t height, const char* title)
	{
		m_pWindow = fd::createGLFWWindow(width, height, title);

		glfwSetWindowUserPointer(m_pWindow.get(), this);
		glfwSetWindowSizeCallback(m_pWindow.get(), onWindowResized);
	}

	void Window::_createSurface()
	{
		m_pSurface = fd::createSurface(m_pVKInstance, m_pWindow);
		LOG(plog::debug) << "Create successfully surface.";
	}

	void Window::_createSwapchain()
	{
		SwapChainSupportDetails details = SwapChainSupportDetails::querySwapChainSupport(*m_pPhysicalDevice, *m_pSurface);
		vk::SurfaceFormatKHR surfaceFormat = details.chooseSurfaceFormat();
		vk::PresentModeKHR presentMode = details.choosePresentMode();
		vk::Extent2D extent = details.chooseExtent(m_pWindow.get());

		//LOG(plog::debug) << "Swapchain surface format: " << surfaceFormat.format

		uint32_t minImageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && minImageCount > details.capabilities.maxImageCount)
		{
			minImageCount = details.capabilities.maxImageCount;
		}


		vk::SwapchainCreateInfoKHR createInfo = {
			vk::SwapchainCreateFlagsKHR(),
			*m_pSurface,
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

		UsedQueueFamily usedQueueFamily = UsedQueueFamily::findQueueFamilies(*m_pPhysicalDevice, *m_pSurface);
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

		m_pSwapchain = fd::createSwapchainKHR(m_pDevice, createInfo);
		LOG(plog::debug) << "Create successfully swapchain.";

		m_swapchainImages = m_pDevice->getSwapchainImagesKHR(*m_pSwapchain);
		m_swapchainImageFormat = surfaceFormat.format;
		m_swapchainExtent = extent;
	}

	void Window::_createSwapchainImageViews()
	{
		size_t num = m_swapchainImages.size();
		m_pSwapchainImageViews.resize(m_swapchainImages.size());

		for (size_t i = 0; i < num; ++i)
		{
			m_pSwapchainImageViews[i] = _createImageView(m_swapchainImages[i], m_swapchainImageFormat, vk::ImageAspectFlagBits::eColor);
		}
	}

	void Window::_createCommandPool()
	{
		UsedQueueFamily queueFamilyIndices = UsedQueueFamily::findQueueFamilies(*m_pPhysicalDevice, *m_pSurface);

		vk::CommandPoolCreateInfo createInfo = {
			vk::CommandPoolCreateFlags(),
			static_cast<uint32_t>(queueFamilyIndices.graphicsFamily)
		};
		m_pCommandPool = fd::createCommandPool(m_pDevice, createInfo);
	}

	void Window::_createRenderPass()
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
		vk::RenderPassCreateInfo createInfo = {
			vk::RenderPassCreateFlags(),
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			static_cast<uint32_t>(1),
			&subpass,
			static_cast<uint32_t>(1),
			&dependency
		};
		m_pRenderPass = fd::createRenderPass(m_pDevice, createInfo);
	}

	void Window::_createDepthResources()
	{
		_checkDepthFormat();
		_createImage(m_swapchainExtent.width,
			m_swapchainExtent.height,
			m_depthFormat,
			vk::ImageTiling::eOptimal,
			vk::ImageUsageFlagBits::eDepthStencilAttachment,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			m_pDepthImage,
			m_pDepthImageMemory);
		m_pDepthImageView = _createImageView(*m_pDepthImage, m_depthFormat, vk::ImageAspectFlagBits::eDepth);
		_transitionImageLayout(*m_pDepthImage, m_depthFormat, vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}

	void Window::_createFramebuffers()
	{
		size_t count = m_pSwapchainImageViews.size();
		m_pSwapchainFramebuffers.resize(count);
		for (size_t i = 0; i < count; ++i)
		{
			std::array<vk::ImageView, 2> attachments = { *m_pSwapchainImageViews[i], *m_pDepthImageView };
			vk::FramebufferCreateInfo createInfo = {
				vk::FramebufferCreateFlags(),
				*m_pRenderPass,
				static_cast<uint32_t>(attachments.size()),
				attachments.data(),
				m_swapchainExtent.width,
				m_swapchainExtent.height,
				1u
			};

			m_pSwapchainFramebuffers[i] = fd::createFrameBuffer(m_pDevice, createInfo);
		}
	}

	void Window::_createSemaphores()
	{
		vk::SemaphoreCreateInfo createInfo = {
			vk::SemaphoreCreateFlags()
		};

		m_pImageAvailableSemaphore = fd::createSemaphore(m_pDevice, createInfo);
		m_pRenderFinishedSemaphore = fd::createSemaphore(m_pDevice, createInfo);
	}

	void Window::_update()
	{
	}

	void Window::_render()
	{
		/*_beginRender();
		_endRender();*/
	}

	std::shared_ptr<vk::CommandBuffer> Window::_beginRender()
	{
		vk::CommandBufferAllocateInfo allocateInfo = {
			*m_pCommandPool,
			vk::CommandBufferLevel::ePrimary,
			static_cast<uint32_t>(1)
		};

		m_pCurrCommandBuffer = fd::allocateCommandBuffer(m_pDevice, m_pCommandPool, allocateInfo);

		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlags()
		};

		m_pCurrCommandBuffer->begin(beginInfo);


		uint32_t imageIndex = m_pDevice->acquireNextImageKHR(*m_pSwapchain,
			std::numeric_limits<uint64_t>::max(),
			*m_pImageAvailableSemaphore, nullptr).value;

		std::array<vk::ClearValue, 2> clearValues = {};
		clearValues[0] = vk::ClearColorValue();
		clearValues[1] = vk::ClearDepthStencilValue(1.0f, 0);

		vk::RenderPassBeginInfo renderPassBeginInfo = {
			*m_pRenderPass,
			*m_pSwapchainFramebuffers[imageIndex],
			vk::Rect2D({0, 0}, m_swapchainExtent),
			static_cast<uint32_t>(clearValues.size()),
			clearValues.data()
		};

		m_pCurrCommandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		return m_pCurrCommandBuffer;
	}

	void Window::_endRender()
	{
		m_pCurrCommandBuffer->endRenderPass();

		m_pCurrCommandBuffer->end();

		//free command buffer created when begin of render.
		m_pCurrCommandBuffer = nullptr;
	}

	void Window::_createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling,
		vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, 
		std::shared_ptr<vk::Image>& pImage, std::shared_ptr<vk::DeviceMemory>& pImageMemory)
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

		pImage = fd::createImage(m_pDevice, createInfo);

		auto memRequirements = m_pDevice->getImageMemoryRequirements(*pImage);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			_findMemoryType(memRequirements.memoryTypeBits, properties)
		};

		pImageMemory = fd::allocateMemory(m_pDevice, allocInfo);
		m_pDevice->bindImageMemory(*pImage, *pImageMemory, vk::DeviceSize(0));
	}

	std::shared_ptr<vk::ImageView> Window::_createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
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

		return fd::createImageView(m_pDevice, createInfo);
	}

	void Window::_transitionImageLayout(vk::Image image, vk::Format format,
		vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
		auto commandBuffer = _beginSingleTimeCommands();

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

		commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
			vk::PipelineStageFlagBits::eTopOfPipe,
			vk::DependencyFlags(),
			nullptr,
			nullptr,
			barrier
		);

		_endSingleTimeCommands(commandBuffer);
	}

	uint32_t Window::_findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties = m_pPhysicalDevice->getMemoryProperties();
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");

	}

	std::shared_ptr<vk::CommandBuffer> Window::_beginSingleTimeCommands() {
		vk::CommandBufferAllocateInfo allocateInfo = {
			*m_pCommandPool,
			vk::CommandBufferLevel::ePrimary,
			uint32_t(1)
		};
		auto pCommandBuffer = fd::allocateCommandBuffer(m_pDevice, m_pCommandPool, allocateInfo);

		vk::CommandBufferBeginInfo beginInfo = {
			vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
		};
		pCommandBuffer->begin(beginInfo);

		return pCommandBuffer;
	}

	void Window::_endSingleTimeCommands(std::shared_ptr<vk::CommandBuffer> pCommandBuffer) {
		pCommandBuffer->end();
		vk::SubmitInfo submitInfo = { 0, nullptr, nullptr, 1, pCommandBuffer.get(), 0, nullptr };
		m_graphicsQueue.submit(submitInfo, nullptr);
		m_graphicsQueue.waitIdle();
		pCommandBuffer = nullptr;
	}

	void Window::_checkDepthFormat()
	{
		auto props = m_pPhysicalDevice->getFormatProperties(m_depthFormat);
		if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) !=
			vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			throw std::runtime_error("Depth format is be supported!");
		}
	}

	void Window::_onWindowResized(int32_t width, int32_t height)
	{
		LOG(plog::debug) << "Context resize.";

		//recreate.
		_createSwapchain();
		_createSwapchainImageViews();
		_createDepthResources();
		_createFramebuffers();
	}

	void onWindowResized(GLFWwindow* window, int32_t width, int32_t height)
	{
		Window* const instance = (Window*)glfwGetWindowUserPointer(window);
		instance->_onWindowResized(width, height);
	}


	SwapChainSupportDetails SwapChainSupportDetails::querySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) {
		SwapChainSupportDetails details;
		details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
		details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		return details;
	}

	vk::SurfaceFormatKHR SwapChainSupportDetails::chooseSurfaceFormat()
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

	vk::PresentModeKHR SwapChainSupportDetails::choosePresentMode()
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

	vk::Extent2D SwapChainSupportDetails::chooseExtent(GLFWwindow* window)
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
}
