#include <exception>
#include "framework/app/app_base.hpp"
#include "vulkan/vulkan.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	gfw::AppBase app(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.createSubWindow(200, 200, "sub window");
	app.run();

	return 0;
}