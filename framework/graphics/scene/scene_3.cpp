#include "graphics/scene/scene_3.hpp"

namespace vg
{
	Scene3::Scene3()
		: Scene<SpaceType::SPACE_3>()
	{

	}

	Matrix4x4 Scene3::getProjMatrix(const CameraType *pCamera) const
	{
		auto projMatrix = pCamera->getProjMatrix();
		if (m_isRightHand == VG_FALSE)
		{
			projMatrix[1][1] *= -1;
		}
		else
		{
			vg::Vector3 eulerAngles = vg::Vector3(glm::radians(-90.0f), 0.0f, 0.0f);
			vg::Quaternion changeToGLCoord = vg::Quaternion(eulerAngles);
			projMatrix = projMatrix * glm::toMat4(changeToGLCoord);
		}
		return projMatrix;
	}

	Bool32 Scene3::isInView(const CameraType *pCamera
		, TransformType *pTransform
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
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
		auto mvpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();

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