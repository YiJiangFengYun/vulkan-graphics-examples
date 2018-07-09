#include "graphics/light/light_spot_3.hpp"

namespace vg
{
    const uint32_t LightSpot3::DEFAULT_DEPTH_TEXTURE_WIDTH = 1280u;
    const uint32_t LightSpot3::DEFAULT_DEPTH_TEXTURE_HEIGHT = 1280u;
    const float LightSpot3::DEFAULT_RADIUS = 100.0f;
    const float LightSpot3::DEFAULT_FOV = glm::radians(45.0f);
    LightSpot3::LightSpot3(float fov
        , float radius
        , uint32_t depthTextureWidth
        , uint32_t depthTextureHeight
        )
        : Light3()
        , m_fov(fov)
        , m_radius(radius)
        , m_pDepthTarget()
        , m_refDepthTarget()
        , m_pProjector()
        , m_refProjector()
    {
        m_pDepthTarget = std::shared_ptr<LightDepthTarget2D>{ new LightDepthTarget2D(depthTextureWidth, depthTextureHeight)};
        m_refDepthTarget = m_pDepthTarget.get();
        m_pProjector = std::shared_ptr<Projector3>{new Projector3()};
        m_pProjector->updateProj(fov, 1.0f, std::min(0.001f, radius), radius);
        Matrix4x4 transform(1.0f);
        m_pProjector->setTransformMatrix(transform);
        m_refProjector = m_pProjector.get();

        //add radius to lgiht data
        if (m_data.hasData(LIGHT_SPOT3_DATA_RADIUS_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY,
            };
            m_data.addData(LIGHT_SPOT3_DATA_RADIUS_NAME, info, radius);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(LIGHT_SPOT3_DATA_RADIUS_NAME, radius);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[LIGHT_SPOT3_DATA_RADIUS_NAME] = VG_TRUE;

        //add the depth texture to light textures.
        LightTextureInfo texInfo = {
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
            reinterpret_cast<const PreDepthTarget *const *>(&m_refDepthTarget),
        };
        return info;
    }

    void LightSpot3::_beginRender()
    {
        Light3::_beginRender();
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
} //vg