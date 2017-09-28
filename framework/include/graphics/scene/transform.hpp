#ifndef KGS_TRANSFORM_H
#define KGS_TRANSFORM_H

#include <memory>
#include <vector>
#include <algorithm>
#include "graphics/global.hpp"
#include "graphics/scene/option.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Transform
	{
	public:
		typedef Transform<SPACE_TYPE> Type;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::VectorType VectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::PointType PointType;
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
			return m_appliedPosition;
		}

		void setLocalPosition(PointType position)
		{
			m_localPosition = position;
			m_isChanged = KGS_TRUE;
		}

		PointType getPosition()
		{
			return _getMatrixLocalToWorld(KGS_FALSE) * m_appliedPosition;
		}

		RotationType getLocalRotation()
		{
			return m_localRotation;
		}

		void setLocalRotation(RotationType rotation)
		{
			m_localRotation = rotation;
			m_isChanged = KGS_TRUE;
		}

		RotationType getRotation()
		{
			RotationType rotation = m_appliedRotation;
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
			return m_localScale;
		}

		void setLocalScale(VectorType scale)
		{
			m_localScale = scale;
			m_isChanged = KGS_TRUE;
		}

		VectorType getScale()
		{
			VectorType scale = m_appliedScale;
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

		}

		/*void lookAt(PointType target, VectorType worldUp)
		{
			glm::lookAt(m_appliedPosition, target, )
		}*/

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

	private:
		std::shared_ptr<Type> m_pParant;
		std::vector<std::shared_ptr<Type>> m_pChildren;
		Bool32 m_isChanged;
		//pre apply
		PointType m_localPosition;
		VectorType m_localScale;
		RotationType m_localRotation;
		//post apply
		MatrixType m_localMatrix;
		PointType m_appliedPosition;
		VectorType m_appliedScale;
		RotationType m_appliedRotation;

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
