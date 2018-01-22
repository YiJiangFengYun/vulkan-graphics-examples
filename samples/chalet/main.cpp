#include <exception>
#include <iostream>
#include <graphics/graphics.hpp>
#include "vulkan/vulkan.h"
#include "chalet/window.hpp"
#include "chalet/app.hpp"

//Main.obj will contain implementions of stb_image and tiny_obj_loader.
#define STB_IMAGE_IMPLEMENTATION
#include "sampleslib/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "sampleslib/tiny_obj_loader.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
int main() {

	vgf::moduleCreate();

	chalet::App app;
	app.init<chalet::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "chalet", vgf::Window::RenderType::RENDERER_3);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	app.run();

	return 0;
}