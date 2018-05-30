#ifndef VG_SWAPCHAIN_INFO_H
#define VG_SWAPCHAIN_INFO_H

#include <unordered_map>
#include "graphics/global.hpp"

namespace vg
{
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
        SwapChainSupportDetails static querySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);
        vk::SurfaceFormatKHR chooseSurfaceFormat();
        vk::PresentModeKHR choosePresentMode(Bool32 vsync = VG_TRUE);
        vk::Extent2D chooseExtent(int width, int height);
    };
} //namespace kgs

#endif // !VG_SWAPCHAIN_INFO_H
