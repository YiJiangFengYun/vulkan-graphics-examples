
#include "framework/module.hpp"

namespace vgf
{
	Bool32 isInited = VGF_FALSE;
	void moduleCreate(plog::Severity severity, plog::IAppender *appender = nullptr)
	{
		if (isInited == VGF_TRUE) return;
		// if (appender == nullptr) {
		//     //init default log to write to the windows debug output
		//     static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
		// 	appender = &DebugOutputAppender;
		// }
		// plog::init<FD_PLOG_ID>(severity, appender);

		isInited = VGF_TRUE;
	}

	void moduleDestroy()
	{
		isInited = VGF_FALSE;
	}
} //vgf