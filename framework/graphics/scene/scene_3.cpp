#include "graphics/scene/scene_3.hpp"

#include "graphics/util/gemo_util.hpp"
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
			float minY = -maxY;
			bounds.setMinMax(PointType(minX, minY, minZ), PointType(maxX, maxY, maxZ));
		}
		else
		{
			const CameraOP3 *pCameraOP3 = dynamic_cast<const CameraOP3 *>(pCamera);
			bounds = pCameraOP3->getViewBounds();
		}

		//matrix from camera local to world.
		auto matrix = pCamera->getTransform()->getMatrixLocalToWorld();
		auto boundsInWorld = tranBoundsToNewSpace<PointType>(bounds, matrix, VG_FALSE);

		return boundsInWorld;
	}

	Bool32 Scene3::isInView(const CameraType *pCamera
		, TransformType *pTransform
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
	{
		if (pCamera->getIsOrthographic() == VG_TRUE) 
		{
			//get MVP matrix.
		    auto mvpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();
		    auto boundsInView = tranBoundsToNewSpace<PointType>(bounds, mvpMatrix, VG_FALSE);
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
		else 
		{
			//avoid error of calculating result projected bounds when bounds cross with z = 0 plane,
			//we should separate to multiply step to caluclate result bounds.
		    //1.first, we should get mv matrix to calculate bounds in camera space.
		    auto mvMatrix = pCamera->getTransform()->getMatrixWorldToLocal() * pTransform->getMatrixLocalToWorld();
		    auto boundsInCamera = tranBoundsToNewSpace<PointType>(bounds, mvMatrix, VG_FALSE);
			//2.then, we clip bounds by z far and z near planes.
			auto min = boundsInCamera.getMin();
			auto max = boundsInCamera.getMax();
			const Camera3 *pCamera3 = dynamic_cast<const Camera3 *>(pCamera);
			auto zNear = pCamera3->getZNear();
			auto zFar = pCamera3->getZFar();
			if (min.z < zNear)
			{
				min.z = zNear;
				if (max.z < zNear) max.z = zNear;
			}
			if (max.z > zFar)
			{
				max.z = zFar;
				if (min.z > zFar) min.z = zFar;
			}
			boundsInCamera.setMinMax(min, max);
			//3. final, we get bounds in normalize device space.
			auto projMatrx = getProjMatrix(pCamera);
			auto boundsInView = tranBoundsToNewSpace<PointType>(boundsInCamera, projMatrx, VG_TRUE);
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
	}

	Bool32 Scene3::isInView(const CameraType *pCamera
		, BoundsType bounds
		, fd::Rect2D *viewRect) const
	{
		//get VP matrix.
		auto vpMatrix = getProjMatrix(pCamera) * pCamera->getTransform()->getMatrixWorldToLocal();
		auto boundsInView = tranBoundsToNewSpace<PointType>(bounds, vpMatrix, pCamera->getIsOrthographic() == VG_FALSE);
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