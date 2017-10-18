#include <exception>
#include <iostream>
#include "vulkan/vulkan.h"
#include "sample/window.hpp"
#include "sample/app.hpp"
#include "graphics/global.hpp"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
std::vector<char> readFile(const std::string& filename);
int main() {

	//kgs::initGraphics();
	gfw::init();

	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");

	app::App app;
	app.init<app::Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.createSubWindow<app::Window>(200, 200, "sub window");
	app.createSubWindow<app::Window>(200, 400, "sub window2");
	app.run();

	return 0;
}

std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Failed to open file!" << std::endl;
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}