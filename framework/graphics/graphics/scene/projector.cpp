#include "graphics/scene/projector.hpp"

namespace vg
{
    BaseProjector::BaseProjector()
        : Base(BaseType::SCENE_OBJECT)
        , m_isOrthographic(VG_FALSE)
    {

    }
        
    BaseProjector::~BaseProjector()
    {

    }
    
    Bool32 BaseProjector::getIsOrthographic() const
    {
        return m_isOrthographic;
    }

    const Space &BaseProjector::getSpace() const
    {
        return m_space;
    }
        
    void BaseProjector::setSpace(const Space &space)
    {
        m_space = space;
    }

    template <SpaceType SPACE_TYPE>
    Projector<SPACE_TYPE>::Projector()
        : BaseProjector()
    {
    }

    template <SpaceType SPACE_TYPE>
    typename Projector<SPACE_TYPE>::MatrixType Projector<SPACE_TYPE>::getProjMatrix() const
    {
        return m_projMatrix;
    }

    template <SpaceType SPACE_TYPE>
    typename Projector<SPACE_TYPE>::MatrixType Projector<SPACE_TYPE>::getTransformMatrix() const
    {
        return m_localToWorldMatrix;
    }

    template <SpaceType SPACE_TYPE>
    typename Projector<SPACE_TYPE>::MatrixType Projector<SPACE_TYPE>::getLocalToWorldMatrix() const
    {
        return m_localToWorldMatrix;
    }
    
    template <SpaceType SPACE_TYPE>
    typename Projector<SPACE_TYPE>::MatrixType Projector<SPACE_TYPE>::getWorldToLocalMatrix() const
    {
        return m_worldToLocalMatrix;
    }
    
    template <SpaceType SPACE_TYPE>
    void Projector<SPACE_TYPE>::setTransformMatrix(const MatrixType &matrix)
    {
        m_localToWorldMatrix = matrix;
        m_worldToLocalMatrix = glm::inverse(matrix);
    }

    template <SpaceType SPACE_TYPE>
    void Projector<SPACE_TYPE>::setLocalToWorldMatrix(const MatrixType &matrix)
    {
        m_localToWorldMatrix = matrix;
        m_worldToLocalMatrix = glm::inverse(matrix);
    }

    //template instantiation
    template class Projector<SpaceType::SPACE_2>;
    template class Projector<SpaceType::SPACE_3>;

    template <SpaceType SPACE_TYPE>
    ProjectorOP<SPACE_TYPE>::ProjectorOP()
        : Projector<SPACE_TYPE>()
        , m_viewBounds(BoundsType::ValueType(-1.0f), BoundsType::ValueType(1.0f))
    {
        m_isOrthographic = VG_TRUE;
    }

    template <SpaceType SPACE_TYPE>
    void ProjectorOP<SPACE_TYPE>::updateProj(BoundsType viewBounds)
    {
        m_viewBounds = viewBounds;
        _applyProj();
    }

    template <SpaceType SPACE_TYPE>
    typename ProjectorOP<SPACE_TYPE>::BoundsType ProjectorOP<SPACE_TYPE>::getViewBounds() const
    {
        return m_viewBounds;
    }

    //template instantiation
    template class ProjectorOP<SpaceType::SPACE_2>;
    template class ProjectorOP<SpaceType::SPACE_3>;

    ProjectorOP2::ProjectorOP2()
        : ProjectorOP<SpaceType::SPACE_2>()
    {
        _applyProj();
    }
     
    void ProjectorOP2::_applyProj()
    {
        auto min = m_viewBounds.getMin();
        auto max = m_viewBounds.getMax();
        m_projMatrix[0] = { 2.0f / (max.x - min.x), 0.0f, 0.0f };
        m_projMatrix[1] = {0.0f, 2.0f / (max.y - min.y), 0.0f };
        m_projMatrix[2] = { -(max.x + min.x) / (max.x - min.x), -(max.y + min.y) / (max.y - min.y), 1.0f };
        // m_projMatrix[1][1] *= -1;
    }

    ProjectorOP3::ProjectorOP3()
        : ProjectorOP<SpaceType::SPACE_3>()
    {
        m_viewBounds = { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} };
        _applyProj();
    }

    void ProjectorOP3::_applyProj()
    {
        auto min = m_viewBounds.getMin();
        auto max = m_viewBounds.getMax();
        m_projMatrix[0] = { 2.0f / (max.x - min.x), 0.0f, 0.0f, 0.0f };
        m_projMatrix[1] = { 0.0f, 2.0f / (max.y - min.y), 0.0f, 0.0f };
        m_projMatrix[2] = { 0.0f, 0.0f, 1.0f / (max.z - min.z), 0.0f };
        m_projMatrix[3] = { -(max.x + min.x) / (max.x - min.x)
            , -(max.y + min.y) / (max.y - min.y)
            , -min.z / (max.z - min.z)
            , 1.0f };
    }

    const float Projector3::DEFAULT_FOV = glm::radians(45.0f);
    const float Projector3::DEFAULT_ASPECT = 1.0f;
    const float Projector3::DEFAULT_DEPTH_NEAR = 0.1f;
    const float Projector3::DEFAULT_DEPTH_FAR = 10.0f;
    Projector3::Projector3()
        : Projector<SpaceType::SPACE_3>()
        , m_fov(DEFAULT_FOV)
        , m_aspect(DEFAULT_ASPECT)
        , m_depthNear(DEFAULT_DEPTH_NEAR)
        , m_depthFar(DEFAULT_DEPTH_FAR)
    {
        m_projMatrix = glm::perspective(m_fov, m_aspect, m_depthNear, m_depthFar);
    }

    void Projector3::updateProj(float fov, float aspect, float depthNear, float depthFar)
    {
        m_fov = fov;
        m_aspect = aspect;
        m_depthNear = depthNear;
        m_depthFar = depthFar;
        m_projMatrix = glm::perspective(m_fov, m_aspect, m_depthNear, m_depthFar);
    }

    float Projector3::getFov() const
    {
        return m_fov;
    }

    void Projector3::setFov(float fov)
    {
        m_fov = fov;
    }

    float Projector3::getAspect() const
    {
        return m_aspect;
    }

    void Projector3::setAspect(float aspect)
    {
        m_aspect = aspect;
    }

    float Projector3::getDepthNear() const
    {
        return m_depthNear;
    }

    void Projector3::setDepthNear(float depthNear)
    {
        m_depthNear = depthNear;
    }

    float Projector3::getDepthFar() const
    {
        return m_depthFar;
    }

    void Projector3::setDepthFar(float depthFar)
    {
        m_depthFar = depthFar;
    }

}