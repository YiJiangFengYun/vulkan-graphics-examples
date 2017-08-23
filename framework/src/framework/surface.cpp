#include "surface.hpp"

#include <plog/Log.h>
#include <map>
#include <unordered_map>
#include <set>


fw::Surface::Surface(uint32_t width, uint32_t height, const char* title,
	vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
{
	m_vkInstance = instance;
	m_physicalDevice = physicalDevice;
	m_device = device;
	_createWindow(width, height, title);
	_createSurface();
	_createSwapchain();
	_createSwapchainImageViews();
}

fw::Surface::Surface(GLFWwindow *pWindow, vk::SurfaceKHR surface,
	vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
{
	m_pWindow = pWindow;
	m_surface = surface;
	m_vkInstance = instance;
	m_physicalDevice = physicalDevice;
	m_device = device;
	glfwSetWindowUserPointer(pWindow, this);
	_createSwapchain();
	_createSwapchainImageViews();
}

fw::Surface::~Surface()
{
	for (const auto& imageView : m_swapchainImageViews)
	{
		m_device.destroyImageView(imageView);
	}

	if (m_swapchain != vk::SwapchainKHR(nullptr))
	{
		m_device.destroySwapchainKHR(m_swapchain);
	}

	if (m_surface != vk::SurfaceKHR(nullptr))
	{
		m_vkInstance.destroySurfaceKHR(m_surface);
	}

	if (m_pWindow != nullptr)
		glfwDestroyWindow(m_pWindow);
}

GLFWwindow *fw::Surface::getWindow() const
{
	return m_pWindow;
}

void fw::Surface::_createWindow(uint32_t width, uint32_t height, const char* title)
{
	m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, fw::onWindowResized);
}

void fw::Surface::_createSurface()
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

void fw::Surface::_createSwapchain()
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

void fw::Surface::_createSwapchainImageViews()
{

}

vk::ImageView fw::Surface::_createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
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

	return m_device.createImageView(createInfo);
}

void fw::Surface::_onWindowResized(int32_t width, int32_t height)
{
	LOG(plog::debug) << "Context resize.";

	//clear up old datas.
	if (m_swapchain != vk::SwapchainKHR(nullptr))
	{
		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	}


	//recreate.
	_createSwapchain();
}

void fw::onWindowResized(GLFWwindow* window, int32_t width, int32_t height)
{
	fw::Surface* const instance = (fw::Surface*)glfwGetWindowUserPointer(window);
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

