#include <framework/framework.hpp>
#include "triangle/window.hpp"
#include "triangle/app.hpp"

//Main.obj will contain implementions of stb_image and tiny_obj_loader.
#define STB_IMAGE_IMPLEMENTATION
#include "sampleslib/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "sampleslib/tiny_obj_loader.h"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
	vgf::moduleCreate();

	triangle::App triangle;
	triangle.init<triangle::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "triangle");

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	triangle.run();

	return 0;
}