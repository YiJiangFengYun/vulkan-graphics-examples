namespace kgs
{
	template <SpaceType SPACE_TYPE>
	Transform<SPACE_TYPE>::Transform()
		: BaseTransform()
		, m_pParent(nullptr)
		, m_arrPChildren()
		, m_mapPChildren()
		, m_isChanged(KGS_FALSE)
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
	uint32_t Transform<SPACE_TYPE>::getChildCount()
	{
		return static_cast<uint32_t>(m_arrPChildren.size());
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::detachChildren()
	{
		for (const auto& item : m_arrPChildren)
		{
			item->_setParentOnly(nullptr);
		}
		m_arrPChildren.resize(0u);
		m_mapPChildren.clear();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getChildWithIndex(uint32_t index)
	{
		return m_arrPChildren[index];
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Transform<SPACE_TYPE>::isChild(Type *pTransform)
	{
		return _isChild(pTransform);
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::addChild(Type *pNewChild)
	{
		if (_isChild(pNewChild)) return;

		_addChildOnly(pNewChild);
		pNewChild->_setParentOnly(this);
	}

	template <SpaceType SPACE_TYPE>
	void Transform<SPACE_TYPE>::removeChild(Type *pChild)
	{
		if (_isChild(pChild) == KGS_FALSE) return;

		_removeChildOnly(pNewChild);
		pNewChild->_setParentOnly(nullptr);
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
			_setParentOnly(nullptr);
			m_pParent->_removeChildOnly(this);
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
	typename Transform<SPACE_TYPE>::Type *Transform<SPACE_TYPE>::getRoot()
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
		_setLocalPositionOnly(position);
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
	typename void Transform<SPACE_TYPE>::setLocalRotation(RotationType rotation)
	{
		_setLocalRotationOnly(rotation);
		_reCalculateLocalMatrix();
	}

	template <SpaceType SPACE_TYPE>
	typename Transform<SPACE_TYPE>::RotationType Transform<SPACE_TYPE>::getRotation()
	{
		RotationType rotation = m_localRotation;
		Type *curr = m_pParent;
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
		_setLocalScaleOnly(scale);
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
	void Transform<SPACE_TYPE>::_removeChildOnly(Type *pChild)
	{
		std::remove(m_arrPChildren.begin(), m_arrPChildren.end(), pChild);
		m_mapPChildren.erase(pChild->getID());
	}

	template <SpaceType SPACE_TYPE>
	inline Bool32 Transform<SPACE_TYPE>::_isChild(Type *pTransform)
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
		m_localPosMatrix[length - 1] += normalMatrix[i];
		m_isChanged = KGS_TRUE;
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
		m_isChanged = KGS_TRUE;
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
		if (m_isChanged == KGS_FALSE) return;
		_setLocalMatrixOnly(m_localPosMatrix * m_localRotationMatrix * m_localScaleMatrix);
		m_isChanged = KGS_FALSE;
	}
} //namespace kgs