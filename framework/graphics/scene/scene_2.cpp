#include "graphics/scene/scene_2.hpp"

#include "graphics/util/gemo_util.hpp"
#include "graphics/scene/camera_op_2.hpp"

namespace vg
{
	Scene2::Scene2()
		: Scene<SpaceType::SPACE_2>()
	{

	}

	Scene2::MatrixType Scene2::getProjMatrix(const CameraType *pCamera) const
	{
		auto projMatrixOfCamera = pCamera->getProjMatrix();
		if (m_isRightHand == VG_FALSE)
		{
			projMatrixOfCamera[1][1] *= -1;
			return projMatrixOfCamera;
		}
		else
		{
			return projMatrixOfCamera;
		}
	}

	Scene2::BoundsType Scene2::getViewBoundsInWorld(const CameraType *pCamera) const
	{
		using PointType = typename SpaceTypeInfo<SpaceType::SPACE_2>::PointType;
		using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_2>::MatrixVectorType;
		const CameraOP2 *pCameraOP2 = dynamic_cast<const CameraOP2 *>(pCamera);
		auto bounds = pCameraOP2->getViewBounds();

		//matrix from camera local to world.
		auto matrix = pCamera->getTransform()->getMatrixLocalToWorld();
		auto boundsInWorld = tranBoundsToNewSpace<PointType>(bounds, matrix, VG_FALSE);

		return boundsInWorld;
	}

	Bool32 Scene2::isInView(const CameraType *pCamera
		, TransformType *pTransform
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
	{
		//get MVP matrix.
		auto mvpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();
		auto boundsInView = tranBoundsToNewSpace<PointType>(bounds, mvpMatrix, VG_FALSE);
		BoundsType boundsOfView(PointType(-1.0f, -1.0f), PointType(1.0f, 1.0f));
		BoundsType intersectionInView;		
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

	Bool32 Scene2::isInView(const CameraType *pCamera
		    , BoundsType bounds
			, fd::Rect2D *viewRect) const
	{
		//get VP matrix.
		auto vpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal();
		auto boundsInView = tranBoundsToNewSpace<PointType>(bounds, vpMatrix, VG_FALSE);
		BoundsType boundsOfView(PointType(-1.0f, -1.0f), PointType(1.0f, 1.0f));
		BoundsType intersectionInView;		
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