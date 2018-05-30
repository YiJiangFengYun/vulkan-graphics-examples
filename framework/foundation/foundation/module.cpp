#include "foundation/module.hpp"

namespace fd
{
    Bool32 isInited = FD_FALSE;
    void moduleCreate(plog::Severity severity, plog::IAppender *appender)
    {
        if (isInited == FD_TRUE) return;
        if (appender == nullptr) {
            //init default log to write to the windows debug output
            static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
            appender = &debugOutputAppender;
        }
        plog::init<FD_PLOG_ID>(severity, appender);

        isInited = FD_TRUE;
    }

    void moduleDestroy()
    {
        isInited = FD_FALSE;
    }

    void setLogSeverity(plog::Severity severity)
    {
        plog::get<FD_PLOG_ID>()->setMaxSeverity(severity);
    }
}