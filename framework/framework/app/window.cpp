#include "framework/app/window.hpp"

#include <plog/Log.h>
#include <map>
#include <unordered_map>
#include <set>

namespace vgf {
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
		auto result = static_cast<vk::Result> (glfwCreateWindowSurface(static_cast<VkInstance>(*pInstance),
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

	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	    )
		: m_width(width)
		, m_height(height)
	{
		_createWindow(width, height, title);
		_createSurface();
		//_allocatePresentQueue();
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: m_pWindow(pWindow)
		, m_pSurface(pSurface)
	{
		int32_t width;
		int32_t height;
		glfwGetWindowSize(m_pWindow.get(), &width, &height);
		m_width = static_cast<uint32_t>(width);
		m_height = static_cast<uint32_t>(height);
		glfwSetWindowUserPointer(pWindow.get(), this);
		glfwSetWindowSizeCallback(pWindow.get(), onWindowResized);
		//_allocatePresentQueue();
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
		_createSemaphores();
	}

	Window::~Window()
	{
		//_freePresentQueue();
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
		m_pWindow = createGLFWWindow(width, height, title);

		glfwSetWindowUserPointer(m_pWindow.get(), this);
		glfwSetWindowSizeCallback(m_pWindow.get(), onWindowResized);
	}

	void Window::_createSurface()
	{
		auto pInstance = vg::pApp->getVKInstance();
		m_pSurface = createSurface(pInstance, m_pWindow);
		LOG(plog::debug) << "Create successfully surface.";
	}

	/*void Window::_allocatePresentQueue()
	{
		kgs::pApp->allocatePresentQueue(m_presentQueueIndex, m_presentQueue);
	}

	void Window::_freePresentQueue()
	{
		kgs::pApp->freePresentQueue(m_presentQueueIndex);
	}*/

	void Window::_createSwapchain()
	{
		auto pPhysicalDevice = vg::pApp->getPhysicalDevice();
		vg::SwapChainSupportDetails details = vg::SwapChainSupportDetails::querySwapChainSupport(*pPhysicalDevice, *m_pSurface);
		vk::SurfaceFormatKHR surfaceFormat = details.chooseSurfaceFormat();
		vk::PresentModeKHR presentMode = details.choosePresentMode();
		int32_t width, height;
		glfwGetWindowSize(m_pWindow.get(), &width, &height);
		vk::Extent2D extent = details.chooseExtent(width, height);
		
		//LOG(plog::debug) << "Swapchain surface format: " << surfaceFormat.format

		uint32_t minImageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && minImageCount > details.capabilities.maxImageCount)
		{
			minImageCount = details.capabilities.maxImageCount;
		}

		vk::SwapchainKHR oldSwapchain;
		if (m_pSwapchain != nullptr)
		{
			oldSwapchain = *m_pSwapchain;
		}

		vk::SwapchainCreateInfoKHR createInfo = {
			vk::SwapchainCreateFlagsKHR(),            //flags
			*m_pSurface,                              //surface
			minImageCount,                            //minImageCount
			surfaceFormat.format,                     //imageFormat
			surfaceFormat.colorSpace,                 //imageColorSpace
			extent,                                   //imageExtent
			1u,                                       //imageArrayLayers
			vk::ImageUsageFlagBits::eColorAttachment, //imageUsage
			vk::SharingMode::eExclusive,              //imageSharingMode
			0u,                                       //queueFamilyIndexCount
			nullptr,                                  //pQueueFamilyIndices
			details.capabilities.currentTransform,    //preTransform
			vk::CompositeAlphaFlagBitsKHR::eOpaque,   //compositeAlpha
			presentMode,                              //presentMode
			VK_TRUE,                                  //clipped
			oldSwapchain                              //oldSwapchain
		};

		vg::UsedQueueFamily usedQueueFamily = vg::UsedQueueFamily::findQueueFamilies(*pPhysicalDevice, *m_pSurface);
		std::vector<uint32_t> queueFamilyIndices = {
			(uint32_t)usedQueueFamily.graphicsFamily,
			(uint32_t)usedQueueFamily.presentFamily
		};

		if (usedQueueFamily.graphicsFamily != usedQueueFamily.presentFamily)
		{
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
			createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}

		auto pDevice = vg::pApp->getDevice();
		m_pSwapchain = fd::createSwapchainKHR(pDevice, createInfo);
		LOG(plog::debug) << "Create successfully swapchain.";

		m_swapchainImages = pDevice->getSwapchainImagesKHR(*m_pSwapchain);
		m_swapchainImageFormat = surfaceFormat.format;
		m_swapchainExtent = extent;
	}

	void Window::_createSwapchainImageViews()
	{
		size_t num = m_swapchainImages.size();
		m_pSwapchainImageViews.resize(num);

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
		m_pRenderers.resize(num, nullptr);

		//Create the renders of the specified type.
		for (size_t i = 0; i < num; ++i)
		{
			m_pRenderers[i] = _createRenderer(m_pSwapchainImageViews[i]);
		}

	}

	std::shared_ptr<vg::Renderer> Window::_createRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView)
	{
		return std::shared_ptr<vg::Renderer>(
					new vg::Renderer(pSwapchainImageView
						, m_swapchainImageFormat
					    , m_swapchainExtent.width
					    , m_swapchainExtent.height
					)
				);
	}
			

