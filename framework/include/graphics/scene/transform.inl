namespace kgs
{
	template <SpaceType SPACE_TYPE>
	Transform<SPACE_TYPE>::Transform()
		: BaseTransform()
		, m_pParent(nullptr)
		, m_pChildren()
		, m_isChanged(KGS_FALSE)
		, m_localPosition(0.0f)
		, m_localPosMatrix(1.0f)
		, m_localScale(1.0f)
		, m_localScaleMatrix(1.0f)
		, m_localRotation()
		, m_localRotationMatrix(1.0f)
		, m_localMatrix(1.0f)
	{

	}

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
		typename MatrixVectorType::length_type i;
		typename MatrixVectorType::length_type length = MatrixVectorType::length();
		MatrixType normalMatrix(1.0f);
		m_localPosMatrix = normalMatrix;
		for (i = 0; i < length - 1; ++i)
		{
			m_localPosMatrix[length - 1] += normalMatrix[i] * position[i];
		}
		m_localPosMatrix[length - 1] += normalMatrix[i];
		//Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::PointType Transform<SPACE_TYPE>::getPosition()
	{
		return _getMatrixLocalToWorld(KGS_FALSE) * MatrixVectorType(m_localPosition, 0.0);
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getLocalRotation()
	{
		return m_localPosition;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getRotation()
	{
		RotationType rotation = m_localRotation;
		std::shared_ptr<Type> curr = m_pParent;
		while (curr != nullptr)
		{
			rotation = curr->m_localRotation * rotation;
			curr = curr->m_pParant;
		}
		return rotation;
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getLocalScale()
	{
		return m_localScale;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::setLocalScale(VectorType scale)
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
		/*tmat4x4<T, P> Result(uninitialize);
		Result[0] = m[0] * v[0];
		Result[1] = m[1] * v[1];
		Result[2] = m[2] * v[2];
		Result[3] = m[3];
		return Result;*/
		m_isChanged = KGS_TRUE;
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::VectorType Transform<SPACE_TYPE>::getScale()
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
		matrix = glm::inverse(matrix);
		return matrix;
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::_reCalculateLocalMatrix()
	{
		m_localMatrix = m_localPosMatrix * m_localRotationMatrix * m_localScaleMatrix;
	}
} //namespace kgs