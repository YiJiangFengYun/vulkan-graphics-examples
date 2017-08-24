#include <exception>
#include "framework/app/app_base.hpp"
#include "vulkan/vulkan.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	fw::AppBase app(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.run();
	return 0;
}