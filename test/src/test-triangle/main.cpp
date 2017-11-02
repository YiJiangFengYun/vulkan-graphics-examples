#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "test-triangle/window.hpp"
#include "test-triangle/app.hpp"
#include "graphics/global.hpp"

//Main.obj will contain implementions of stb_image and tiny_obj_loader.
#define STB_IMAGE_IMPLEMENTATION
#include "testlib/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "testlib/tiny_obj_loader.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {

	//kgs::initGraphics();
	gfw::moduleCreate();

	testTriangle::App testTriangle;
	testTriangle.init<testTriangle::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "test-triangle", gfw::Window::RenderType::RENDERER_3);
	//testTriangle.createSubWindow<testTriangle::Window>(200, 200, "sub window", gfw::Window::RenderType::RENDERER_3);
	//testTriangle.createSubWindow<testTriangle::Window>(200, 400, "sub window2", gfw::Window::RenderType::RENDERER_3);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	testTriangle.run();

	return 0;
}