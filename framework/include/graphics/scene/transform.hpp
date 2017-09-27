#ifndef KGS_TRANSFORM_H
#define KGS_TRANSFORM_H

#include <memory>
#include <vector>
#include "graphics/global.hpp"
#include "graphics/scene/option.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Transform
	{
	public:
		typedef Transform<SPACE_TYPE> Type;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::VecType VecType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixType MatrixType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::RotationType RotationType;

		uint32_t getChildCount()
		{
			return m_pChildren.size();
		}

		Bool32 getIsChanged()
		{
			return m_isChanged;
		}

		VecType getLocalPosition()
		{
			return m_appliedPosition;
		}

		void setLocalPosition(VecType position)
		{
			m_localPosition = position;
			m_isChanged = KGS_TRUE;
		}

		VecType getPosition()
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

		VecType getLocalScale()
		{
			return m_localScale;
		}

		void setLocalScale(VecType scale)
		{
			m_localScale = scale;
			m_isChanged = KGS_TRUE;
		}

		VecType getScale()
		{
			VecType scale = m_appliedScale;
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

		MatrixType getMatrixLocalToWorld()
		{
			return _getMatrixLocalToWorld(KGS_TRUE);
		}

		MatrixType getMatrixWorldToLocal()
		{
			return _getMatrixWorldToLocal(KGS_TRUE);
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


	private:
		std::shared_ptr<Type> m_pParant;
		std::vector<std::shared_ptr<Type>> m_pChildren;
		Bool32 m_isChanged;
		//pre apply
		VecType m_localPosition;
		VecType m_localScale;
		RotationType m_localRotation;
		//post apply
		MatrixType m_localMatrix;
		VecType m_appliedPosition;
		VecType m_appliedScale;
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
