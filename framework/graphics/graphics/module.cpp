#include "graphics/module.hpp"

namespace vg
{
    Bool32 isInited = VG_FALSE;
    Bool32 isCreated = VG_FALSE;    
    Bool32 isCreatedVkInstance = VG_FALSE;
    Bool32 isCreatedOther = VG_FALSE;
    void moduleCreate(plog::Severity severity, plog::IAppender *appender)
    {
        if (isCreated == VG_TRUE) return;
        isCreated = VG_TRUE;
        plog::IAppender *callerAppender = appender;
        if (appender == nullptr) {
            //init default log to write to the windows debug output
            static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
            appender = &debugOutputAppender;
        }
        plog::init<VG_PLOG_ID>(severity, appender);
        plog::init<VG_VULKAN_PLOG_ID>(severity, appender);
        plog::init<VG_COST_TIME_PLOG_ID>(severity, appender);
        fd::moduleCreate(severity, callerAppender);
    }

    void moduleCreateVkinstance(std::string name, uint32_t version, std::vector<const char*> vkExtensions)
    {
        if (isCreatedVkInstance == VG_TRUE) return;
        isCreatedVkInstance = VG_TRUE; 
        pApp = std::shared_ptr<Application>(new Application(name, version));
        pApp->initCreateVkInstance(vkExtensions);
    }

    void moduleCreateOther(std::shared_ptr<vk::SurfaceKHR> pSurface
        , uint32_t graphicsQueueCount
        , uint32_t presentQueueCount
        , vg::PhysicalDeviceFeatures requiredPhysicalDeviceFeatures
        , vg::PhysicalDeviceFeaturePriorities optionalPhysicalDeviceFeatures
    )
    {
        if (isCreatedOther == VG_TRUE) return;
        isCreatedOther = VG_TRUE;
        pApp->initOther(pSurface.get()
            , graphicsQueueCount
            , presentQueueCount
            , requiredPhysicalDeviceFeatures
            , optionalPhysicalDeviceFeatures
            );

        createDefaultTextures();
        createDefaultPasses();
        createDefaultMaterials();
        isInited = VG_TRUE;
    }

    void moduleDestory()
    {
        destroyDefaultTextures();
        destroyDefaultPasses();
        destroyDefaultMaterials();
        pApp = nullptr;
        //fd::moduleDestroy();
        isInited = VG_FALSE;
    }

    void setLogSeverity(plog::Severity severity)
    {
        plog::get<VG_PLOG_ID>()->setMaxSeverity(severity);
    }

    void setVulkanLogSeverity(plog::Severity severity)
    {
        plog::get<VG_VULKAN_PLOG_ID>()->setMaxSeverity(severity);
    }

    void setCostTimeLogSeverity(plog::Severity severity)
    {
        plog::get<VG_COST_TIME_PLOG_ID>()->setMaxSeverity(severity);
    }
}
