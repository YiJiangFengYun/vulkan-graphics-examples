#include "framework/global.hpp"

#include "graphics/global.hpp"
#include "foundation/global.hpp"

namespace gfw
{
	Bool32 isInited = GFW_FALSE;
	void moduleCreate()
	{
		if (isInited == GFW_TRUE) return;
		fd::moduleCreate();
		isInited = GFW_TRUE;
	}

	void moduleDestroy()
	{
		fd::moduleDestroy();
		isInited = GFW_FALSE;
	}
}
