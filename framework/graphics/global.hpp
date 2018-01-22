#ifndef VG_GLOBAL_H
#define VG_GLOBAL_H

#define NOMINMAX

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#define VG_FALSE 0
#define VG_TRUE 1

#ifdef DEBUG
#define ENABLE_VALIDATION_LAYERS
#endif // DEBUG

#include <cstdint>
#include <foundation/foundation.hpp>
#include "graphics/base.hpp"


namespace vg
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
	typedef fd::Bounds<glm::vec2> Bound2;
	typedef fd::Bounds<glm::vec3> Bound3;
	typedef glm::quat Quaternion;


	typedef vk::PhysicalDevice PhysicalDevice;
	typedef vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;
	typedef vk::Queue Queue;
	typedef vk::SurfaceKHR Surface;

	extern const Color COLOR_ZERO;
	extern const Color COLOR_WHITE;
	extern const Color COLOR_BLACK;
	extern const Color COLOR_BLUE;
	extern const Color COLOR_GREEN;
	extern const Color COLOR_RED;
	extern const Color COLOR_YELLOW;
	extern const Color COLOR_CYAN;
	extern const Color COLOR_GRAY;
	extern const Color COLOR_GREY;
	extern const Color COLOR_MAGENTA;
}


#endif // !VG_GLOBAL_H
