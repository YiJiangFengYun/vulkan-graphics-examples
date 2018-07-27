#include "graphics/light/light_point_3.hpp"

namespace vg
{
    const uint32_t LightPoint3::DEFAULT_DEPTH_TEXTURE_WIDTH = 1280u;
    const uint32_t LightPoint3::DEFAULT_DEPTH_TEXTURE_HEIGHT = 1280u;
    const float LightPoint3::DEFAULT_RANGE = 100.0f;

    LightPoint3::LightPoint3(float range
        , uint32_t depthTextureWidth
        , uint32_t depthTextureHeight
        )
        : Light3()
        , m_range(range)
        , m_cubeTargets(depthTextureWidth
            , depthTextureHeight
            )
        , m_pProjectors()
        , m_refProjectors()
    {

        //set projectors.
        uint32_t size = static_cast<uint32_t>(m_pProjectors.size());
        for (uint32_t i = 0; i < size; ++i)
        {
            m_pProjectors[i] = std::shared_ptr<Projector3>{new Projector3()};
        }
        //six projector is: x(+-), y(+-) and z(+-).
        for (auto &pProjector : m_pProjectors)
        {
            pProjector->updateProj(glm::radians(90.0f), 1.0f, std::min(1.0f, range), range);
        }
        _syncProjectorTransform();

        for (uint32_t i = 0; i < size; ++i)
        {
            m_refProjectors[i] = m_pProjectors[i].get();
        }

        //add range to light data.
        if (m_data.hasData(LIGHT_POINT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY,
            };
            m_data.addData(LIGHT_POINT3_DATA_RANGE_NAME, info, range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(LIGHT_POINT3_DATA_RANGE_NAME, range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[LIGHT_POINT3_DATA_RANGE_NAME] = VG_TRUE;

        //add cube depth texture to light textures.
        LightTextureInfo texInfo = {
            SamplerTextureType::TEX_CUBE,
            VG_LIGHT_TEXTURE_DEPTH_BINDING_PRIORITY,
            m_cubeTargets.getDepthTargetTexture(),
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

    LightDepthRenderInfo LightPoint3::getDepthRenderInfo() const
    {
        LightDepthRenderInfo info = {
            static_cast<uint32_t>(CubemapFace::RANGE_SIZE),
            reinterpret_cast<const BaseProjector *const *>(m_refProjectors.data()),
            reinterpret_cast<const OnceRenderTarget *const *>(m_cubeTargets.getFaceTargets().data()),
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
        for (auto &pProjector : m_pProjectors)
        {
            pProjector->updateProj(glm::radians(90.0f), 1.0f, std::min(1.0f, range), range);
        }
        //add range to light data.
        if (m_data.hasData(LIGHT_POINT3_DATA_RANGE_NAME) == VG_FALSE)
        {
            LightDataInfo info = {
                VG_LIGHT_DATA_OTHER_MIN_LAYOUT_PRIORITY,
            };
            m_data.addData(LIGHT_POINT3_DATA_RANGE_NAME, info, range);
            m_dataChanged = VG_TRUE;
        } else {
            m_data.setData(LIGHT_POINT3_DATA_RANGE_NAME, range);
        }
        m_dataContentChanged = VG_TRUE;
        m_dataContentChanges[LIGHT_POINT3_DATA_RANGE_NAME] = VG_TRUE;
        _apply();
    }

    void LightPoint3::_beginRender()
    {
        Light3::_beginRender();
        _syncProjectorTransform();
    }

    void LightPoint3::_syncProjectorTransform()
    {
        uint32_t index = 0u;
        for (auto &pProjector : m_pProjectors)
        {
            auto transform = m_pTransform->getMatrixLocalToWorld();
            auto rotationTransform = Matrix4x4(1.0f);
            if (m_space.rightHand == VG_TRUE)
            {
                switch (index)
                {
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_X): //+x
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(-90.0f), Vector3(0.0f, 0.0f, 1.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_X): //-x
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(90.0f), Vector3(0.0f, 0.0f, 1.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_Y): //+y
                    {
                        //Don't need to transform.
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_Y): //-y
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(180.0f), Vector3(0.0f, 0.0f, 1.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_Z): //+z
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(90.0f), Vector3(1.0f, 0.0f, 0.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_Z): //-z
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(-90.0f), Vector3(1.0f, 0.0f, 0.0f));
                        break;
                    }
                }
            }
            else
            {
                switch (index)
                {
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_X): //+x
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(90.0f), Vector3(0.0f, 1.0f, 0.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_X): //-x
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(-90.0f), Vector3(0.0f, 1.0f, 0.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_Y): //+y
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(-90.0f), Vector3(1.0f, 0.0f, 0.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_Y): //-y
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(90.0f), Vector3(1.0f, 0.0f, 0.0f));
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::POSITIVE_Z): //+z
                    {
                        //Don't need to transform.
                        break;
                    }
                    case static_cast<uint32_t>(CubemapFace::NEGATIVE_Z): //-z
                    {
                        rotationTransform = glm::rotate(rotationTransform, glm::radians(180.0f), Vector3(0.0f, 1.0f, 0.0f));
                        break;
                    }
                }
            }
            pProjector->setTransformMatrix(transform * rotationTransform);
            ++index;
        }
    }
} //vg