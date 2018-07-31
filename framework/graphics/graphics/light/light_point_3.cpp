#include "graphics/light/light_point_3.hpp"

namespace vg
{
    LightPoint3::LightDepthRenderData::LightDepthRenderData()
        : cubeFaceTransform()
        , depthNear(0.1f)
        , depthFar(100.0f)
    {

    }

    const uint32_t LightPoint3::DEFAULT_DEPTH_TEXTURE_WIDTH = 1280u;
    const uint32_t LightPoint3::DEFAULT_DEPTH_TEXTURE_HEIGHT = 1280u;
    const float LightPoint3::DEFAULT_RANGE = 100.0f;

    LightPoint3::LightPoint3(float range
        , uint32_t depthTextureWidth
        , uint32_t depthTextureHeight
        )
        : Light3()
        , m_range(range)
        , m_strength(1.0f)
        , m_pDistTarget()
        , m_pProjector()
        , m_depthRenderData()
    {
        //projector
        m_pProjector = std::shared_ptr<Projector3>{new Projector3(VG_TRUE)};
        _setProjector();
        m_refProjector = m_pProjector.get();
        //depth render target
        m_pDistTarget = std::shared_ptr<LightDistTargetCube>{
            new LightDistTargetCube{
                depthTextureWidth,
                depthTextureHeight
            }
        };
        m_refDistTarget = m_pDistTarget.get();
        //depth render data
        Matrix4x4 identifyMatrix(1.0f);
        auto &faceTransforms = m_depthRenderData.cubeFaceTransform;
        if (m_space.rightHand == VG_TRUE)
        {
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_X)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(-90.0f), Vector3(0.0f, 0.0f, 1.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_X)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(90.0f), Vector3(0.0f, 0.0f, 1.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_Y)] = identifyMatrix;
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_Y)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(180.0f), Vector3(0.0f, 0.0f, 1.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_Z)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(90.0f), Vector3(1.0f, 0.0f, 0.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_Z)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(90.0f), Vector3(1.0f, 0.0f, 0.0f)));
        }
        else
        {
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_X)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(90.0f), Vector3(0.0f, 1.0f, 0.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_X)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(-90.0f), Vector3(0.0f, 1.0f, 0.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_Y)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(-90.0f), Vector3(1.0f, 0.0f, 0.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_Y)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(90.0f), Vector3(1.0f, 0.0f, 0.0f)));
            faceTransforms[static_cast<size_t>(CubemapFace::POSITIVE_Z)] = identifyMatrix;
            faceTransforms[static_cast<size_t>(CubemapFace::NEGATIVE_Z)] = glm::inverse(glm::rotate(identifyMatrix, glm::radians(180.0f), Vector3(0.0f, 1.0f, 0.0f)));
        }
        m_depthRenderData.rightHand = m_space.rightHand;
        m_depthRenderData.depthNear = m_pProjector->getDepthNear();
        m_depthRenderData.depthFar = m_pProjector->getDepthFar();

        //add range to light data.
        _setRange();

        _setStrength();
        //add cube dist texture to light textures.
        _setDistTexture();
        _apply();
    }

    LightDepthRenderInfo LightPoint3::getDepthRenderInfo() const
    {
        LightDepthRenderInfo info = {
            1u,
            reinterpret_cast<const BaseProjector *const *>(&m_refProjector),
            reinterpret_cast<const OnceRenderTarget *const *>(&m_refDistTarget),
            reinterpret_cast<const void *>(&m_depthRenderData),
            static_cast<uint32_t>(sizeof(LightDepthRenderData)),
        };
        return info;
    }

    float LightPoint3::getRange() const
    {
        return m_range;
    }

    void LightPoint3::setRange(float range)
    {
        m_range = range;
        _setProjector();
        _setRange();
        _apply();
    }

    vg::Vector3 LightPoint3::getStrength() const
    {
        return m_strength;
    }

    void LightPoint3::setStrength(vg::Vector3 value)
    {
        m_strength = value;
        _setStrength();
        _apply();
    }

    const LightDistTargetCube *LightPoint3::getLightDistTargetCube() const
    {
        return m_pDistTarget.get();
    }

    void LightPoint3::_beginRender()
    {
        Light3::_beginRender();
        //Projector only use translation of transform.
        Matrix4x4 transform = Matrix4x4(1.0f);
        auto pos = m_pTransform->getLocalPosition();
        transform = glm::translate(transform, pos);
        m_pProjector->setTransformMatrix(transform);
    }

    void LightPoint3::_setProjector()
    {
        float f = m_range;
        float n = std::min(0.1f, f);
        m_pProjector->updateProj(glm::radians(90.0f), 1.0f, n, f);
    }

    void LightPoint3::_setRange()
    {
        if (m_data.hasData(VG_LIGHT_POINT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_POINT3_DATA_RANGE_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_POINT3_DATA_RANGE_NAME, info, m_range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_POINT3_DATA_RANGE_NAME, m_range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_POINT3_DATA_RANGE_NAME] = VG_TRUE;
    }
    
    void LightPoint3::_setStrength()
    {
        if (m_data.hasData(VG_LIGHT_POINT3_DATA_STRENGTH_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_POINT3_DATA_STRENGTH_LAYOUT_PRIORITY,
            };
            m_data.addData(VG_LIGHT_POINT3_DATA_STRENGTH_NAME, info, m_strength);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(VG_LIGHT_POINT3_DATA_STRENGTH_NAME, m_strength);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[VG_LIGHT_POINT3_DATA_STRENGTH_NAME] = VG_TRUE;
    }
    
    void LightPoint3::_setDistTexture()
    {
        LightTextureInfo texInfo = {
            SamplerTextureType::TEX_CUBE,
            VG_LIGHT_TEXTURE_DEPTH_BINDING_PRIORITY,
            m_pDistTarget->getColorTargetTexture(),
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