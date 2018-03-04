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
		BaseTransform();
	protected:

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
		uint32_t getChildCount() const;

		Type *getChildWithIndex(uint32_t index) const;

		typename std::vector<Type *>::const_iterator getChildPos(Type *child);

		const std::vector<Type *> &getChildren();

		void detachChildren();

		Bool32 isChild(Type *pTransform);

		void addChild(Type *pNewChild);
		void addChild(Type *pNewChild, typename std::vector<Type *>::const_iterator pos);
		void removeChild(Type *pChild);

		Type *getParent();

		void setParent(Type *pParent);

		Type *getRoot();

		//------------state-----------------------
		Bool32 getIsChanged();

		PointType getLocalPosition();

		void setLocalPosition(PointType position);

		PointType getPosition();

		RotationType getLocalRotation();

		void setLocalRotation(RotationType rotation);

		RotationType getRotation();

		VectorType getLocalScale();

		void setLocalScale(VectorType scale);

		VectorType getScale();

		void setLocalMatrix(MatrixType matrix);
		void setLocalMatrixInverse(MatrixType matrix);

		/*void apply()
		{
			m_isChanged = VG_FALSE;
		}*/

		//-------------------------transform tool-------------------------------------

		VectorType transformVectorToWorld(VectorType vector);

		VectorType transformVectorToLocal(VectorType vector);

		PointType transformPointToWorld(PointType point);

		PointType transformPointToLocal(PointType point);

		MatrixType getMatrixLocalToWorld();

		MatrixType getMatrixWorldToLocal();

	protected:
		Type *m_pParent;
		std::unordered_map<InstanceID, Type *> m_mapPChildren;
		std::vector<Type *> m_arrPChildren;
		Bool32 m_isChanged;
		PointType m_localPosition;
		MatrixType m_localPosMatrix;
		VectorType m_localScale;
		MatrixType m_localScaleMatrix;
		RotationType m_localRotation;
		MatrixType m_localRotationMatrix;
		MatrixType m_localMatrix;
		MatrixType m_localMatrixInverse;

		void _setParentOnly(Type *pNewParent);
		void _addChildOnly(Type *pNewChild);
		void _addChildOnly(Type *pNewChild, 
		    typename std::vector<Type *>::const_iterator pos);
		void _removeChildOnly(Type *pChild);
		Bool32 _isChild(Type *pTransform);

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

		MatrixType _getMatrixLocalToWorld(Bool32 includeSelf);

		MatrixType _getMatrixWorldToLocal(Bool32 includeSelf);

		void _reCalculateLocalMatrix();

	};
} //namespace kgs

#include "graphics/scene/transform.inl"

#endif // !VG_TRANSFORM_H
