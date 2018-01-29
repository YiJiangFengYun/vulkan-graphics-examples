#include "graphics/renderer/renderer_3.hpp"

#include <functional>

namespace vg
{
	Renderer3::Renderer3(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
		: Renderer(pSwapchainImageView
			, swapchainImageFormat
		    , swapchainImageWidth
		    , swapchainImageHeight)
	{

	}

	Renderer3::Renderer3(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: Renderer(pSwapchainImageView
			, swapchainImageFormat
			, swapchainImageWidth
			, swapchainImageHeight
			, pScene
			, pCamera)
	{

	}

	Renderer3::Renderer3(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		: Renderer(pColorAttachmentTex)
	{

	}

	Renderer3::Renderer3(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: Renderer(pColorAttachmentTex
			, pScene
			, pCamera)
	{

	}

	void Renderer3::_render(const RenderInfo &info, RenderResultInfo &resultInfo)
	{
		Renderer::_render(info, resultInfo);

		auto queueTypeCount = static_cast<uint32_t>(RenderQueueType::RANGE_SIZE);
		auto pDevice = pApp->getDevice();

		auto pScene = m_pScene;
		uint32_t visualObjectCount = pScene->getVisualObjectCount();
		
		//----------Preparing render.

		//Filter visualObject is out of camera with its bounds.
		std::vector<std::shared_ptr<SceneType::VisualObjectType>> validVisualObjects(visualObjectCount); //allocate enough space for array to storage points.
		uint32_t validVisualObjectCount(0u);
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{
			auto pVisualObject = pScene->getVisualObjectWithIndex(i);
			auto pMesh = pVisualObject->getMesh();
			auto bounds = dynamic_cast<SceneType::VisualObjectType::MeshType *>(pMesh.get())->getBounds();
			auto pTransform = pVisualObject->getTransform();
			if(m_pCamera->isInView(pTransform.get(), bounds) == VG_TRUE)
			{
				validVisualObjects[validVisualObjectCount++] = pVisualObject;
			}
		}

		//Get queue count for each queue type.
		std::vector<uint32_t> queueLengths(queueTypeCount, 0u);
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			++queueLengths[static_cast<size_t>(renderQueueType)];
		}

		std::vector<std::vector<std::shared_ptr<SceneType::VisualObjectType>>> queues(queueTypeCount);
		//Resize queues and reset quue counts to zero for preparing next use.
		for (uint32_t i = 0; i < queueTypeCount; ++i)
		{
			queues[i].resize(queueLengths[i], nullptr);
			queueLengths[i] = 0u;
		}

		//Update queues to point to visual object.
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			queues[static_cast<size_t>(renderQueueType)][queueLengths[static_cast<size_t>(renderQueueType)]++] = pVisualObject;
		}

		//sort transparent queue.
		std::sort(queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].begin(),
			queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].end(),
			std::bind(&Renderer3::_sortObjectsWithCameraZ, this, std::placeholders::_1, std::placeholders::_2));

		//Caculate total draw count.
		uint32_t drawCount = 0;
		for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
		{
			auto queueLength = queueLengths[typeIndex];
			for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
			{
				auto pVisualObject = queues[typeIndex][objectIndex];
				auto pMesh = pVisualObject->getMesh();
				auto subMeshCount = pMesh->getIndexData()->getSubIndexDataCount();
				auto pMaterial = pVisualObject->getMaterial();
				auto passCount = pMaterial->getPassCount();
				drawCount += subMeshCount * passCount;
			}
		}

		//-----Doing render.
		
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		std::vector<vk::SubmitInfo> submitInfos(drawCount);
		m_arrPLastSemaphores.resize(drawCount);
		m_arrSemaphores.resize(drawCount);

		auto projMatrix = m_pCamera->getProjMatrix();
		auto viewMatrix = m_pCamera->getTransform()->getMatrixWorldToLocal();

		uint32_t drawIndex = 0;
		for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
		{
			auto queueLength = queueLengths[typeIndex];
			for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
			{
				auto pVisualObject = queues[typeIndex][objectIndex];
				auto modelMatrix = pVisualObject->getTransform()->getMatrixLocalToWorld();
				auto mvMatrix = viewMatrix * modelMatrix;
				auto mvpMatrix = projMatrix * mvMatrix;
				auto pMesh = pVisualObject->getMesh();
				auto subMeshCount = pMesh->getSubMeshCount();
				auto pMaterial = pVisualObject->getMaterial();
				auto passCount = pMaterial->getPassCount();

				for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
				{
					//update building in matrix variable.
					auto pPass = pMaterial->getPassWithIndex(passIndex);

					pPass->_setBuildInMatrixData(mvpMatrix, mvMatrix, modelMatrix);

					pPass->apply();
				}

				for (uint32_t subMeshIndex = 0u; subMeshIndex < subMeshCount; ++subMeshIndex)
				{
					for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
					{
						std::shared_ptr<vk::Pipeline> pPipeline;
						_createPipelineForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);
						_recordCommandBufferForRender(pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);

						//submit
						std::shared_ptr<vk::Semaphore> pSemaphore = nullptr;
						if (m_arrPLastSemaphores[drawIndex] != nullptr)
						{
							pSemaphore = m_arrPLastSemaphores[drawIndex];
						}
						else
						{
							vk::SemaphoreCreateInfo createInfo = {
								vk::SemaphoreCreateFlags()
							};
							pSemaphore = fd::createSemaphore(pDevice, createInfo);
							m_arrPLastSemaphores[drawIndex] = pSemaphore;
						}
						m_arrSemaphores[drawIndex] = *pSemaphore;

						vk::SubmitInfo submitInfo = {
							info.waitSemaphoreCount,              //waitSemaphoreCount
							info.pWaitSemaphores,                 //pWaitSemaphores
							waitStages,                           //pWaitDstStageMask
							1u,                                   //commandBufferCount
							m_pCommandBuffer.get(),               //pCommandBuffers
							1u,                                   //signalSemaphoreCount
							pSemaphore.get()                      //pSignalSemaphores
						};

						submitInfos[drawIndex] = submitInfo;
						++drawIndex;
					}
				}
			}
		}

