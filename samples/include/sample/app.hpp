#ifndef APP_H
#define APP_H

#include "framework/app/app_base.hpp"

namespace app
{
	class App : public gfw::AppBase
	{
	public:
		App(uint32_t width, uint32_t height, const char *title);

	private:

	};
}

#endif // !APP_H
