#include <exception>
#include "vulkan/vulkan.h"
#include "window.hpp"
#include "app.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	app::App app(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.createSubWindow<app::Window>(200, 200, "sub window");
	app.createSubWindow<gfw::Window>(200, 400, "sub window2");
	app.run();

	return 0;
}