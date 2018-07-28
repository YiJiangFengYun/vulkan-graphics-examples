#include "graphics/scene/transform.hpp"

namespace vg
{
//BaesTransform
    BaseTransform::BaseTransform()
        : Base(BaseType::TRANSFORM)
        , m_space()
    {

    }

    const Space &BaseTransform::getSpace() const
    {
        return m_space;
    }
       
    void BaseTransform::setSpace(const Space &space)
    {
        m_space = space;
    }

//Transform
    template <SpaceType SPACE_TYPE>
    Transform<SPACE_TYPE>::Transform()
        : BaseTransform()
        , m_pParent(nullptr)
        , m_arrPChildren()
        , m_mapPChildren()
        , m_isChanged(VG_FALSE)
        , m_localPosition(0.0f)
        , m_localPosMatrix(1.0f)
        , m_localScale(1.0f)
        , m_localScaleMatrix(1.0f)
        , m_localRotation()
        , m_localRotationMatrix(1.0f)
        , m_localMatrix(1.0f)
        , m_localMatrixInverse(1.0f)
    {

    }

    template <SpaceType SPACE_TYPE>
    uint32_t Transform<SPACE_TYPE>::getChildCount() const
    {
        return static_cast<uint32_t>(m_arrPChildren.size());
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::detachChildren()
    {
        for (auto item : m_arrPChildren)
        {
            item->_setParentOnly(nullptr);
        }
        m_arrPChildren.resize(0u);
        m_mapPChildren.clear();
    }

    template <SpaceType SPACE_TYPE>
    const typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getChildWithIndex(uint32_t index) const
    {
        return m_arrPChildren[index];
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getChildWithIndex(uint32_t index)
    {
        return m_arrPChildren[index];
    }

    template <SpaceType SPACE_TYPE>
    typename std::vector<typename Transform<SPACE_TYPE>::Type *>::const_iterator Transform<SPACE_TYPE>::getChildPos(const Type *child) const
    {
#ifdef DEBUG
        if (isChild(child) == VG_FALSE) throw std::invalid_argument("The target is not a child of the tranform.");
#endif // DEBUG
        return std::find(m_arrPChildren.cbegin(), m_arrPChildren.cend(), child);
    }

    template <SpaceType SPACE_TYPE>
    const typename Transform<SPACE_TYPE>::Type * const * Transform<SPACE_TYPE>::getChildren() const
    {
        return m_arrPChildren.data();
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::Type * const * Transform<SPACE_TYPE>::getChildren()
    {
        return m_arrPChildren.data();
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Transform<SPACE_TYPE>::isChild(const Type *pTransform) const
    {
        return _isChild(pTransform);
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::addChild(Type *pNewChild)
    {
        if (_isChild(pNewChild)) return;

        _addChildOnly(pNewChild);
        pNewChild->setParent(this);
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::addChild(Type *pNewChild, typename std::vector<Type *>::const_iterator pos)
    {
        _addChildOnly(pNewChild, pos);
        pNewChild->setParent(this);
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::removeChild(Type *pChild)
    {
        if (_isChild(pChild) == VG_FALSE) return;

        _removeChildOnly(pChild);
        pChild->_setParentOnly(nullptr);
    }

    template <SpaceType SPACE_TYPE>
    const typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getParent() const
    {
        return m_pParent;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getParent()
    {
        return m_pParent;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::setParent(Type *pParent)
    {
        if (m_pParent == pParent) return;
        if (pParent == nullptr)
        {
            m_pParent->_removeChildOnly(this);
            _setParentOnly(nullptr);
        }
        else
        {
            if (m_pParent != nullptr)
            {
                setParent(nullptr);
            }
            _setParentOnly(pParent);
            pParent->_addChildOnly(this);
        }

    }

    template <SpaceType SPACE_TYPE>
    const typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getRoot() const
    {
        if (m_pParent == nullptr)
        {
            return nullptr;
        }
        else
        {
            auto root = m_pParent;
            while (root->m_pParent != nullptr)
            {
                root = root->m_pParent;
            }
            return root;
        }
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getRoot()
    {
        if (m_pParent == nullptr)
        {
            return nullptr;
        }
        else
        {
            auto root = m_pParent;
            while (root->m_pParent != nullptr)
            {
                root = root->m_pParent;
            }
            return root;
        }
    }

    template <SpaceType SPACE_TYPE>
    Bool32 Transform<SPACE_TYPE>::getIsChanged() const
    {
        return m_isChanged;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::getLocalPosition() const
    {
        return m_localPosition;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::setLocalPosition(PointType position)
    {
        _setLocalPositionOnly(position);
        _reCalculateLocalMatrix();
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::getPosition() const
    {
        return _getMatrixLocalToWorld(VG_FALSE) * MatrixVectorType(m_localPosition, 1.0);
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getLocalRotation() const
    {
        return m_localRotation;
    }

    template <SpaceType SPACE_TYPE>
    typename void Transform<SPACE_TYPE>::setLocalRotation(RotationType rotation)
    {
        _setLocalRotationOnly(rotation);
        _reCalculateLocalMatrix();
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getRotation() const
    {
        RotationType rotation = m_localRotation;
        Type *curr = m_pParent;
        while (curr != nullptr)
        {
            rotation = curr->m_localRotation * rotation;
            curr = curr->m_pParent;
        }
        return rotation;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getLocalScale() const
    {
        return m_localScale;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::setLocalScale(VectorType scale)
    {
        _setLocalScaleOnly(scale);
        _reCalculateLocalMatrix();
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getScale() const
    {
        VectorType scale = m_localScale;
        auto curr = m_pParent;
        while (curr != nullptr)
        {
            scale = curr->m_localScale * scale;
            curr = curr->m_pParent;
        }
        return scale;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getLocalMatrix() const
    {
        return m_localMatrix;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getLocalMatrixInverse() const
    {
        return m_localMatrixInverse;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::transformVectorToWorld(VectorType vector) const
    {
        return VectorType(_getMatrixLocalToWorld(VG_TRUE) * MatrixVectorType(vector, 0.0f));
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::transformVectorToLocal(VectorType vector) const
    {
        return VectorType(_getMatrixWorldToLocal(VG_TRUE) * MatrixVectorType(vector, 0.0f));
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::transformPointToWorld(PointType point) const
    {
        return _getMatrixLocalToWorld(VG_TRUE) * MatrixVectorType(point, 1.0f);
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::transformPointToLocal(PointType point) const
    {
        return _getMatrixWorldToLocal(VG_TRUE) * MatrixVectorType(point, 1.0f);
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getMatrixLocalToWorld() const
    {
        return _getMatrixLocalToWorld(VG_TRUE);
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getMatrixWorldToLocal() const
    {
        return _getMatrixWorldToLocal(VG_TRUE);
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_setParentOnly(Type *pNewParent)
    {
        m_pParent = pNewParent;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_addChildOnly(Type *pNewChild)
    {
        m_arrPChildren.push_back(pNewChild);
        m_mapPChildren[pNewChild->getID()] = pNewChild;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_addChildOnly(Type *pNewChild, typename std::vector<Type *>::const_iterator pos)
    {
        if (_isChild(pNewChild))
        {
            auto removed = std::remove(m_arrPChildren.begin(), m_arrPChildren.end(), pNewChild);
            m_arrPChildren.erase(removed, m_arrPChildren.end());
            m_arrPChildren.insert(pos, pNewChild);
        }
        else
        {
            m_arrPChildren.insert(pos, pNewChild);
            m_mapPChildren[pNewChild->getID()] = pNewChild;
        }
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_removeChildOnly(Type *pChild)
    {
        auto removed = std::remove(m_arrPChildren.begin(), m_arrPChildren.end(), pChild);
        m_arrPChildren.erase(removed, m_arrPChildren.end());
        m_mapPChildren.erase(pChild->getID());
    }

    template <SpaceType SPACE_TYPE>
    inline Bool32 Transform<SPACE_TYPE>::_isChild(const Type *pTransform) const
    {
        return m_mapPChildren.find(pTransform->getID()) != m_mapPChildren.cend();
    }

    template <SpaceType SPACE_TYPE>
    typename void Transform<SPACE_TYPE>::_setLocalPositionOnly(PointType position)
    {
        m_localPosition = position;
        typename MatrixVectorType::length_type i;
        typename MatrixVectorType::length_type length = MatrixVectorType::length();
        MatrixType normalMatrix(1.0f);
        m_localPosMatrix = normalMatrix;
        for (i = 0; i < length - 1; ++i)
        {
            m_localPosMatrix[length - 1] += normalMatrix[i] * position[i];
        }
        //m_localPosMatrix[length - 1] += normalMatrix[i];
        m_isChanged = VG_TRUE;
    }

    template <SpaceType SPACE_TYPE>
    typename void Transform<SPACE_TYPE>::_setLocalScaleOnly(VectorType scale)
    {
        m_localScale = scale;
        typename MatrixVectorType::length_type i;
        typename MatrixVectorType::length_type length = MatrixVectorType::length();
        MatrixType normalMatrix(1.0f);
        MatrixType result;
        for (i = 0; i < length - 1; ++i)
        {
            result[i] = normalMatrix[i] * scale[i];
        }
        result[i] = normalMatrix[i];
        m_localScaleMatrix = result;
        m_isChanged = VG_TRUE;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::_getMatrixLocalToWorld(Bool32 includeSelf) const
    {
        MatrixType matrix(1.0f); //identity matrix;
        if (includeSelf)
        {
            matrix = m_localMatrix;
        }
        auto curr = m_pParent;
        while (curr != nullptr)
        {
            matrix = curr->m_localMatrix * matrix;
            curr = curr->m_pParent;
        }
        return matrix;
    }

    template <SpaceType SPACE_TYPE>
    typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::_getMatrixWorldToLocal(Bool32 includeSelf) const
    {
        MatrixType matrix = _getMatrixLocalToWorld(includeSelf);
        matrix = glm::inverse(matrix);
        return matrix;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_reCalculateLocalMatrix()
    {
        if (m_isChanged == VG_FALSE) return;
        _setLocalMatrixOnly(m_localPosMatrix * m_localRotationMatrix * m_localScaleMatrix);
        m_isChanged = VG_FALSE;
    }

    template <SpaceType SPACE_TYPE>
    void Transform<SPACE_TYPE>::_setLocalRotationOnly(RotationType rotation)
    {
        throw std::runtime_error("Method cann't be called.");
    }

    template <SpaceType SPACE_TYPE>
    void  Transform<SPACE_TYPE>::setLocalMatrix(MatrixType matrix)
    {
        throw std::runtime_error("Method cann't be called.");
    }

    template <SpaceType SPACE_TYPE>
    void  Transform<SPACE_TYPE>::setLocalMatrixInverse(MatrixType matrix)
    {
        throw std::runtime_error("Method cann't be called.");
    }

    //template specilization

    template<>
    void Transform<SpaceType::SPACE_2>::_setLocalRotationOnly(float rotation)
    {
        m_localRotation = rotation;
        Matrix3x3 result(1.0f);
        result[0][0] = glm::cos(rotation);
        result[0][1] = glm::sin(rotation);
        result[1][0] = - glm::sin(rotation);
        result[1][1] = glm::cos(rotation);
        m_localRotationMatrix = result;
        m_isChanged = VG_TRUE;
    }

    template<>
    void Transform<SpaceType::SPACE_3>::_setLocalRotationOnly(Quaternion rotation)
    {
        m_localRotation = rotation;
        m_localRotationMatrix = glm::toMat4(rotation);
        m_isChanged = VG_TRUE;
    }

    template <>
    void  Transform<SpaceType::SPACE_3>::setLocalMatrix(MatrixType matrix)
    {
        _setLocalMatrixOnly(matrix);
        VectorType tempScale;
        RotationType tempRotation;
        PointType tempTranslate;
        PointType tempSkew;
        MatrixVectorType tempPerspective;
        glm::decompose(m_localMatrix, tempScale, tempRotation, tempTranslate, tempSkew, tempPerspective);
        _setLocalScaleOnly(tempScale);
        _setLocalRotationOnly(tempRotation);
        _setLocalPositionOnly(tempTranslate);
        m_isChanged = VG_FALSE;
    }


    template <>
    void  Transform<SpaceType::SPACE_3>::setLocalMatrixInverse(MatrixType matrix)
    {
        _setLocalMatrixInverseOnly(matrix);
        VectorType tempScale;
        RotationType tempRotation;
        PointType tempTranslate;
        PointType tempSkew;
        MatrixVectorType tempPerspective;
        glm::decompose(m_localMatrix, tempScale, tempRotation, tempTranslate, tempSkew, tempPerspective);
        _setLocalScaleOnly(tempScale);
        _setLocalRotationOnly(tempRotation);
        _setLocalPositionOnly(tempTranslate);
        m_isChanged = VG_FALSE;
    }
    
    //template instantiation
    template class Transform<SpaceType::SPACE_2>;
    template class Transform<SpaceType::SPACE_3>;
    
} //namespace kgs