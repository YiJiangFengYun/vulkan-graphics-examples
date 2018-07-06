#include <framework/framework.hpp>

#include "cubemaps/window.hpp"
#include "cubemaps/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
    vgf::moduleCreate(plog::warning);
    // vg::setLogSeverity(plog::debug);
    // vg::setCostTimeLogSeverity(plog::debug);
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::debug, &debugOutputAppender);

    vg::setVulkanLogSeverity(plog::info);

    vg::PhysicalDeviceFeatures requiredFeatures;

    vg::PhysicalDeviceFeaturePriorities optionalFeatures;
    optionalFeatures.samplerAnisotropy = 1u;
    optionalFeatures.textureCompressionBC = 1u;
    optionalFeatures.textureCompressionASTC_LDR = 1u;
    optionalFeatures.textureCompressionETC2 = 1u;

    App app;
    app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "cubemaps", requiredFeatures, optionalFeatures);

    LOG(plog::debug) << "Initialization completed." << std::endl;

    LOG(plog::debug) << "Start to app run loop." << std::endl;
    app.run();

    return 0;
}