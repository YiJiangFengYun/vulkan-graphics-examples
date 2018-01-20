#ifndef FD_GLOBAL_H
#define FD_GLOBAL_H

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#define FD_FALSE 0
#define FD_TRUE 1

#define NOMINMAX

#include <plog/Log.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/hash.hpp>

namespace fd
{
	typedef uint32_t Bool32;

	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestroy();
}

#endif // !FD_GLOBAL_H
