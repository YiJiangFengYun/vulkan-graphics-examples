#ifndef FW_MACRO_H
#define FW_MACRO_H


#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif // !GLFW_INCLUDE_VULKAN

#define FW_FALSE 0
#define FW_TRUE 1

namespace fw
{ 
	using Bool32 = uint32_t;
}

#endif // !FW_MACRO_H



