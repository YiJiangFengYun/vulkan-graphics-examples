#ifndef VGF_MODULE_HPP
#define VGF_MODULE_HPP

#include "framework/global.hpp"

namespace vgf
{
    extern Bool32 isInited;
	extern void moduleCreate(plog::Severity severity, plog::IAppender *appender = nullptr);
	extern void moduleDestory();
} // vgf

#endif // VGF_MODULE_HPP