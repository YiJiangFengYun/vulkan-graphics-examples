#include "graphics/light/light_direct_3.hpp"

namespace vg
{
    const uint32_t LightDirect3::DEFAULT_DEPTH_TEXTURE_WIDTH = 1280u;
    const uint32_t LightDirect3::DEFAULT_DEPTH_TEXTURE_HEIGHT = 1280u;
    const vk::Format LightDirect3::DEFAULT_FORMAT = vk::Format::eD32Sfloat;
    const float LightDirect3::DEFAULT_WIDTH = 100.0f;
    const float LightDirect3::DEFAULT_HEIGHT = 100.0f;
    const float LightDirect3::DEFAULT_RANGE = 100.0f;
    LightDirect3::LightDirect3(float width
        , float height
        , float range
        , uint32_t depthTextureWidth
        , uint32_t depthTextureHeight
        , vk::Format format
        )
        : Light3()
        , m_width(width)
        , m_height(height)
        , m_range(range)
        , m_strength(1.0f)
        , m_pDepthTarget()
        , m_refDepthTarget()
        , m_pProjector()
        , m_refProjector() 
    {
        m_pDepthTarget = std::shared_ptr<LightDepthTarget2D>{ new LightDepthTarget2D(depthTextureWidth, depthTextureHeight, format)};
        m_refDepthTarget = m_pDepthTarget.get();
        m_pProjector = std::shared_ptr<ProjectorOP3>{new ProjectorOP3()};
        _setProjector();
        Matrix4x4 transform(1.0f);
        m_pProjector->setTransformMatrix(transform);
        m_refProjector = m_pProjector.get();

        _setRange();
        _setStrength();
        _setDepthTexture();
        _apply();
    }

    LightDepthRenderInfo LightDirect3::getDepthRenderInfo() const
    {
        LightDepthRenderInfo info = {
            1u,
            reinterpret_cast<const BaseProjector *const *>(&m_refProjector),
            reinterpret_cast<const OnceRenderTarget *const *>(&m_refDepthTarget),
        };
        return info;
    }

    float LightDirect3::getWidth() const
    {
        return m_width;
    }

    void LightDirect3::setWidth(float value)
    {
        m_width = value;
        _setProjector();
    }
        
    float LightDirect3::getHeight() const
    {
        return m_height;
    }

    void LightDirect3::setHeight(float value)
    {
        m_height = value;
        _setProjector();
    }
        
    float LightDirect3::getRange() const
    {
        return m_range;
    }
        
    void LightDirect3::setRange(float value)
    {
        m_range = value;
        _setProjector();
        _setRange();
        _apply();
    }
        
    vg::Vector3 LightDirect3::getStrength() const
    {
        return m_strength;
    }
    
    void LightDirect3::setStrength(vg::Vector3 value)
    {
        m_strength = value;
        _setStrength();
        _apply();
    }

    void LightDirect3::_beginRender()
    {
        Light3::_beginRender();

        //sync transform between camera and projector.
        m_pProjector->setLocalToWorldMatrix(m_pTransform->getMatrixLocalToWorld());
        //sync projection data.
        _setProjection();
        _apply();
    }

    void LightDirect3::_setProjector()
    {
        m_pProjector->updateProj(
            vg::Bounds3(
                vg::Vector3(- m_width / 2, - m_height / 2, 0u),
                vg::Vector3(m_width / 2, m_height / 2, m_range)
            )
        );
    }

    void LightDirect3::_setRange()
    {
        if (m_data.hasData(VG_LIGHT_DIRECT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DIRECT3_DATA_RANGE_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_DIRECT3_DATA_RANGE_NAME, info, m_range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_DIRECT3_DATA_RANGE_NAME, m_range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_DIRECT3_DATA_RANGE_NAME] = VG_TRUE;
    }

    void LightDirect3::_setProjection()
    {
        auto matrix = m_space.getVulkanProjMatrix(m_pProjector->getProjMatrix());
        if (m_data.hasData(VG_LIGHT_DATA_PROJECTION_NAME) == VG_FALSE) {
            LightDataInfo dataInfo = {
                VG_LIGHT_DATA_PROJECTION_LAYOUT_PRORITY
            };
            m_data.addData(VG_LIGHT_DATA_PROJECTION_NAME, dataInfo, matrix);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_DATA_PROJECTION_NAME, matrix);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_DATA_PROJECTION_NAME] = VG_TRUE;
    }

    void LightDirect3::_setStrength()
    {
        if (m_data.hasData(VG_LIGHT_DIRECT3_DATA_STRENGTH_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DIRECT3_DATA_STRENGTH_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_DIRECT3_DATA_STRENGTH_NAME, info, m_strength);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_DIRECT3_DATA_STRENGTH_NAME, m_strength);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_DIRECT3_DATA_STRENGTH_NAME] = VG_TRUE;
    }

    void LightDirect3::_setDepthTexture()
    {
        LightTextureInfo texInfo = {
            SamplerTextureType::TEX_2D,
            VG_LIGHT_TEXTURE_DEPTH_BINDING_PRIORITY,
            m_pDepthTarget->getDepthTargetTexture()
            };
        if (m_data.hasTexture(VG_LIGHT_TEXTURE_DEPTH_NAME) == VG_FALSE)
        {
            m_data.addTexture(VG_LIGHT_TEXTURE_DEPTH_NAME, texInfo);
        } else {
            m_data.setTexture(VG_LIGHT_TEXTURE_DEPTH_NAME, texInfo);
        }
        m_textureChanged = VG_TRUE;
    }
} //vg