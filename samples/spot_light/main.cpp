#include <framework/framework.hpp>
#include "spot_light/window.hpp"
#include "spot_light/app.hpp"

const uint32_t WINDOW_WIDTH = 1280;
const uint32_t WINDOW_HEIGHT = 720;
int main() {
    vgf::moduleCreate(plog::warning);
    vg::setLogSeverity(plog::debug);
    // vg::setCostTimeLogSeverity(plog::debug);
    vg::setVulkanLogSeverity(plog::debug);
    static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
    plog::init(plog::debug, &debugOutputAppender);


    App app;
    app.init<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "spot light");

    LOG(plog::debug) << "Initialization completed." << std::endl;

    LOG(plog::debug) << "Start to app run loop." << std::endl;
    app.run();

    return 0;
}