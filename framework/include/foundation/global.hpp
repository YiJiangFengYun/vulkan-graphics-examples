#ifndef FD_GLOBAL_H
#define FD_GLOBAL_H

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#define FD_FALSE 0
#define FD_TRUE 1

#include <plog/Log.h>

namespace fd
{
	typedef uint32_t Bool32;

	extern Bool32 isInited;
	extern void init();
}

#endif // !FD_GLOBAL_H
