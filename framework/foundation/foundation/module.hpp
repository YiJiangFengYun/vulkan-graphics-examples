#ifndef FD_MODULE_HPP
#define FD_MODULE_HPP

#include "foundation/global.hpp"

namespace fd
{
    extern Bool32 isInited;
    extern void moduleCreate(plog::Severity severity, plog::IAppender *appender = nullptr);
    extern void moduleDestroy();
    extern void setLogSeverity(plog::Severity severity);
} // fd

#endif //FD_MODULE_HPP