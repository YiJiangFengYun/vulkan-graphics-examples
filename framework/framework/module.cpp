
#include "framework/module.hpp"

namespace vgf
{
	Bool32 isInited = VGF_FALSE;
	void moduleCreate(plog::Severity severity, plog::IAppender *appender)
	{
		if (isInited == VGF_TRUE) return;
        plog::IAppender *callerAppender = appender;
		if (appender == nullptr) {
		    //init default log to write to the windows debug output
		    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
			appender = &debugOutputAppender;
		}
		plog::init<VGF_PLOG_ID>(severity, appender);
		fd::moduleCreate(severity, callerAppender);
		vg::moduleCreate(severity, callerAppender);
		vgim::moduleCreate(severity, callerAppender);
		isInited = VGF_TRUE;
	}

	void moduleDestroy()
	{
		vgim::moduleDestory();
		vg::moduleDestory();
		fd::moduleDestroy();
		isInited = VGF_FALSE;
	}

	void setLogSeverity(plog::Severity severity)
	{
		plog::get<VGF_PLOG_ID>()->setMaxSeverity(severity);
	}
} //vgf