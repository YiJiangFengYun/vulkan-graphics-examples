#ifndef KGS_GLOBAL_H
#define KGS_GLOBAL_H

#include <glm/glm.hpp>

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#define KGS_FALSE 0
#define KGS_TRUE 1

#include <cstdint>
#include <plog/Log.h>
#include "graphics/context.hpp"

namespace kgs
{
	typedef uint32_t Bool32;
	typedef unsigned char Byte;
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;
	typedef glm::mat2x2 Matrix2x2;
	typedef glm::mat3x3 Matrix3x3;
	typedef glm::mat4x4 Matrix4x4;
	typedef glm::tvec4<float> Color;
	typedef glm::tvec4<Byte> Color32;


	extern std::shared_ptr<Context> pContext;
	extern void initLog();
	extern void initGraphics(std::shared_ptr<vk::PhysicalDevice> pPhysicalDevice,
		std::shared_ptr<vk::Device> pNativeDevice, vk::Queue graphicsQueue,
		std::shared_ptr<vk::CommandPool> pCommandPool);
}


#endif // !KGS_GLOBAL_H