	void Window::_createSemaphores()
	{
		vk::SemaphoreCreateInfo createInfo = {
			vk::SemaphoreCreateFlags()
		};
		auto pDevice = vg::pApp->getDevice();
		m_pImageAvailableSemaphore = fd::createSemaphore(pDevice, createInfo);
		//m_pRenderFinishedSemaphore = fd::createSemaphore(m_pDevice, createInfo);
	}

	void Window::_reCreateSwapchain()
	{
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
	}

	void Window::_doUpdate()
	{
		_onPreUpdate();
		_onUpdate();
		_onPostUpdate();
	}

	void Window::_doRender()
	{
		_onPreRender();
		_render();
		_onPostRender();
	}

	void Window::_doReCreateSwapchain()
	{
		_onPreReCreateSwapchain();
		_reCreateSwapchain();
		_onPostReCreateSwapchain();
	}

	void Window::_render()
	{
		auto pDevice = vg::pApp->getDevice();
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(static_cast<VkDevice>(*pDevice)
			, static_cast<VkSwapchainKHR>(*m_pSwapchain)
			, std::numeric_limits<uint64_t>::max()
			, static_cast<VkSemaphore>(*m_pImageAvailableSemaphore)
			, VK_NULL_HANDLE
			, &imageIndex);

		if (m_pRenderers[imageIndex]->isValidForRender())
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				_doReCreateSwapchain();
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Failed to acquire swap chain image");
			}

			vg::Renderer::RenderInfo info;
			info.sceneAndCameraCount = 0;
			info.pSceneAndCamera = nullptr;
			info.waitSemaphoreCount = 1u;
			info.pWaitSemaphores = m_pImageAvailableSemaphore.get();

			vg::Renderer::RenderResultInfo resultInfo;
			resultInfo.isRendered = VG_FALSE;

			_renderWithRenderer(m_pRenderers[imageIndex], info, resultInfo);

			if (resultInfo.isRendered == VG_FALSE) throw std::runtime_error("No content was rendered.");
			vk::PresentInfoKHR presentInfo = {
				resultInfo.signalSemaphoreCount, //waitSemaphoreCount
			    resultInfo.pSignalSemaphores,   //pWaitSemaphores
				1u,                                 //swapchainCount
				m_pSwapchain.get(),                 //pSwapchains
				&imageIndex,                        //pImageIndices
				nullptr                             //pResults
			};

			vk::Queue queue;
			uint32_t queueIndex;
			vg::pApp->allocatePresentQueue(queueIndex, queue);
			result = vkQueuePresentKHR(static_cast<VkQueue>(queue), reinterpret_cast<VkPresentInfoKHR *>(&presentInfo));
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				_doReCreateSwapchain();
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
			vkQueueWaitIdle(static_cast<VkQueue>(queue));
#endif //ENABLE_VALIDATION_LAYERS
			vg::pApp->freePresentQueue(queueIndex);
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

		auto pDevice = vg::pApp->getDevice();
		pImage = fd::createImage(pDevice, createInfo);

		auto memRequirements = pDevice->getImageMemoryRequirements(*pImage);

		vk::MemoryAllocateInfo allocInfo = {
			memRequirements.size,
			_findMemoryType(memRequirements.memoryTypeBits, properties)
		};

		pImageMemory = fd::allocateMemory(pDevice, allocInfo);
		pDevice->bindImageMemory(*pImage, *pImageMemory, vk::DeviceSize(0));
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
				0u,
				1u,
				0u,
				1u
			}
		};
		auto pDevice = vg::pApp->getDevice();
		return fd::createImageView(pDevice, createInfo);
	}

	uint32_t Window::_findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		auto pPhysicalDevice = vg::pApp->getPhysicalDevice();
		vk::PhysicalDeviceMemoryProperties memProperties = pPhysicalDevice->getMemoryProperties();
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
		if (width == 0 || height == 0) return;
		LOG(plog::debug) << "Context resize.";

		m_width = width;
		m_height = height;
		_onResize();
		//recreate.
		_doReCreateSwapchain();
	}

	void onWindowResized(GLFWwindow* window, int32_t width, int32_t height)
	{
		Window* const instance = (Window*)glfwGetWindowUserPointer(window);
		instance->_onWindowResized(width, height);
	}


	
}
