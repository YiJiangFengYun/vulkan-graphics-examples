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
		, m_currImageIndex(-1)
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
	)
		: m_pWindow(pWindow)
		, m_pSurface(pSurface)
		, m_currImageIndex(-1)
	{
		int32_t width;
		int32_t height;
		glfwGetWindowSize(m_pWindow.get(), &width, &height);
		m_width = static_cast<uint32_t>(width);
		m_height = static_cast<uint32_t>(height);
		glfwSetWindowUserPointer(pWindow.get(), this);
		glfwSetWindowSizeCallback(pWindow.get(), onWindowResized);
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
		if(m_pRenderers.size() != num) m_pRenderers.resize(num);

		//Create the renders of the specified type.
		for (size_t i = 0; i < num; ++i)
		{
			if (m_pRenderers[i] == nullptr) 
			{
				m_pRenderers[i] = _createRenderer(
					m_pSwapchainImageViews[i],
					m_swapchainImageFormat,
					m_swapchainExtent.width,
					m_swapchainExtent.height
				);
			}
			else 
			{
				m_pRenderers[i]->reset(m_pSwapchainImageViews[i],
					m_swapchainImageFormat,
					m_swapchainExtent.width,
					m_swapchainExtent.height);
			}
		}

	}

	std::shared_ptr<vg::Renderer> Window::_createRenderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
	{
		return std::shared_ptr<vg::Renderer>(
					new vg::Renderer(pSwapchainImageView
						, swapchainImageFormat
					    , swapchainImageWidth
					    , swapchainImageHeight
					)
				);
	}

	void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
	{
#ifdef USE_IMGUI_BIND
        vg::Renderer::SceneAndCamera sceneAndCamera;
		sceneAndCamera.pScene = vgim::getScene().get();
		sceneAndCamera.pCamera = vgim::getCamera().get();
		auto addedInfo = info;
		addedInfo.sceneAndCameraCount = info.sceneAndCameraCount + 1u;
		std::vector<vg::Renderer::SceneAndCamera> sceneAndCameras(addedInfo.sceneAndCameraCount);
		for (uint32_t i = 0; i < info.sceneAndCameraCount; ++i)
		{
			sceneAndCameras[i] = *(info.pSceneAndCamera + i);
		}
		sceneAndCameras[info.sceneAndCameraCount] = sceneAndCamera;
		addedInfo.pSceneAndCamera = sceneAndCameras.data();
        pRenderer->render(addedInfo, resultInfo);
#else
        pRenderer->render(info, resultInfo); 		
#endif //USE_IMGUI_BIND
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

#ifdef USE_IMGUI_BIND
    void Window::_initIMGUI()
	{
		m_mousePressed[0] = VGF_FALSE;
		m_mousePressed[1] = VGF_FALSE;
		m_mousePressed[2] = VGF_FALSE;

		ImGuiIO& io = ImGui::GetIO();

		//init key map.
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		//init feature functions.
		io.ClipboardUserData = m_pWindow.get();		
		io.RenderDrawListsFn = nullptr;
		io.SetClipboardTextFn = [](void *userData, const char *text)
		{
			glfwSetClipboardString((GLFWwindow*)userData, text);
		};
		io.GetClipboardTextFn = [](void *userData)->const char * {
			return glfwGetClipboardString((GLFWwindow *)userData);
		};

		//init call backs.
		glfwSetMouseButtonCallback(m_pWindow.get(), [](GLFWwindow* glfwWindow, int button, int action, int /*mods*/) 
		{
			Window* const instance = (Window*)glfwGetWindowUserPointer(glfwWindow);
			if (action == GLFW_PRESS && button >= 0 && button < 3)
                instance->m_mousePressed[button] = VGF_TRUE;
		});
		glfwSetScrollCallback(m_pWindow.get(), [](GLFWwindow*, double xOffset, double yOffset)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheel += (float)yOffset;
		});
		glfwSetKeyCallback(m_pWindow.get(), [](GLFWwindow*, int key, int, int action, int mods)
		{
			ImGuiIO& io = ImGui::GetIO();
            if (action == GLFW_PRESS)
                io.KeysDown[key] = true;
            if (action == GLFW_RELEASE)
                io.KeysDown[key] = false;
        
            (void)mods; // Modifiers are not reliable across systems
            io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
            io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
            io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
            io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		});
		glfwSetCharCallback(m_pWindow.get(), [](GLFWwindow*, unsigned int c)
		{
			ImGuiIO& io = ImGui::GetIO();
            if (c > 0 && c < 0x10000)
                io.AddInputCharacter((unsigned short)c);
		});
	}

#endif //USE_IMGUI_BIND

	void Window::_reCreateSwapchain()
	{
		_createSwapchain();
		_createSwapchainImageViews();
		_createRenderers();
	}

	void Window::_doUpdate()
	{
		_onPreUpdate();
#ifdef USE_IMGUI_BIND
		vgim::updateCanvasSize(m_width, m_height);
#endif
		_onUpdate();
#ifdef USE_IMGUI_BIND
		vgim::updateFromImGUI();
#endif
		_onPostUpdate();
	}

	void Window::_doRender()
	{
		_onPreRender();
		uint32_t imageIndex;
		if (m_currImageIndex < 0)
		{
			auto pDevice = vg::pApp->getDevice();
			VkResult result = vkAcquireNextImageKHR(static_cast<VkDevice>(*pDevice)
				, static_cast<VkSwapchainKHR>(*m_pSwapchain)
				, std::numeric_limits<uint64_t>::max()
				, static_cast<VkSemaphore>(*m_pImageAvailableSemaphore)
				, VK_NULL_HANDLE
				, &imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				_doReCreateSwapchain();
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Failed to acquire swap chain image");
			}
			m_currImageIndex = static_cast<int32_t>(imageIndex);
		}
		else
		{
			imageIndex = m_currImageIndex;
		}

		if (m_pRenderers[imageIndex]->isValidForRender())
		{
			vg::Renderer::RenderInfo info;
			info.sceneAndCameraCount = 0;
			info.pSceneAndCamera = nullptr;
			info.waitSemaphoreCount = 1u;
			info.pWaitSemaphores = m_pImageAvailableSemaphore.get();

			vg::Renderer::RenderResultInfo resultInfo;
			resultInfo.isRendered = VG_FALSE;

			_renderWithRenderer(m_pRenderers[imageIndex], info, resultInfo);

			//if (resultInfo.isRendered == VG_FALSE) throw std::runtime_error("No content was rendered.");
			if (resultInfo.isRendered)
			{
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
				VkResult result = vkQueuePresentKHR(static_cast<VkQueue>(queue), reinterpret_cast<VkPresentInfoKHR *>(&presentInfo));
				m_currImageIndex = -1;
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
		}
		else
		{
			throw std::runtime_error("Renderer is invalid for render.");
		}
		_onRender();
		_onPostRender();
	}

	void Window::_doReCreateSwapchain()
	{
		_onPreReCreateSwapchain();
		_reCreateSwapchain();
		_onPostReCreateSwapchain();
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
