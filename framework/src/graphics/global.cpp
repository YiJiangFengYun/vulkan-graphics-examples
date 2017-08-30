#include "graphics/global.hpp"

#include <plog/Log.h>
#include <plog/Appenders/DebugOutputAppender.h>

namespace kgs
{
	void initLog()
	{
		//init default log to write to the windows debug output
		static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
		plog::init(plog::verbose, &debugOutputAppender);
	}

	void initGraphics()
	{
		initLog();
	}
}
