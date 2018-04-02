#include "graphics/scene/scene_3.hpp"

#include "graphics/scene/camera_3.hpp"
#include "graphics/scene/camera_op_3.hpp"

namespace vg
{
	Scene3::Scene3()
		: Scene<SpaceType::SPACE_3>()
	{

	}

	Scene3::MatrixType Scene3::getProjMatrix(const CameraType *pCamera) const
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

	Scene3::BoundsType Scene3::getViewBoundsInWorld(const CameraType *pCamera) const
	{
		using PointType = typename SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
		using MatrixVectorType = typename SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
		BoundsType bounds;
		if (pCamera->getIsOrthographic() == VG_FALSE)
		{
			const Camera3 *pCamera3 = dynamic_cast<const Camera3 *>(pCamera);
			auto tanHalfFovy = glm::tan(pCamera3->getFovY() / 2.0f);
			float minZ = pCamera3->getZNear();
			float maxZ = pCamera3->getZFar();
			float maxX = maxZ * pCamera3->getAspect() * tanHalfFovy;
			float minX = -maxX;
			float maxY = maxZ * tanHalfFovy;
			float minY = -minY;
			bounds.setMinMax(PointType(minX, minY, minZ), PointType(maxX, maxY, maxZ));
		}
		else
		{
			const CameraOP3 *pCameraOP3 = dynamic_cast<const CameraOP3 *>(pCamera);
			bounds = pCameraOP3->getViewBounds();
		}

		//matrix from camera local to world.
		auto matrix = pCamera->getTransform()->getMatrixLocalToWorld();
		auto boundsInWorld = tranBoundsToNewSpace(bounds, matrix, VG_FALSE);

		return boundsInWorld;
	}

	Bool32 Scene3::isInView(const CameraType *pCamera
		, TransformType *pTransform
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
	{
		//get MVP matrix.
		auto mvpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();
		auto boundsInView = tranBoundsToNewSpace(bounds, mvpMatrix, pCamera->getIsOrthographic() == VG_FALSE);
		BoundsType boundsOfView(PointType(-1.0f, -1.0f, 0.0f), PointType(1.0f, 1.0f, 1.0f));
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