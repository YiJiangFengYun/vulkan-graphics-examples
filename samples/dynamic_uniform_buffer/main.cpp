#include <framework/framework.hpp>

#include "dynamic_uniform_buffer/window.hpp"
#include "dynamic_uniform_buffer/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
	vgf::moduleCreate(plog::warning);
	static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
	plog::init(plog::debug, &debugOutputAppender);

	vg::PhysicalDeviceFeatures requiredFeatures;

    vg::PhysicalDeviceFeaturePriorities optionalFeatures;
	optionalFeatures.samplerAnisotropy = 1u;

	App app;
	app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "dynamic uniform buffer", requiredFeatures, optionalFeatures);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	app.run();

	return 0;
}