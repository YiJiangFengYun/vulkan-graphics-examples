#include <exception>
#include "vulkan/vulkan.h"
#include "sample/window.hpp"
#include "sample/app.hpp"
#include "graphics/global.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {

	//kgs::initGraphics();

	app::App app;
	app.init<app::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.createSubWindow<app::Window>(200, 200, "sub window");
	app.createSubWindow<app::Window>(200, 400, "sub window2");
	app.run();

	return 0;
}