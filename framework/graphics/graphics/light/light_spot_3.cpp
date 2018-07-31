#include "graphics/light/light_spot_3.hpp"

namespace vg
{
    const uint32_t LightSpot3::DEFAULT_DEPTH_TEXTURE_WIDTH = 1280u;
    const uint32_t LightSpot3::DEFAULT_DEPTH_TEXTURE_HEIGHT = 1280u;
    const float LightSpot3::DEFAULT_RANGE = 100.0f;
    const float LightSpot3::DEFAULT_FOV = glm::radians(45.0f);
    static const vk::Format DEFAULT_FORMAT = vk::Format::eD32Sfloat;
    LightSpot3::LightSpot3(float fov
        , float range
        , uint32_t depthTextureWidth
        , uint32_t depthTextureHeight
        , vk::Format format
        )
        : Light3()
        , m_fov(fov)
        , m_range(range)
        , m_strength(1.0f)
        , m_pDepthTarget()
        , m_refDepthTarget()
        , m_pProjector()
        , m_refProjector()
    {
        m_pDepthTarget = std::shared_ptr<LightDepthTarget2D>{ new LightDepthTarget2D(depthTextureWidth, depthTextureHeight, format)};
        m_refDepthTarget = m_pDepthTarget.get();
        m_pProjector = std::shared_ptr<Projector3>{new Projector3()};
        _setProjector();
        Matrix4x4 transform(1.0f);
        m_pProjector->setTransformMatrix(transform);
        m_refProjector = m_pProjector.get();

        //add range to lgiht data
        _setRange();

        _setStrength();

        //add the depth texture to light textures.
        _setDepthTexture();
        _apply();
    }

    LightDepthRenderInfo LightSpot3::getDepthRenderInfo() const
    {
        LightDepthRenderInfo info = {
            1u,
            reinterpret_cast<const BaseProjector *const *>(&m_refProjector),
            reinterpret_cast<const OnceRenderTarget *const *>(&m_refDepthTarget),
        };
        return info;
    }

    float LightSpot3::getFOV() const
    {
        return m_fov;
    }

    void LightSpot3::setFOV(float value)
    {
        m_fov = value;
        auto range = m_range;
        m_pProjector->updateProj(m_fov, 1.0f, std::min(1.0f, range), range);
    }

    float LightSpot3::getRange() const
    {
        return m_range;
    }

    void LightSpot3::setRange(float range)
    {
        m_range = range;
        _setProjector();
        _setRange();
        _apply();
    }

    vg::Vector3 LightSpot3::getStrength() const
    {
        return m_strength;
    }

    void LightSpot3::setStrength(vg::Vector3 value)
    {
        m_strength = value;
        _setStrength();
        _apply();
    }

    void LightSpot3::_beginRender()
    {
        Light3::_beginRender();

        //sync transform between camera and projector.
        m_pProjector->setLocalToWorldMatrix(m_pTransform->getMatrixLocalToWorld());
        //sync projection data.
        _setProjection();
        _apply();
    }

    void LightSpot3::_setProjector()
    {
        m_pProjector->updateProj(m_fov, 1.0f, std::min(1.0f, m_range), m_range);
    }

    void LightSpot3::_setRange()
    {
        if (m_data.hasData(VG_LIGHT_SPOT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_SPOT3_DATA_RANGE_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_SPOT3_DATA_RANGE_NAME, info, m_range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_SPOT3_DATA_RANGE_NAME, m_range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_SPOT3_DATA_RANGE_NAME] = VG_TRUE;
    }

    void LightSpot3::_setProjection()
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

    void LightSpot3::_setStrength()
    {
        if (m_data.hasData(VG_LIGHT_SPOT3_DATA_STRENGTH_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_SPOT3_DATA_STRENGTH_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_SPOT3_DATA_STRENGTH_NAME, info, m_strength);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_SPOT3_DATA_STRENGTH_NAME, m_strength);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_SPOT3_DATA_STRENGTH_NAME] = VG_TRUE;
    }

    void LightSpot3::_setDepthTexture()
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