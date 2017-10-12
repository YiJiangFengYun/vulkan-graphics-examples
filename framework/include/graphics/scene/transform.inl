namespace kgs
{
	template <SpaceType SPACE_TYPE>
	uint32_t Transform<SPACE_TYPE>::getChildCount()
	{
		return m_pChildren.size();
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::detachChildren()
	{
		m_pChildren.resize(0u);
	}

	template <SpaceType SPACE_TYPE>
	std::shared_ptr<typename Transform<SPACE_TYPE>::Type> Transform<SPACE_TYPE>::getChildWithIndex(uint32_t index)
	{
		return m_pChildren[index];
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Transform<SPACE_TYPE>::isChild(std::shared_ptr<Type> transform)
	{
		std::find(m_pChildren.cbegin(), m_pChildren.cend(), transform) != m_pChildren.cend();
	}

	template <SpaceType SPACE_TYPE>
	std::shared_ptr<typename Transform<SPACE_TYPE>::Type> Transform<SPACE_TYPE>::getParent()
	{
		return m_pParent;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setParent(std::shared_ptr<Type> pParent)
	{
		m_pParent = pParent;
	}

	template <SpaceType SPACE_TYPE>
	std::shared_ptr<typename Transform<SPACE_TYPE>::Type> Transform<SPACE_TYPE>::getRoot()
	{
		if (m_pParent == nullptr)
		{
			return this;
		}
		else
		{
			auto root = m_pParent;
			while (root.m_pParent != nullptr)
			{
				root = root.m_pParent
			}
			return root;
		}
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Transform<SPACE_TYPE>::getIsChanged()
	{
		return m_isChanged;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::getLocalPosition()
	{
		return m_localPosition;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setLocalPosition(PointType position)
	{
		m_localPosition = position;
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::getPosition()
	{
		return _getMatrixLocalToWorld(KGS_FALSE) * m_appliedLocalPosition;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getLocalRotation()
	{
		return m_appliedLocalRotation;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setLocalRotation(RotationType rotation)
	{
		m_localRotation = rotation;
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getRotation()
	{
		RotationType rotation = m_appliedLocalRotation;
		std::shared_ptr<Type> curr = m_pParent;
		while (curr != nullptr)
		{
			rotation = curr->m_appliedRotation * rotation;
			curr = curr->m_pParant;
		}
		return rotation;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getLocalScale()
	{
		return m_appliedLocalScale;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setLocalScale(VectorType scale)
	{
		m_localScale = scale;
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getScale()
	{
		VectorType scale = m_appliedLocalScale;
		auto curr = m_pParent;
		while (curr != nullptr)
		{
			scale = curr->m_appliedScale * scale;
			curr = curr->m_pParent;
		}
		return scale;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::transformVectorToWorld(VectorType vector)
	{
		return VectorType(_getMatrixLocalToWorld() * MatrixVectorType(vector, 0.0f));
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::transformVectorToLocal(VectorType vector)
	{
		return VectorType(_getMatrixWorldToLocal() * MatrixVectorType(vector, 0.0f));
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::transformPointToWorld(PointType point)
	{
		return _getMatrixLocalToWorld() * MatrixVectorType(point, 1.0f);
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::transformPointToLocal(PointType point)
	{
		return _getMatrixWorldToLocal() * MatrixVectorType(point, 1.0f);
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getMatrixLocalToWorld()
	{
		return _getMatrixLocalToWorld(KGS_TRUE);
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::getMatrixWorldToLocal()
	{
		return _getMatrixWorldToLocal(KGS_TRUE);
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::_getMatrixLocalToWorld(Bool32 includeSelf)
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
	typename Transform<SPACE_TYPE>::MatrixType Transform<SPACE_TYPE>::_getMatrixWorldToLocal(Bool32 includeSelf)
	{
		MatrixType matrix = _getMatrixLocalToWorld(includeSelf);
		glm::inverse(matrix);
		return matrix;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::_reCalculateLocalMatrix()
	{
		m_localMatrix = m_localPosition * m_localRotation * m_localScale;
	}
} //namespace kgs