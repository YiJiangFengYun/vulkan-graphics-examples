#include <framework/framework.hpp>

#include "pre_depth_tex/window.hpp"
#include "pre_depth_tex/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
    vgf::moduleCreate(plog::warning);
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::debug, &debugOutputAppender);

    vg::PhysicalDeviceFeatures requiredFeatures;

    vg::PhysicalDeviceFeaturePriorities optionalFeatures;
    optionalFeatures.samplerAnisotropy = 1u;
    optionalFeatures.textureCompressionBC = 1u;
    optionalFeatures.textureCompressionASTC_LDR = 1u;
    optionalFeatures.textureCompressionETC2 = 1u;
    optionalFeatures.fillModeNonSolid = 1u;

    App app;
    app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "pre_depth_tex", requiredFeatures, optionalFeatures);

    LOG(plog::debug) << "Initialization completed." << std::endl;

    LOG(plog::debug) << "Start to app run loop." << std::endl;
    app.run();

    return 0;
}