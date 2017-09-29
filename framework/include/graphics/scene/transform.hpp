#ifndef KGS_TRANSFORM_H
#define KGS_TRANSFORM_H

#include <memory>
#include <vector>
#include <algorithm>
#include "graphics/global.hpp"
#include "graphics/scene/option.hpp"
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
		uint32_t getChildCount()
		{
			return m_pChildren.size();
		}

		void detachChildren()
		{
			m_pChildren.resize(0u);
		}

		std::shared_ptr<Transform> getChildWithIndex(uint32_t index)
		{
			return m_pChildren[index];
		}

		Bool32 isChild(std::shared_ptr<Transform> transform)
		{
			std::find(m_pChildren.cbegin(), m_pChildren.cend(), transform) != m_pChildren.cend();
		}

		std::shared_ptr<Type> getPParent()
		{
			return m_pParant;
		}

		std::shared_ptr<Type> getRoot()
		{
			if (m_pParant == nullptr)
			{
				return this;
			}
			else
			{
				auto root = m_pParant;
				while (root.m_pParent != nullptr)
				{
					root = root.m_pParent
				}
				return root;
			}
		}

		//------------state-----------------------
		Bool32 getIsChanged()
		{
			return m_isChanged;
		}

		PointType getLocalPosition()
		{
			return m_appliedLocalPosition;
		}

		void setLocalPosition(PointType position)
		{
			m_localPosition = position;
			m_isChanged = KGS_TRUE;
		}

		PointType getPosition()
		{
			return _getMatrixLocalToWorld(KGS_FALSE) * m_appliedLocalPosition;
		}

		RotationType getLocalRotation()
		{
			return m_appliedLocalRotation;
		}

		void setLocalRotation(RotationType rotation)
		{
			m_localRotation = rotation;
			m_isChanged = KGS_TRUE;
		}

		RotationType getRotation()
		{
			RotationType rotation = m_appliedLocalRotation;
			auto curr = m_pParant;
			while (curr != nullptr)
			{
				rotation = curr->m_appliedRotation * rotation;
				curr = curr->m_pParent;
			}
			return rotation;
		}

		VectorType getLocalScale()
		{
			return m_appliedLocalScale;
		}

		void setLocalScale(VectorType scale)
		{
			m_localScale = scale;
			m_isChanged = KGS_TRUE;
		}

		VectorType getScale()
		{
			VectorType scale = m_appliedLocalScale;
			auto curr = m_pParant;
			while (curr != nullptr)
			{
				scale = curr->m_appliedScale * scale;
				curr = curr->m_pParent;
			}
			return scale;
		}

		void apply()
		{
			m_appliedLocalScale = m_localScale;
			m_appliedLocalRotation = m_localRotation;
			m_appliedLocalPosition = m_localPosition;
			m_localMatrix = m_appliedLocalPosition * m_appliedLocalRotation * m_appliedLocalScale;
			m_isChanged = KGS_FALSE;
		}

		//-------------------------transform tool-------------------------------------

		VectorType transformVectorToWorld(VectorType vector)
		{
			return VectorType(_getMatrixLocalToWorld() * PointType(vector, 0.0f));
		}

		VectorType transformVectorToLocal(VectorType vector)
		{
			return VectorType(_getMatrixWorldToLocal() * PointType(vector, 0.0f));
		}

		PointType transformPointToWorld(PointType point)
		{
			return _getMatrixLocalToWorld() * point;
		}

		PointType transformPointToLocal(PointType point)
		{
			return _getMatrixWorldToLocal() * point;
		}

		MatrixType getMatrixLocalToWorld()
		{
			return _getMatrixLocalToWorld(KGS_TRUE);
		}

		MatrixType getMatrixWorldToLocal()
		{
			return _getMatrixWorldToLocal(KGS_TRUE);
		}

	protected:
		std::shared_ptr<Type> m_pParant;
		std::vector<std::shared_ptr<Type>> m_pChildren;
		Bool32 m_isChanged;
		//pre apply
		PointType m_localPosition;
		VectorType m_localScale;
		RotationType m_localRotation;
		//post apply
		MatrixType m_localMatrix;
		PointType m_appliedLocalPosition;
		VectorType m_appliedLocalScale;
		RotationType m_appliedLocalRotation;

		inline MatrixType _getMatrixLocalToWorld(Bool32 includeSelf)
		{
			MatrixType matrix(1.0f); //identity matrix;
			if (includeSelf)
			{
				matrix = m_localMatrix;
			}
			auto curr = m_pParant;
			while (curr != nullptr)
			{
				matrix = curr->m_localMatrix * matrix;
				curr = curr->m_pParent;
			}
			return matrix;
		}

		inline MatrixType _getMatrixWorldToLocal(Bool32 includeSelf)
		{
			MatrixType matrix = _getMatrixLocalToWorld(includeSelf);
			glm::inverse(matrix);
			return matrix;
		}

	};
} //namespace kgs
#endif // !KGS_TRANSFORM_H
