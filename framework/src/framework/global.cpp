#include "framework/global.hpp"

#include "graphics/global.hpp"
#include "foundation/global.hpp"

namespace gfw
{
	Bool32 isInited = GFW_FALSE;
	void init()
	{
		if (isInited == GFW_TRUE) return;
		fd::init();
		//kgs::init();
		isInited = GFW_TRUE;
	}
}
