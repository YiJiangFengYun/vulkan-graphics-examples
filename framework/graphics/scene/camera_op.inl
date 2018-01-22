namespace vg
{
	template <SpaceType SPACE_TYPE>
	CameraOP<SPACE_TYPE>::CameraOP()
		: Camera<SPACE_TYPE>()
		, m_viewBounds(BoundsType::ValueType(-1.0f), BoundsType::ValueType(1.0f))
	{
		apply();
	}

	template <SpaceType SPACE_TYPE>
	void CameraOP<SPACE_TYPE>::apply()
	{

	}

	template <SpaceType SPACE_TYPE>
	void CameraOP<SPACE_TYPE>::updateProj(BoundsType viewBounds)
	{
		m_viewBounds = viewBounds;
		apply();
	}

	template <SpaceType SPACE_TYPE>
	typename CameraOP<SPACE_TYPE>::TransformType::MatrixType CameraOP<SPACE_TYPE>::getProjMatrix()
	{
		return m_projMatrix;
	}

	template <SpaceType SPACE_TYPE>
	typename CameraOP<SPACE_TYPE>::BoundsType CameraOP<SPACE_TYPE>::getViewBounds()
	{
		return m_viewBounds;
	}

	template <SpaceType SPACE_TYPE>
	Bool32 CameraOP<SPACE_TYPE>::isInView(Transform<SPACE_TYPE> *pTransform, BoundsType bounds)
	{
		auto min = bounds.getMin();
		auto max = bounds.getMax();
		typedef typename SpaceTypeInfo<SPACE_TYPE>::PointType PointType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixVectorType MatrixVectorType;
		typename PointType::length_type len = PointType::length();

		//use permutation combination algorithm to get all points of bounds.
		uint32_t pointCount = static_cast<uint32_t>(std::pow(2u, len));
		std::vector<MatrixVectorType> points(pointCount);
		uint32_t pointIndex = 0u;
		std::vector<Bool32> places(len);
		for (typename PointType::length_type i = 0; i <= len; ++i)
		{
			std::fill(places.begin(), places.end(), VG_FALSE);
			std::fill(places.begin(), places.begin() + i, VG_TRUE);
			do
			{
				typename PointType::length_type j;
				for (j = 0; j < len; ++j)
				{
					if (places[j] == VG_TRUE)
					{
						points[pointIndex][j] = min[j];
					}
					else
					{
						points[pointIndex][j] = max[j];
					}
				}
				points[pointIndex][j] = 1.0f; //final number is 1.0 for point.
				++pointIndex;
			} while (std::prev_permutation(places.begin(), places.end()));
		}

		//get MV matrix.
		auto mvMatrix = m_pTransform->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();

		//transform point from model coordinate system to view coordinate system.
		for (uint8_t i = 0; i < pointCount; ++i)
		{
			points[i] = mvMatrix * points[i];
		}

		PointType minInView;
		PointType maxInView;
		
		for (typename PointType::length_type i = 0; i < len; ++i)
		{
			typename PointType::value_type min = std::numeric_limits<typename PointType::value_type>::max(), max = -std::numeric_limits<typename PointType::value_type>::max();
			for (uint8_t j = 0; j < pointCount; ++j)
			{
				if (min > points[j][i])min = points[j][i];
				if (max < points[j][i])max = points[j][i];
			}
			minInView[i] = min;
			maxInView[i] = max;
		}

		fd::Bounds<PointType> boundsInView(minInView, maxInView);

		Bool32 isInsideCameraView = VG_FALSE;
		////check if it is inside camera view.
		if (m_viewBounds.isIntersects(boundsInView))
		{
			isInsideCameraView = VG_TRUE;
		}

		return isInsideCameraView;
	}
} //namespace kgs