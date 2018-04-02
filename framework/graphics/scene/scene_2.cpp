#include "graphics/scene/scene_2.hpp"

#include "graphics/util/gemo_util.hpp"
namespace vg
{
	Scene2::Scene2()
		: Scene<SpaceType::SPACE_2>()
	{

	}

	Matrix4x4 Scene2::getProjMatrix(const CameraType *pCamera) const
	{
		auto projMatrixOfCamera = pCamera->getProjMatrix();
		if (m_isRightHand == VG_FALSE)
		{
			projMatrixOfCamera[1][1] *= -1;
			return tranMat3ToMat4(projMatrixOfCamera);
		}
		else
		{
			// vg::Vector3 eulerAngles = vg::Vector3(glm::radians(180.0f), 0.0f, 0.0f);
			// vg::Quaternion changeToGLCoord = vg::Quaternion(eulerAngles);
			// return tranMat3ToMat4(projMatrixOfCamera) * glm::toMat4(changeToGLCoord);
			return tranMat3ToMat4(projMatrixOfCamera);
		}
	}

	Bool32 Scene2::isInView(const CameraType *pCamera
		, TransformType *pTransform
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
	{
		auto min = bounds.getMin();
		auto max = bounds.getMax();
		using PointType = typename SpaceTypeInfo<SpaceType::SPACE_2>::PointType;
		using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_2>::MatrixVectorType;
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
		auto mvpMatrix = getProjMatrix(pCamera) * tranMat3ToMat4(pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld());

		//transform point from model coordinate system to normalize device coordinate system.
		const typename PointType::value_type epsilon = std::numeric_limits<typename PointType::value_type>::epsilon();
		for (uint8_t i = 0; i < pointCount; ++i)
		{
			points[i] = mvpMatrix * vg::Vector4(points[i].x, points[i].y, 0.0f, points[i].z);
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
		fd::Bounds<PointType> boundsOfView(PointType(-1.0f, -1.0f), PointType(1.0f, 1.0f));
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