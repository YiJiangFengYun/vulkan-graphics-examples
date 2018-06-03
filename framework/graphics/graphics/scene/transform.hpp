#ifndef VG_TRANSFORM_H
#define VG_TRANSFORM_H

#include <memory>
#include <algorithm>
#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace vg
{
    class BaseTransform : public Base
    {
    public:
        using ConstIterator = typename std::vector<BaseTransform *>::const_iterator;
        BaseTransform();

        uint32_t getChildCount() const;
        void detachChildren();
        const BaseTransform * const * getChildren() const;
        BaseTransform * const * getChildren();

        void setParent(BaseTransform *pParent);
    protected:
        BaseTransform *m_pParent;
        std::unordered_map<InstanceID, BaseTransform *> m_mapPChildren;
        std::vector<BaseTransform *> m_arrPChildren;

        const BaseTransform *_getChildWithIndex(uint32_t index) const;
        BaseTransform *_getChildWithIndex(uint32_t index);
        const BaseTransform * const * _getChildren() const;
        BaseTransform * const * _getChildren();

        ConstIterator _getChildPos(const BaseTransform *child) const;

        void _addChild(BaseTransform *pNewChild);
        void _addChild(BaseTransform *pNewChild, ConstIterator pos);
        void _removeChild(BaseTransform *pChild);

        const BaseTransform *_getParent() const;
        BaseTransform *_getParent();

        const BaseTransform *_getRoot() const;
        BaseTransform *_getRoot();

        void _setParentOnly(BaseTransform *pNewParent);
        void _addChildOnly(BaseTransform *pNewChild);
        void _addChildOnly(BaseTransform *pNewChild, ConstIterator pos);
        void _removeChildOnly(BaseTransform *pChild);
        Bool32 _isChild(const BaseTransform *pTransform) const;
    };

    template <SpaceType SPACE_TYPE>
    class Transform : public BaseTransform
    {
    public:
        using Type = Transform<SPACE_TYPE>;
        using VectorType = typename SpaceTypeInfo<SPACE_TYPE>::VectorType;
        using PointType = typename SpaceTypeInfo<SPACE_TYPE>::PointType;
        using MatrixVectorType = typename SpaceTypeInfo<SPACE_TYPE>::MatrixVectorType;
        using MatrixType = typename SpaceTypeInfo<SPACE_TYPE>::MatrixType;
        using RotationType = typename SpaceTypeInfo<SPACE_TYPE>::RotationType;

        Transform();

        //------------hierarchy-----------------------
        const Type *getChildWithIndex(uint32_t index) const;
        Type *getChildWithIndex(uint32_t index);
        ConstIterator getChildPos(const Type *child) const;
		Bool32 isChild(const Type *pTransform) const;
        void addChild(Type *pNewChild);
        void addChild(Type *pNewChild, ConstIterator pos);
        void removeChild(Type *pChild);

        const Type *getParent() const;
        Type *getParent();

        void setParent(Type *pParent);

        const Type *getRoot() const;
        Type *getRoot();

        //------------state-----------------------
        Bool32 getIsChanged() const;

        PointType getLocalPosition() const;

        void setLocalPosition(PointType position);

        PointType getPosition() const;

        RotationType getLocalRotation() const;

        void setLocalRotation(RotationType rotation);

        RotationType getRotation() const;

        VectorType getLocalScale() const;

        void setLocalScale(VectorType scale);

        VectorType getScale() const;

        void setLocalMatrix(MatrixType matrix);
        MatrixType getLocalMatrix() const;
        void setLocalMatrixInverse(MatrixType matrix);
        MatrixType getLocalMatrixInverse() const;

        /*void apply()
        {
            m_isChanged = VG_FALSE;
        }*/

        //-------------------------transform tool-------------------------------------

        VectorType transformVectorToWorld(VectorType vector) const;

        VectorType transformVectorToLocal(VectorType vector) const;

        PointType transformPointToWorld(PointType point) const;

        PointType transformPointToLocal(PointType point) const;

        MatrixType getMatrixLocalToWorld() const;

        MatrixType getMatrixWorldToLocal() const;

    protected:
        Bool32 m_isChanged;
        PointType m_localPosition;
        MatrixType m_localPosMatrix;
        VectorType m_localScale;
        MatrixType m_localScaleMatrix;
        RotationType m_localRotation;
        MatrixType m_localRotationMatrix;
        MatrixType m_localMatrix;
        MatrixType m_localMatrixInverse;

        void _setLocalPositionOnly(PointType position);
        void _setLocalScaleOnly(VectorType scale);
        void _setLocalRotationOnly(RotationType rotation);
        void _setLocalMatrixOnly(MatrixType matrix)
        {
            m_localMatrix = matrix;
            m_localMatrixInverse = glm::inverse(m_localMatrix);
        }

        void _setLocalMatrixInverseOnly(MatrixType matrix)
        {
            m_localMatrixInverse = matrix;
            m_localMatrix = glm::inverse(m_localMatrixInverse);
        }

        MatrixType _getMatrixLocalToWorld(Bool32 includeSelf) const;

        MatrixType _getMatrixWorldToLocal(Bool32 includeSelf) const;

        void _reCalculateLocalMatrix();

    };
} //namespace kgs

#endif // !VG_TRANSFORM_H
