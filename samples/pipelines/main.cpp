#include <framework/framework.hpp>

#include "pipelines/window.hpp"
#include "pipelines/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
	vgf::moduleCreate();
	plog::get()->setMaxSeverity(plog::Severity::warning);

	vg::PhysicalDeviceFeatures requiredFeatures;

    vg::PhysicalDeviceFeaturePriorities optionalFeatures;
	optionalFeatures.fillModeNonSolid = 1u;
	optionalFeatures.wideLines = 1u;

	App app;
	app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "pipelines", requiredFeatures, optionalFeatures);

	LOG(plog::debug) << "Initialization completed." << std::endl;

	LOG(plog::debug) << "Start to app run loop." << std::endl;
	app.run();

	return 0;
}