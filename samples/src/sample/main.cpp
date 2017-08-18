#include <exception>
#include "framework/app_base.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int main() {
	fw::AppBase app(WINDOW_WIDTH, WINDOW_HEIGHT, "sample");
	app.init();
	app.run();
	return 0;
}