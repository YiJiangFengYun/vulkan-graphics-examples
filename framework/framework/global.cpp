#include "framework/global.hpp"

#include "graphics/global.hpp"
#include "foundation/global.hpp"

namespace vgf
{
	Bool32 isInited = VGF_FALSE;
	void moduleCreate()
	{
		if (isInited == VGF_TRUE) return;
		fd::moduleCreate();
		isInited = VGF_TRUE;
	}

	void moduleDestroy()
	{
		fd::moduleDestroy();
		isInited = VGF_FALSE;
	}
}
