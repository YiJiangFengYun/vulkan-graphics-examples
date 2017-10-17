#include "framework/app/window.hpp"

#include <plog/Log.h>
#include <map>
#include <unordered_map>
#include <set>

namespace gfw {

	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: m_renderType(RenderType::BEGIN_RANGE)
		, m_pVKInstance(pInstance)
		, m_pPhysicalDevice(pPhysicalDevice)
		, m_pDevice(pDevice)
		, m_graphicsQueue(graphicsQueue)
		, m_presentQueue(presentQueue)
	{
		_createWindow(width, height, title);
		_createSurface();
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
		, RenderType renderType
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: m_renderType(renderType)
		, m_pVKInstance(pInstance)
		, m_pPhysicalDevice(pPhysicalDevice)
		, m_pDevice(pDevice)
		, m_graphicsQueue(graphicsQueue)
		, m_presentQueue(presentQueue)
	{
		_createWindow(width, height, title);
		_createSurface();
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: m_renderType(RenderType::BEGIN_RANGE)
		, m_pWindow(pWindow)
		, m_pSurface(pSurface)
		, m_pVKInstance(pInstance)
		, m_pPhysicalDevice(pPhysicalDevice)
		, m_pDevice(pDevice)
		, m_graphicsQueue(graphicsQueue)
		, m_presentQueue(presentQueue)
	{

		glfwSetWindowUserPointer(pWindow.get(), this);
		glfwSetWindowSizeCallback(pWindow.get(), onWindowResized);
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::Window(RenderType renderType
		, std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
		, std::shared_ptr<vk::Instance> pInstance
		, std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice
		, std::shared_ptr<vk::Device> pDevice
		, vk::Queue graphicsQueue
		, vk::Queue presentQueue
	)
		: m_renderType(renderType)
		, m_pWindow(pWindow)
		, m_pSurface(pSurface)
		, m_pVKInstance(pInstance)
		, m_pPhysicalDevice(pPhysicalDevice)
		, m_pDevice(pDevice)
		, m_graphicsQueue(graphicsQueue)
		, m_presentQueue(presentQueue)
	{

		glfwSetWindowUserPointer(pWindow.get(), this);
		glfwSetWindowSizeCallback(pWindow.get(), onWindowResized);
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::~Window()
	{
	}

	void Window::run()
	{
		LOG(plog::debug) << "Window run" << std::endl;
		_doUpdate();
		_doRender();
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

	void Window::_createRenderers()
	{
		size_t num = m_swapchainImages.size();

		//Clear references in m_pRenderers.
		m_pRenderers.resize(0u);
		m_pRenderers.resize(m_swapchainImages.size(), nullptr);

		//Create the renders of the specified type.
		for (size_t i = 0; i < num; ++i)
		{
			switch (m_renderType)
			{
			case RenderType::RENDERER_2:
			{
				m_pRenderers[i] = std::shared_ptr<kgs::BaseRenderer>(
					new RenderTypeInfo<RenderType::RENDERER_2>::RendererType(m_pSwapchainImageViews[i]
						, m_swapchainImageFormat
					    , m_swapchainExtent.width
					    , m_swapchainExtent.height
					)
				);
				break;
			}
			case RenderType::RENDERER_3:
			{
				m_pRenderers[i] = std::shared_ptr<kgs::BaseRenderer>(
					new RenderTypeInfo<RenderType::RENDERER_3>::RendererType(m_pSwapchainImageViews[i]
						, m_swapchainImageFormat
					    , m_swapchainExtent.width
					    , m_swapchainExtent.height
					)
				);
				break;
			}
			default:
				throw std::runtime_error("Render type of window is invalid.");
				break;
			}
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

	void Window::_reCreate()
	{
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
	}

	void Window::_doUpdate()
	{
		_onPreUpdate();
		_update();
		_onPostUpdate();
	}

	void Window::_doRender()
	{
		_onPreRender();
		_render();
		_onPostRender();
	}

	void Window::_render()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(*m_pDevice
			, *m_pSwapchain
			, std::numeric_limits<uint64_t>::max()
			, *m_pImageAvailableSemaphore
			, VK_NULL_HANDLE
			, &imageIndex);

		if (m_pRenderers[imageIndex]->isValidForRender())
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				_reCreate();
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Failed to acquire swap chain image");
			}

			kgs::RenderInfo renderInfo;
			renderInfo.waitSemaphoreCount = 1u;
			renderInfo.pWaitSemaphores = m_pImageAvailableSemaphore.get();
			renderInfo.signalSemaphoreCount = 1u;
			renderInfo.pSignalSemaphores = m_pRenderFinishedSemaphore.get();

			m_pRenderers[imageIndex]->render(renderInfo);

			vk::PresentInfoKHR presentInfo = {
				1u,                                 //waitSemaphoreCount
				m_pRenderFinishedSemaphore.get(),   //pWaitSemaphores
				1u,                                 //swapchainCount
				m_pSwapchain.get(),                 //pSwapchains
				&imageIndex,                        //pImageIndices
				nullptr                             //pResults
			};


			result = vkQueuePresentKHR(m_presentQueue, &VkPresentInfoKHR(presentInfo));

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				_reCreate();
			}
			else if (result != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to present swap chain image!");
			}

			/**
			If you run your application with validation layers enabled and you monitor the memory usage of your application,
			you may notice that it is slowly growing. The reason for this is that the validation layer implementation expects
			the application to explicitly synchronize with the GPU. Although this is technically not required, doing so once a
			frame will not noticeably affect performance.
			**/
#ifdef ENABLE_VALIDATION_LAYERS
			vkQueueWaitIdle(m_presentQueue);
#endif //ENABLE_VALIDATION_LAYERS
		}
		else
		{
			throw std::runtime_error("Renderer is invalid for render.");
		}
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

	void Window::_onWindowResized(int32_t width, int32_t height)
	{
		LOG(plog::debug) << "Context resize.";

		//recreate.
		_reCreate();
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
