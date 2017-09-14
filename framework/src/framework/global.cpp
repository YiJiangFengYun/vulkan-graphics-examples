#include "framework/global.hpp"

#include "graphics/global.hpp"
namespace gfw
{
	Bool32 isInited = GFW_FALSE;
	void init()
	{
		isInited = GFW_TRUE;
	}
}
