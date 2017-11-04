#ifndef KGS_TRANSFORM_H
#define KGS_TRANSFORM_H

#include <memory>
#include <vector>
#include <algorithm>
#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include <glm/gtx/matrix_decompose.hpp>

namespace kgs
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
		typedef Transform<SPACE_TYPE> Type;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::VectorType VectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::PointType PointType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixVectorType MatrixVectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixType MatrixType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::RotationType RotationType;

		Transform();

		//------------hierarchy-----------------------
		uint32_t getChildCount();

		void detachChildren();

		std::shared_ptr<Type> getChildWithIndex(uint32_t index);

		Bool32 isChild(std::shared_ptr<Type> pTransform);

		std::shared_ptr<Type> getParent();

		void setParent(std::shared_ptr<Type> pParent);

		std::shared_ptr<Type> getRoot();

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
			m_isChanged = KGS_FALSE;
		}*/

		//-------------------------transform tool-------------------------------------

		VectorType transformVectorToWorld(VectorType vector);

		VectorType transformVectorToLocal(VectorType vector);

		PointType transformPointToWorld(PointType point);

		PointType transformPointToLocal(PointType point);

		MatrixType getMatrixLocalToWorld();

		MatrixType getMatrixWorldToLocal();

	protected:
		std::shared_ptr<Type> m_pParent;
		std::vector<std::shared_ptr<Type>> m_pChildren;
		Bool32 m_isChanged;
		PointType m_localPosition;
		MatrixType m_localPosMatrix;
		VectorType m_localScale;
		MatrixType m_localScaleMatrix;
		RotationType m_localRotation;
		MatrixType m_localRotationMatrix;
		MatrixType m_localMatrix;
		MatrixType m_localMatrixInverse;

		inline void _setParentOnly(std::shared_ptr<Type> pNewParent);
		inline void _setChildOnly(std::shared_ptr<Type> pNewChild);

		inline void _setLocalPositionOnly(PointType position);
		inline void _setLocalScaleOnly(VectorType scale);
		inline void _setLocalRotationOnly(RotationType rotation);
		inline void _setLocalMatrixOnly(MatrixType matrix)
		{
			m_localMatrix = matrix;
			m_localMatrixInverse = glm::inverse(m_localMatrix);
		}

		inline void _setLocalMatrixInverseOnly(MatrixType matrix)
		{
			m_localMatrixInverse = matrix;
			m_localMatrix = glm::inverse(m_localMatrixInverse);
		}

		inline MatrixType _getMatrixLocalToWorld(Bool32 includeSelf);

		inline MatrixType _getMatrixWorldToLocal(Bool32 includeSelf);

		inline void _reCalculateLocalMatrix();

	};
} //namespace kgs

#include "graphics/scene/transform.inl"

#endif // !KGS_TRANSFORM_H
