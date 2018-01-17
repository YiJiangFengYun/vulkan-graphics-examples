#include "foundation/global.hpp"

#include <plog/Appenders/DebugOutputAppender.h>

namespace fd
{
	Bool32 isInited = FD_FALSE;
	void moduleCreate()
	{
		if (isInited == FD_TRUE) return;
		//init default log to write to the windows debug output
		static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
		plog::init(plog::verbose, &debugOutputAppender);

		isInited = FD_TRUE;
	}

	void moduleDestroy()
	{
		isInited = FD_FALSE;
	}
}