#ifndef GFW_MACRO_H
#define GFW_MACRO_H


#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#define GFW_FALSE 0
#define GFW_TRUE 1

namespace gfw
{ 
	using Bool32 = uint32_t;
}

#endif // !GFW_MACRO_H



