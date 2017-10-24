#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "sample/window.hpp"
#include "sample/app.hpp"
#include "graphics/global.hpp"

//Main.obj will contain implementions of stb_image and tiny_obj_loader.
#define STB_IMAGE_IMPLEMENTATION
#include "sample/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "sample/tiny_obj_loader.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {

	//kgs::initGraphics();
	gfw::init();

	app::App app;
	app.init<app::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "sample", gfw::Window::RenderType::RENDERER_3);
	//app.createSubWindow<app::SubWindow>(200, 200, "sub window", gfw::Window::RenderType::RENDERER_3);
	//app.createSubWindow<app::SubWindow>(200, 400, "sub window2", gfw::Window::RenderType::RENDERER_3);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	app.run();

	return 0;
}