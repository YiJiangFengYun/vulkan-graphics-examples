#ifndef VG_IM_MODULE_H
#define VG_IM_MODULE_H

#include "vgim/global.hpp"

namespace vgim {
    extern Bool32 getInited();
    extern void moduleCreate(plog::Severity severity, plog::IAppender *appender = nullptr);
    extern void moduleDestory();
    extern void moduleCreateCanvas(uint32_t canvasWidth, uint32_t canvasHeight);
    extern void setLogSeverity(plog::Severity severity);
    extern uint32_t getCanvasWidth();
    extern uint32_t getCanvasHeight();
    extern vg::Material *getMaterial();
    extern vg::DimSimpleMesh2 *getMesh();
    extern vg::VisualObject2 *getUIObject();
    extern vg::CameraOP2 *getCamera();
    extern vg::Scene2 *getScene();
    extern void setShaderPath(const std::string &vertShaderPath, const std::string &fragShaderPat);
    extern void updateIMGUI(uint32_t canvasWidth
        , uint32_t canvasHeight
        , double currTime
        , float mouseX
        , float mouseY
        , Bool32 mouseDown[3]
        );    
    extern void updateImGUIRender();
} //vgim
#endif //VG_IM_MODULE_H