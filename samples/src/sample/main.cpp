#include <exception>
#include "framework/app_base.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int main() {
	fw::AppBase app;
	app.init(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.run();
	/*glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();*/

	return 0;
}