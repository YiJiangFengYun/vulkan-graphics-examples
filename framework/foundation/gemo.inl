namespace fd
{
	template <typename VecType = glm::vec3>
	Ray<VecType>::Ray()
	{

	}

	template <typename VecType = glm::vec3>
	Ray<VecType>::Ray(ValueType origin, ValueType direction) 
		: m_origin(origin)
		, m_direction(direction)
		, m_isUpdateCache(FD_TRUE)
	{
#ifdef DEBUG
		vec_value_type distSqr = glm::dot(direction, direction);
		if (distSqr == 0)
			throw std::invalid_argument("Invalid direction, direction length is 0.");
#endif // DEBUG
	}

	template <typename VecType = glm::vec3>
	Ray<VecType>::Ray(const Ray<ValueType>& target) 
		: m_origin(target.m_origin)
		, m_direction(target.m_direction)
		, m_isUpdateCache(target.m_isUpdateCache)
		, m_invDir(target.m_invDir)
		, m_signs(target.m_signs)
	{
	}

	template <typename VecType = glm::vec3>
	Ray<VecType>& Ray<VecType>::operator =(const Ray<ValueType>& target)
	{
		m_origin = target.m_origin;
		m_direction = target.m_direction;
		m_isUpdateCache = target.m_isUpdateCache;
		m_invDir = target.m_invDir;
		m_signs = target.m_signs;
		return *this;
	}

	template <typename VecType = glm::vec3>
	typename Ray<VecType>::ValueType Ray<VecType>::getOrigin()
	{
		return m_origin;
	}

	template <typename VecType = glm::vec3>
	void Ray<VecType>::setOrigin(ValueType origin)
	{
		m_origin = origin;
		m_isUpdateCache = true;
	}

	template <typename VecType = glm::vec3>
	typename Ray<VecType>::ValueType Ray<VecType>::getDirection()
	{
		return m_direction;
	}

	template <typename VecType = glm::vec3>
	void Ray<VecType>::setDirection(ValueType direction)
	{
		m_direction = direction;
		m_isUpdateCache = true;
	}

	template <typename VecType = glm::vec3>
	typename Ray<VecType>::ValueType Ray<VecType>::getInvDir()
	{
		_updateCache();
		return m_invDir;
	}

	template <typename VecType = glm::vec3>
	typename Ray<VecType>::ValueType Ray<VecType>::getSigns()
	{
		_updateCache();
		return m_signs;
	}

	template <typename VecType = glm::vec3>
	typename Ray<VecType>::ValueType Ray<VecType>::getPoint(vec_value_type distance)
	{
		ValueType direction = m_direction;
		glm::normalize(direction);
		direction *= distance;
		direction += m_origin;
		return direction;
	}

	template <typename VecType = glm::vec3>
	void Ray<VecType>::_updateCache()
	{
		if (m_isUpdateCache)
		{
			m_isUpdateCache = FD_FALSE;
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_invDir[i] = 1 / m_direction[i];
				//negative is 0 and positive is 1, it is better to be used as array index.
				m_signs[i] = m_direction[i] < 0 ? 0 : 1;
			}
		}
	}

	template <typename VecType = glm::vec3>
	Bounds<VecType>::Bounds()
	{
		_updateSize();
	}

	template <typename VecType = glm::vec3>
	Bounds<VecType>::Bounds(ValueType min, ValueType max)
		: m_min(min)
		, m_max(max)
	{
		_updateSize();
	}

	template <typename VecType = glm::vec3>
	Bounds<VecType>::Bounds(const Bounds<ValueType>& target)
		: m_min(target.m_min)
		, m_max(target.m_max)
		, m_size(target.m_size)
	{
	}

	template <typename VecType = glm::vec3>
	Bounds<VecType>::~Bounds()
	{

	}

	template <typename VecType = glm::vec3>
	Bounds<VecType>& Bounds<VecType>::operator =(const Bounds<ValueType>& target)
	{
		m_min = target.m_min;
		m_max = target.m_max;
		m_size = target.m_size;
		return *this;
	}

	template <typename VecType = glm::vec3>
	typename Bounds<VecType>::ValueType Bounds<VecType>::getMin() { return m_min; }

	template <typename VecType = glm::vec3>
	typename Bounds<VecType>::ValueType Bounds<VecType>::getMax() { return m_max; }

	template <typename VecType = glm::vec3>
	void Bounds<VecType>::setMinMax(ValueType min, ValueType max)
	{
		m_min = min;
		m_max = max;
		_updateSize();
	}

	template <typename VecType = glm::vec3>
	typename Bounds<VecType>::ValueType Bounds<VecType>::getSize() { return m_size; }

	template <typename VecType = glm::vec3>
	void Bounds<VecType>::setSize(ValueType size)
	{
#ifdef DEBUG
		length_type length = ValueType::length();
		for (length_type i = 0; i < length; ++i)
		{
			if (size[i] < 0)throw std::invalid_argument("Invalid size of Bounds");
		}
#endif // DEBUG
		m_size = size;
		_updateMax();
	}

	template <typename VecType = glm::vec3>
	typename Bounds<VecType>::ValueType Bounds<VecType>::getClosestPoint(ValueType point)
	{
		length_type length = ValueType::length();
		ValueType result;
		for (length_type i = 0; i < length; ++i)
		{
			if (point[i] < m_min[i]) result[i] = m_min[i];
			else if (point[i] < m_max[i]) result[i] = point[i];
			else result[i] = m_max[i];
		}
		return result;
	}

	template <typename VecType = glm::vec3>
	Bool32 Bounds<VecType>::isContains(ValueType point)
	{
		length_type length = ValueType::length();
		for (length_type i = 0; i < length; ++i)
		{
			if (point[i] < m_min[i] || point[i] > m_max[i])
				return false;
		}
		return true;
	}

	template <typename VecType = glm::vec3>
	void Bounds<VecType>::expand(vec_value_type amount)
	{
		vec_value_type halt = amount / 2;
		length_type length = ValueType::length();
		for (length_type i = 0; i < length; ++i)
		{
			m_min[i] -= halt;
			m_max[i] += halt;
			m_size[i] += amount;
		}
	}

	template <typename VecType = glm::vec3>
	Bool32 Bounds<VecType>::isIntersects(Bounds<ValueType> bounds)
	{
		typename ValueType::length_type length = ValueType::length();
		for (typename ValueType::length_type i = 0; i < length; ++i)
		{
			if (m_min[i] > bounds.m_max[i] || m_max[i] < bounds.m_min[i])
				return false;
		}
		return true;
	}

	template <typename VecType = glm::vec3>
	typename Bounds<VecType>::vec_value_type Bounds<VecType>::getSqrDistance(ValueType point)
	{
		ValueType closestPoint = getClosestPoint(point);
		closestPoint -= point;
		return glm::dot(closestPoint, closestPoint);
	}

	template <typename VecType = glm::vec3>
	void Bounds<VecType>::_updateSize()
	{
		length_type length = ValueType::length();
		for (length_type i = 0; i < length; ++i)
		{
			m_size[i] = m_max[i] - m_min[i];
		}
#ifdef DEBUG
		for (length_type i = 0; i < length; ++i)
		{
			if (m_size[i] < 0)
				throw std::invalid_argument("Invalid size of Bounds");
		}
#endif // DEBUG
	}

	template <typename VecType = glm::vec3>
	void Bounds<VecType>::_updateMax()
	{
		length_type length = ValueType::length();
		for (length_type i = 0; i < length; ++i)
		{
			m_max[i] = m_min[i] + m_size[i];
		}
	}
}