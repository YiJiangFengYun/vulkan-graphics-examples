#ifndef VG_GLOBAL_H
#define VG_GLOBAL_H

#define NOMINMAX

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#define VG_FALSE 0
#define VG_TRUE 1

#include <graphics/config.hpp>

#include <cstdint>
#include <foundation/foundation.hpp>

#include "graphics/vulkan_ext.hpp"

#define VG_PLOG_ID _VG_PLOG_ID
#define VG_LOG(severity) LOG_(VG_PLOG_ID, severity)
#define VG_IF_LOG(severity) IF_LOG_(VG_PLOG_ID, severity)


#define VG_VULKAN_PLOG_ID _VG_VULKAN_PLOG_ID
#define VG_VULKAN_LOG(severity) LOG_(VG_VULKAN_PLOG_ID, severity)
#define VG_VULKAN_IF_LOG(severity) IF_LOG_(VG_VULKAN_PLOG_ID, severity)

#define VG_COST_TIME_PLOG_ID _VG_COST_TIME_PLOG_ID
#define VG_COST_TIME_LOG(severity) LOG_(VG_COST_TIME_PLOG_ID, severity)
#define VG_COST_TIME_IF_LOG(severity) IF_LOG_(VG_COST_TIME_PLOG_ID, severity)

#include "graphics/base.hpp"


namespace vg
{
	using Bool32 = uint32_t;
	using Byte = unsigned char;
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Matrix2x2 = glm::mat2x2;
	using Matrix3x3 = glm::mat3x3;
	using Matrix4x4 = glm::mat4x4;
	using Color = glm::tvec4<float>;
	using Color32 = glm::tvec4<Byte>;
	using Bounds2 = fd::Bounds<glm::vec2>;
	using Bounds3 = fd::Bounds<glm::vec3>;
	using Quaternion = glm::quat;


	using PhysicalDevice = vk::PhysicalDevice;
	using PhysicalDeviceFeatures = vk::PhysicalDeviceFeatures;
	using Queue = vk::Queue;
	using Surface = vk::SurfaceKHR;

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

  template <typename PointType>
  struct PointTypeMapInfo 
  {
      using MatrixType = void;
      using MatrixVectorType = void;
  };

  template<>
  struct PointTypeMapInfo<Vector2>
  {
	  using MatrixType = Matrix3x3;
      using MatrixVectorType = Vector3;
  };

    template<>
  struct PointTypeMapInfo<Vector3>
  {
	  using MatrixType = Matrix4x4;
      using MatrixVectorType = Vector4;
  };

} //vg


#endif // !VG_GLOBAL_H
