#include "graphics/renderer/renderer_2.hpp"

namespace kgs
{
	void fillValidVisualObjects(
		std::vector<std::shared_ptr<VisualObject<SpaceType::SPACE_2>>> &arrPVObjs,
		uint32_t &PVObjIndex,
		Transform<SpaceType::SPACE_2> *pTransform,
		std::shared_ptr<Scene<SpaceType::SPACE_2>> &pScene,
		std::shared_ptr<Camera<SpaceType::SPACE_2>> &pCamera);

	Renderer2::Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
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

	Renderer2::Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
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

	Renderer2::Renderer2(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		: Renderer(pColorAttachmentTex)
	{

	}

	Renderer2::Renderer2(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: Renderer(pColorAttachmentTex
			, pScene
			, pCamera)
	{

	}

	void Renderer2::_render(const RenderInfo &info, RenderResultInfo &resultInfo)
	{
		Renderer::_render(info, resultInfo);

		auto pDevice = pApp->getDevice();

		auto pScene = m_pScene;
		uint32_t visualObjectCount = pScene->getVisualObjectCount();

		//flat visual objects and filter them that is out of camera with its bounds.
		//allocate enough space for array to storage points.
		std::vector<std::shared_ptr<SceneType::VisualObjectType>> validVisualObjects(visualObjectCount);
		uint32_t validVisualObjectCount(0u);
		auto pRoot = pScene->pRootTransformForVisualObject;
		auto pTransform = pRoot;
		fillValidVisualObjects(validVisualObjects
			, validVisualObjectCount
			, pTransform.get()
			, pScene, m_pCamera
		);

		//Caculate total draw count.
		uint32_t drawCount = 0;
		for (uint32_t i = 0; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
			auto pMesh = pVisualObject->getMesh();
			auto subMeshCount = pMesh->_getSubMeshCountForRender();
			auto pMaterial = pVisualObject->getMaterial();
			auto passCount = pMaterial->getPassCount();
			drawCount += subMeshCount * passCount;
		}

		//------Doing render.
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		std::vector<vk::SubmitInfo> submitInfos(drawCount);
		m_arrPLastPipelineLayouts.resize(drawCount);
		m_arrPLastPipelines.resize(drawCount);
		m_arrPLastSemaphores.resize(drawCount);
		m_arrSemaphores.resize(drawCount);

		auto projMatrix = m_pCamera->getProjMatrix();
		auto viewMatrix = m_pCamera->getTransform()->getMatrixWorldToLocal();

		uint32_t drawIndex = 0u;
		for (uint32_t i = 0u; i < validVisualObjectCount; ++i)
		{
			auto pVisualObject = validVisualObjects[i];
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
					std::shared_ptr<vk::PipelineLayout> pPipelineLayout;
					std::shared_ptr<vk::Pipeline> pPipeline;
					_createPipelineForRender(pPipelineLayout, pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);
					m_arrPLastPipelineLayouts[drawIndex] = pPipelineLayout;
					m_arrPLastPipelines[drawIndex] = pPipeline;
					_recordCommandBufferForRender(pPipelineLayout, pPipeline, pMesh, pMaterial, subMeshIndex, passIndex);

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

	void fillValidVisualObjects(
		std::vector<std::shared_ptr<VisualObject<SpaceType::SPACE_2>>> &arrPVObjs,
		uint32_t &PVObjIndex,
		Transform<SpaceType::SPACE_2> *pTransform,
		std::shared_ptr<Scene<SpaceType::SPACE_2>> &pScene,
	    std::shared_ptr<Camera<SpaceType::SPACE_2>> &pCamera)
	{
		std::shared_ptr<VisualObject<SpaceType::SPACE_2>> pVisualObjectOfChild;
		uint32_t childCount = pTransform->getChildCount();
		Transform<SpaceType::SPACE_2> *pChild;
		for (uint32_t i = 0; i < childCount; ++i)
		{
			pChild = pTransform->getChildWithIndex(i);
			//Children's visual object is placed ahead of own visual objects
			fillValidVisualObjects(arrPVObjs, PVObjIndex, pChild, pScene, pCamera);
			//Own visual object is placed behind children's visual object.
			pVisualObjectOfChild = pScene->getVisualObjectWithTransform(pChild);
			//Filter obj out of camera view.
			auto pMeshOfChild = pVisualObjectOfChild->getMesh();
			auto boundsOfChild = dynamic_cast<Mesh<MeshType::SPACE_2> *>(pMeshOfChild.get())->getBounds();
			if (pCamera->isInView(pChild, boundsOfChild) == KGS_TRUE)
			{
				arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
			}
		}
	}

} //namespace kgs