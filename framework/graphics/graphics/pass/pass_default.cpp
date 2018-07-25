#include "graphics/pass/pass_default.hpp"

namespace vg
{
    //pre depth
    extern "C" const unsigned char VG_PRE_DEPTH_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_PRE_DEPTH_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_FRAG_DEFAULT_CODE_LEN;

    //light depth
    extern "C" const unsigned char VG_LIGHT_DEPTH_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHT_DEPTH_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_LIGHT_DEPTH_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHT_DEPTH_FRAG_DEFAULT_CODE_LEN;

    //light point distance
    extern "C" const unsigned char VG_LIGHT_POINT_DIST_VERT_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHT_POINT_DIST_VERT_DEFAULT_CODE_LEN;
    extern "C" const unsigned char VG_LIGHT_POINT_DIST_FRAG_DEFAULT_CODE[];
    extern "C" const size_t VG_LIGHT_POINT_DIST_FRAG_DEFAULT_CODE_LEN;

    std::shared_ptr<Shader> pDefaultPreDepthShader = nullptr;
    std::shared_ptr<Pass> pDefaultPreDepthPass = nullptr;
    std::shared_ptr<Shader> pDefaultLightDepthShader = nullptr;
    std::shared_ptr<Pass> pDefaultLightDepthPass = nullptr;
    std::shared_ptr<Shader> pDefaultLightPointDistShader = nullptr;
    std::shared_ptr<Pass> pDefaultLightPointDistPass = nullptr;

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
            pDefaultLightDepthShader = std::shared_ptr<Shader>{ 
                new Shader{VG_LIGHT_DEPTH_VERT_DEFAULT_CODE, 
                           VG_LIGHT_DEPTH_VERT_DEFAULT_CODE_LEN,
                           VG_LIGHT_DEPTH_FRAG_DEFAULT_CODE,
                           VG_LIGHT_DEPTH_FRAG_DEFAULT_CODE_LEN,
                    } 
                };
            pDefaultLightDepthPass = std::shared_ptr<Pass>{ new Pass(pDefaultLightDepthShader.get()) };
    
            vg::Pass::BuildInDataInfo::Component buildInDataCmp = {
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 1u;
            buildInDataInfo.pComponent = &buildInDataCmp;
            pDefaultLightDepthPass->setBuildInDataInfo(buildInDataInfo);

            pDefaultLightDepthPass->setFrontFace(vk::FrontFace::eClockwise);
            pDefaultLightDepthPass->setCullMode(vk::CullModeFlagBits::eBack);

            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pDefaultLightDepthPass->setDepthStencilInfo(depthStencilState);

            //depth bias
            vg::Pass::DepthBiasInfo depthBiasInfo = {
                VG_TRUE,
                VG_FALSE,
                1.0f,
                0.0f,
                1.0f,
            };
            pDefaultLightDepthPass->setDepthBiasInfo(depthBiasInfo);

            pDefaultLightDepthPass->apply();
        }

        //Light point distance shader and pass.
        {
            pDefaultLightPointDistShader = std::shared_ptr<Shader>{ 
                new Shader{VG_LIGHT_POINT_DIST_VERT_DEFAULT_CODE, 
                           VG_LIGHT_POINT_DIST_VERT_DEFAULT_CODE_LEN,
                           VG_LIGHT_POINT_DIST_FRAG_DEFAULT_CODE,
                           VG_LIGHT_POINT_DIST_FRAG_DEFAULT_CODE_LEN,
                    } 
                };
            pDefaultLightPointDistPass = std::shared_ptr<Pass>{ new Pass(pDefaultLightPointDistShader.get()) };
    
            vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
                    {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
                };
            vg::Pass::BuildInDataInfo buildInDataInfo;
            buildInDataInfo.componentCount = 2u;
            buildInDataInfo.pComponent = buildInDataCmps;
            pDefaultLightPointDistPass->setBuildInDataInfo(buildInDataInfo);

            pDefaultLightPointDistPass->setFrontFace(vk::FrontFace::eClockwise);
            pDefaultLightPointDistPass->setCullMode(vk::CullModeFlagBits::eBack);

            vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
            depthStencilState.depthTestEnable = VG_TRUE;
            depthStencilState.depthWriteEnable = VG_TRUE;
            depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
            pDefaultLightPointDistPass->setDepthStencilInfo(depthStencilState);

            pDefaultLightPointDistPass->apply();
        }
        
    }

    void destroyDefaultPasses()
    {
        pDefaultPreDepthShader = nullptr;
        pDefaultPreDepthPass = nullptr;
        pDefaultLightDepthShader = nullptr;
        pDefaultLightDepthPass = nullptr;
        pDefaultLightPointDistShader = nullptr;
        pDefaultLightPointDistPass = nullptr;
    }
} //vg