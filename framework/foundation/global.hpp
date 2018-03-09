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
/**
 * Using glm left handed means to use positive z at camera projectition, 
 * otherwise using glm right handed means to use nagetive z. 
 **/
#define GLM_FORCE_LEFT_HANDED
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/hash.hpp>

namespace fd
{
	using Bool32 = uint32_t;

	extern Bool32 isInited;
	extern void moduleCreate();
	extern void moduleDestroy();
}

#endif // !FD_GLOBAL_H
