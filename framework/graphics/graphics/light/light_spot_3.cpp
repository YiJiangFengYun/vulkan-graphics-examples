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
        , m_pDepthTarget()
        , m_refDepthTarget()
        , m_pProjector()
        , m_refProjector()
    {
        m_pDepthTarget = std::shared_ptr<LightDepthTarget2D>{ new LightDepthTarget2D(depthTextureWidth, depthTextureHeight, format)};
        m_refDepthTarget = m_pDepthTarget.get();
        m_pProjector = std::shared_ptr<Projector3>{new Projector3()};
        _resetProjector();
        Matrix4x4 transform(1.0f);
        m_pProjector->setTransformMatrix(transform);
        m_refProjector = m_pProjector.get();

        //add range to lgiht data
        if (m_data.hasData(LIGHT_SPOT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY,
            };
            m_data.addData(LIGHT_SPOT3_DATA_RANGE_NAME, info, range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(LIGHT_SPOT3_DATA_RANGE_NAME, range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[LIGHT_SPOT3_DATA_RANGE_NAME] = VG_TRUE;

        //add the depth texture to light textures.
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
        _resetProjector();
        //add range to lgiht data
        if (m_data.hasData(LIGHT_SPOT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY,
            };
            m_data.addData(LIGHT_SPOT3_DATA_RANGE_NAME, info, range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(LIGHT_SPOT3_DATA_RANGE_NAME, range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[LIGHT_SPOT3_DATA_RANGE_NAME] = VG_TRUE;
         _apply();
    }

    void LightSpot3::_beginRender()
    {
        Light3::_beginRender();

        //sync transform between camera and projector.
        m_pProjector->setLocalToWorldMatrix(m_pTransform->getMatrixLocalToWorld());

        //sync projection data.
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
        _apply();
    }

    void LightSpot3::_resetProjector()
    {
        m_pProjector->updateProj(m_fov, 1.0f, std::min(1.0f, m_range), m_range);
    }
} //vg