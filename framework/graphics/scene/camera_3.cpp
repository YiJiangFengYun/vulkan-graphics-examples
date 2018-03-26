#include "graphics/scene/camera_3.hpp"

namespace vg
{
	const float Camera3::DEFAULT_FOVY = glm::radians(45.0f);
	const float Camera3::DEFAULT_ASPECT = 1.0f;
	const float Camera3::DEFAULT_Z_NEAR = 0.1f;
	const float Camera3::DEFAULT_Z_FAR = 10.0f;
	Camera3::Camera3()
		: Camera<SpaceType::SPACE_3>()
		, m_fovy(DEFAULT_FOVY)
		, m_aspect(DEFAULT_ASPECT)
		, m_zNear(DEFAULT_Z_NEAR)
		, m_zFar(DEFAULT_Z_FAR)
	{
		apply();
	}

	void Camera3::updateProj(float fovy, float aspect, float zNear, float zFar)
	{
		m_fovy = fovy;
		m_aspect = aspect;
		m_zNear = zNear;
		m_zFar = zFar;
	}

	void Camera3::apply()
	{
		m_projMatrix = glm::perspective(m_fovy, m_aspect, m_zNear, m_zFar);
		//GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted. 
		// m_projMatrix[1][1] *= -1;
	}

	float Camera3::getFovY() const
	{
		return m_fovy;
	}

	void Camera3::setFovY(float fovy)
	{
		m_fovy = fovy;
	}

	float Camera3::getAspect() const
	{
		return m_aspect;
	}

	void Camera3::setAspect(float aspect)
	{
		m_aspect = aspect;
	}

	float Camera3::getZNear() const
	{
		return m_zNear;
	}

	void Camera3::setZNear(float zNear)
	{
		m_zNear = zNear;
	}

	float Camera3::getZFar() const
	{
		return m_zFar;
	}

	void Camera3::setZFar(float zFar)
	{
		m_zFar = zFar;
	}

	typename Camera3::TransformType::MatrixType Camera3::getProjMatrix() const
	{
		return m_projMatrix;
	}

	Bool32 Camera3::isInView(Transform<SpaceType::SPACE_3> *pTransform, BoundsType bounds, fd::Rect2D *viewRect) const
	{
		auto min = bounds.getMin();
		auto max = bounds.getMax();
		using PointType = typename SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
		using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
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

		//get MVP matrix.
		auto mvpMatrix = m_projMatrix * m_pTransform->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();

		//transform point from model coordinate system to normalize device coordinate system.
		const typename PointType::value_type epsilon = std::numeric_limits<typename PointType::value_type>::epsilon();
		for (uint8_t i = 0; i < pointCount; ++i)
		{
			points[i] = mvpMatrix * points[i];
			if(glm::abs(points[i].w) > epsilon)
			{
				points[i] = points[i] / points[i].w;
			}
			else
			{
				typename PointType::length_type j;
				for (j = 0; j < len; ++j) {
					if (j < len - 1) {
						//x y value is infinite.
						points[i][j] = std::numeric_limits<typename PointType::value_type>::max();
					} else {
						// z value is zero.
						points[i][j] = 0.0f;
					}
				}
			}
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
		fd::Bounds<PointType> boundsOfView(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
		fd::Bounds<PointType> intersectionInView;		
		Bool32 isInsideCameraView = VG_FALSE;
		////check if it is inside camera view.
		if (boundsOfView.intersects(boundsInView, &intersectionInView))
		{
			isInsideCameraView = VG_TRUE;
			if (viewRect != nullptr)
		    {
		    	auto min = intersectionInView.getMin();
		    	auto size = intersectionInView.getSize();
		    	(*viewRect).x = min.x;
		    	(*viewRect).y = min.y;
		    	(*viewRect).width = size.x;
		    	(*viewRect).height = size.y;
		    }
		}

		

		return isInsideCameraView;
	}

} //namespace kgs