		LOG(plog::debug) << "Pre submit to grahics queue." << std::endl;
		vk::Queue queue;
		uint32_t queueIndex;
		pApp->allocateGaphicsQueue(queueIndex, queue);
		queue.submit(submitInfos, nullptr);
		pApp->freeGraphicsQueue(queueIndex);
		LOG(plog::debug) << "Post submit to grahics queue." << std::endl;

		resultInfo.signalSemaphoreCount = static_cast<uint32_t>(m_arrSemaphores.size());
		resultInfo.pSignalSemaphores = m_arrSemaphores.data();
	}

	Bool32 Renderer3::_sortObjectsWithCameraZ(std::shared_ptr<typename SceneType::ObjectType> pObject1, std::shared_ptr<typename SceneType::ObjectType> pObject2)
	{
		//get MV matrix.
		auto mvMatrix1 = _getMVMatrix(pObject1);
		auto mvMatrix2 = _getMVMatrix(pObject2);
		//get position
		auto pos1 = pObject1->getTransform()->getLocalPosition();
		auto pos2 = pObject2->getTransform()->getLocalPosition();

		typedef SpaceTypeInfo<SpaceType::SPACE_3>::PointType PointType;
		typedef SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType MatrixVectorType;
		//transform point from model coordinate system to camera coordinate system.
		pos1 = mvMatrix1 * MatrixVectorType(pos1, 1.0f);
		pos2 = mvMatrix2 * MatrixVectorType(pos2, 1.0f);

		//it is smaller if its z is bigger than the other.
		return static_cast<Bool32>(pos1.z > pos2.z);

	}
} //namespace kgs