#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "test-triangle-2d/window.hpp"
#include "test-triangle-2d/app.hpp"
#include "graphics/global.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {
	vgf::moduleCreate(plog::debug);
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::debug, &debugOutputAppender);

	testTriangle2D::App testTriangle2D;
	testTriangle2D.init<testTriangle2D::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "test-triangle-2d");
	//testTriangle.createSubWindow<testTriangle::Window>(200, 200, "sub window", gfw::Window::RenderType::RENDERER_3);
	//testTriangle.createSubWindow<testTriangle::Window>(200, 400, "sub window2", gfw::Window::RenderType::RENDERER_3);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	testTriangle2D.run();

	return 0;
}