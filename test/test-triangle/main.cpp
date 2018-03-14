#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "test-triangle/window.hpp"
#include "test-triangle/app.hpp"
#include "graphics/global.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	vgf::moduleCreate(plog::debug);
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::debug, &debugOutputAppender);

	testTriangle::App testTriangle;
	testTriangle.init<testTriangle::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "test-triangle");
	// testTriangle.createSubWindow<testTriangle::Window>(200, 200, "sub window");
	//testTriangle.createSubWindow<testTriangle::Window>(200, 400, "sub window2");

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	testTriangle.run();

	return 0;
}