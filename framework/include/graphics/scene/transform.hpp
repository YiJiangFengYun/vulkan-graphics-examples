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
	template <SpaceType SPACE_TYPE>
	class Transform
	{
	public:
		typedef Transform<SPACE_TYPE> Type;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::VectorType VectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::PointType PointType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixVectorType MatrixVectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixType MatrixType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::RotationType RotationType;

		//------------hierarchy-----------------------
		uint32_t getChildCount();

		void detachChildren();

		std::shared_ptr<Type> getChildWithIndex(uint32_t index);

		Bool32 isChild(std::shared_ptr<Type> transform);

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
		VectorType m_localScale;
		RotationType m_localRotation;
		MatrixType m_localMatrix;

		inline MatrixType _getMatrixLocalToWorld(Bool32 includeSelf);

		inline MatrixType _getMatrixWorldToLocal(Bool32 includeSelf);

		inline void _reCalculateLocalMatrix();

	};
} //namespace kgs

#include "graphics/scene/transform.inl"

#endif // !KGS_TRANSFORM_H
