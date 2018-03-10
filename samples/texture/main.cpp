#include <framework/framework.hpp>

//Main.obj will contain implementions of stb_image.
#define STB_IMAGE_IMPLEMENTATION
#include "sampleslib/stb_image.h"

#include "texture/window.hpp"
#include "texture/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
	vgf::moduleCreate();
	plog::get()->setMaxSeverity(plog::Severity::warning);

	vg::PhysicalDeviceFeatures requiredFeatures;

    vg::PhysicalDeviceFeaturePriorities optionalFeatures;
	optionalFeatures.samplerAnisotropy = 1u;

	App app;
	app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "texture", requiredFeatures, optionalFeatures);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	app.run();

	return 0;
}