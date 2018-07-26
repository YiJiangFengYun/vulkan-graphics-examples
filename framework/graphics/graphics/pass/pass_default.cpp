#include "graphics/pass/pass_default.hpp"

namespace vg
{
    //pre depth
    extern "C" const unsigned char VG_PRE_DEPTH_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_PRE_DEPTH_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_FRAG_DEFAULT_CODE_LEN;

    //light depth
    extern "C" const unsigned char VG_LIGHTING_DEPTH_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHTING_DEPTH_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_LIGHTING_DEPTH_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHTING_DEPTH_FRAG_DEFAULT_CODE_LEN;

    //light point distance
    extern "C" const unsigned char VG_LIGHTING_POINT_DIST_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHTING_POINT_DIST_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_LIGHTING_POINT_DIST_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHTING_POINT_DIST_FRAG_DEFAULT_CODE_LEN;

    std::shared_ptr<Shader> pDefaultPreDepthShader = nullptr;
    std::shared_ptr<Pass> pDefaultPreDepthPass = nullptr;
    std::shared_ptr<Shader> pDefaultLightingDepthShader = nullptr;
    std::shared_ptr<Pass> pDefaultLightingDepthPass = nullptr;
    std::shared_ptr<Shader> pDefaultLightingPointDistShader = nullptr;
    std::shared_ptr<Pass> pDefaultLightingPointDistPass = nullptr;

    void createDefaultPasses()
    {
        //pre depth shader and pass.
        {
            pDefaultPreDepthShader = std::shared_ptr<Shader>{ 
                new Shader{VG_PRE_DEPTH_VERT_DEFAULT_CODE, 
                           VG_PRE_DEPTH_VERT_DEFAULT_CODE_LEN,
                           VG_PRE_DEPTH_FRAG_DEFAULT_CODE,
                           VG_PRE_DEPTH_FRAG_DEFAULT_CODE_LEN,
                    } 
                };
            pDefaultPreDepthPass = std::shared_ptr<Pass>{ new Pass(pDefaultPreDepthShader.get()) };
    
            vg::Pass::BuildInDataInfo::Component buildInDataCmp = {
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 1u;
            buildInDataInfo.pComponent = &buildInDataCmp;
            pDefaultPreDepthPass->setBuildInDataInfo(buildInDataInfo);

            pDefaultPreDepthPass->setFrontFace(vk::FrontFace::eClockwise);
            pDefaultPreDepthPass->setCullMode(vk::CullModeFlagBits::eBack);

            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pDefaultPreDepthPass->setDepthStencilInfo(depthStencilState);

            pDefaultPreDepthPass->apply();
        }

        //Light depth shader and pass.
        {
            pDefaultLightingDepthShader = std::shared_ptr<Shader>{ 
                new Shader{VG_LIGHTING_DEPTH_VERT_DEFAULT_CODE, 
                           VG_LIGHTING_DEPTH_VERT_DEFAULT_CODE_LEN,
                           VG_LIGHTING_DEPTH_FRAG_DEFAULT_CODE,
                           VG_LIGHTING_DEPTH_FRAG_DEFAULT_CODE_LEN,
                    } 
                };
            pDefaultLightingDepthPass = std::shared_ptr<Pass>{ new Pass(pDefaultLightingDepthShader.get()) };
    
            vg::Pass::BuildInDataInfo::Component buildInDataCmp = {
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 1u;
            buildInDataInfo.pComponent = &buildInDataCmp;
            pDefaultLightingDepthPass->setBuildInDataInfo(buildInDataInfo);

            pDefaultLightingDepthPass->setFrontFace(vk::FrontFace::eClockwise);
            pDefaultLightingDepthPass->setCullMode(vk::CullModeFlagBits::eBack);

            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pDefaultLightingDepthPass->setDepthStencilInfo(depthStencilState);

            //depth bias
            vg::Pass::DepthBiasInfo depthBiasInfo = {
                VG_TRUE,
                VG_FALSE,
                1.0f,
                0.0f,
                1.0f,
            };
            pDefaultLightingDepthPass->setDepthBiasInfo(depthBiasInfo);

            pDefaultLightingDepthPass->apply();
        }

        //Light point distance shader and pass.
        {
            pDefaultLightingPointDistShader = std::shared_ptr<Shader>{ 
                new Shader{VG_LIGHTING_POINT_DIST_VERT_DEFAULT_CODE, 
                           VG_LIGHTING_POINT_DIST_VERT_DEFAULT_CODE_LEN,
                           VG_LIGHTING_POINT_DIST_FRAG_DEFAULT_CODE,
                           VG_LIGHTING_POINT_DIST_FRAG_DEFAULT_CODE_LEN,
                    } 
                };
            pDefaultLightingPointDistPass = std::shared_ptr<Pass>{ new Pass(pDefaultLightingPointDistShader.get()) };
    
            vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
                };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 2u;
            buildInDataInfo.pComponent = buildInDataCmps;
            pDefaultLightingPointDistPass->setBuildInDataInfo(buildInDataInfo);

            pDefaultLightingPointDistPass->setFrontFace(vk::FrontFace::eClockwise);
            pDefaultLightingPointDistPass->setCullMode(vk::CullModeFlagBits::eBack);

            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pDefaultLightingPointDistPass->setDepthStencilInfo(depthStencilState);

            pDefaultLightingPointDistPass->apply();
        }
        
    }

    void destroyDefaultPasses()
    {
        pDefaultPreDepthShader = nullptr;
        pDefaultPreDepthPass = nullptr;
        pDefaultLightingDepthShader = nullptr;
        pDefaultLightingDepthPass = nullptr;
        pDefaultLightingPointDistShader = nullptr;
        pDefaultLightingPointDistPass = nullptr;
    }
} //vg