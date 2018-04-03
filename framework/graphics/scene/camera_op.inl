namespace vg
{
	template <SpaceType SPACE_TYPE>
	CameraOP<SPACE_TYPE>::CameraOP()
		: Camera<SPACE_TYPE>()
		, m_viewBounds(BoundsType::ValueType(-1.0f), BoundsType::ValueType(1.0f))
	{
		m_isOrthographic = VG_TRUE;
	}

	template <SpaceType SPACE_TYPE>
	void CameraOP<SPACE_TYPE>::updateProj(BoundsType viewBounds)
	{
		m_viewBounds = viewBounds;
		_applyProj();
	}

	template <SpaceType SPACE_TYPE>
	typename CameraOP<SPACE_TYPE>::TransformType::MatrixType CameraOP<SPACE_TYPE>::getProjMatrix() const
	{
		return m_projMatrix;
	}

	template <SpaceType SPACE_TYPE>
	typename CameraOP<SPACE_TYPE>::BoundsType CameraOP<SPACE_TYPE>::getViewBounds() const
	{
		return m_viewBounds;
	}
} //namespace kgs