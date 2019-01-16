#include "graphics/renderer/renderer_pass.hpp"

namespace vg
{

    const std::array<uint32_t, static_cast<size_t>(Pass::BuildInDataType::COUNT)> buildInDataTypeSizes = {
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MAIN_CLOLOR>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_PROJECTION>::Type)
    };

    RendererPass::_BuildInDataCache::_BuildInDataCache(Matrix4x4 matrixObjectToNDC
        , Color mainColor
        , Matrix4x4 matrixObjectToWorld
        , Matrix4x4 matrixObjectToView
        , Matrix4x4 matrixView
        , Matrix4x4 matrixProjection
        , Vector4 posViewer
        )
        : matrixObjectToNDC(matrixObjectToNDC)
        , mainColor(mainColor)
        , matrixObjectToWorld(matrixObjectToWorld)
        , matrixObjectToView(matrixObjectToView)
        , matrixView(matrixView)
        , matrixProjection(matrixProjection)
        , posViewer(posViewer)
    {

    }

    RendererPass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
        , posViewer(target.posViewer)
    {

    }

    RendererPass::_BuildInDataCache &RendererPass::_BuildInDataCache::operator=(const _BuildInDataCache &target)
    {
        matrixObjectToNDC = target.matrixObjectToNDC;
        mainColor = target.mainColor;
        matrixObjectToWorld = target.matrixObjectToWorld;
        matrixObjectToView = target.matrixObjectToView;
        matrixView = target.matrixView;
        matrixProjection = target.matrixProjection;
        posViewer = target.posViewer;
        return *this;
    }

    RendererPass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &&target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
        , posViewer(target.posViewer)
    {
    }

///RendererPass
    RendererPass::RendererPass()
        : m_pPass()
        , m_buildInDataCache()
        , m_bindingSet()
 
    {
        _initBuildInData();
    }
        
    RendererPass::RendererPass(vg::Pass pass)
    {
        
    }

    void RendererPass::setBuildInDataMatrix4x4(Pass::BuildInDataType type, Matrix4x4 matrix)
    {
        _updateBuildInData(type, matrix);
    }


    void RendererPass::setBuildInDataVector4(Pass::BuildInDataType type, Vector4 vector)
    {
        _updateBuildInData(type, vector);
    }

    void RendererPass::_initBuildInData()
    {
        Pass::BuildInDataInfo buildInDataInfo;
        if (m_pPass != nullptr) buildInDataInfo = m_pPass->getBuildInDataInfo();
        uint32_t size = 0u;
        auto componentCount = buildInDataInfo.componentCount;
        for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
        {
            Pass::BuildInDataType type = (*(buildInDataInfo.pComponent + componentIndex)).type;
            uint32_t count = static_cast<uint32_t>(Pass::BuildInDataType::COUNT);
            for (uint32_t i = 0; i < count; ++i)
            {
                if (i == static_cast<uint32_t>(type))
                {
                    size += buildInDataTypeSizes[i];
                    break;
                }
            }
        }

        if (size > 0)
        {
            PassDataInfo info = {
                VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY,
                vk::ShaderStageFlagBits::eAllGraphics,
            };
            PassDataSizeInfo sizeInfo = {
                size,
            };
            if (m_bindingSet.hasData(VG_PASS_BUILDIN_DATA_NAME) == VG_FALSE)
            {
                m_bindingSet.addData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
            }
            else
            {
                m_bindingSet.setData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
            }

            uint32_t offset1 = 0u;
            for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
            {
                Pass::BuildInDataType type = (*(buildInDataInfo.pComponent + componentIndex)).type;
                uint32_t count = static_cast<uint32_t>(Pass::BuildInDataType::COUNT);
                uint32_t offset2= 0u;
                for (uint32_t i = 0; i < count; ++i)
                {
                    if (i == static_cast<uint32_t>(type))
                    {
                        m_bindingSet.setData(VG_PASS_BUILDIN_DATA_NAME
                            , ((char *)(&m_buildInDataCache) + offset2)
                            , buildInDataTypeSizes[static_cast<uint32_t>(type)]
                            , offset1);
                        break;
                    }
                    else
                    {
                        offset2 += buildInDataTypeSizes[i];
                    }
                }
                offset1 += buildInDataTypeSizes[static_cast<uint32_t>(type)];
            }
        }
        else if (m_bindingSet.hasData(VG_PASS_BUILDIN_DATA_NAME))
        {
            m_bindingSet.removeData(VG_PASS_BUILDIN_DATA_NAME);
        }
    }


} //vg