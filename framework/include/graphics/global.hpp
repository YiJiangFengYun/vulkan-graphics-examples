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


namespace kgs
{
	typedef uint32_t Bool32;
	typedef unsigned char Byte;
	typedef glm::tvec4<float> Color;
	typedef glm::tvec4<Byte> Color32;

	extern void initLog();
	extern void initGraphics();
}


#endif // !KGS_GLOBAL_H
