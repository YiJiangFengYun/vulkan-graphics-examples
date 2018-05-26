#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "triangle_2d/window.hpp"
#include "triangle_2d/app.hpp"
#include "graphics/global.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	vgf::moduleCreate(plog::debug);
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::debug, &debugOutputAppender);

	App triangle2D;
	triangle2D.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "triangle_2d");

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	triangle2D.run();

	return 0;
}