#include "graphics/renderer/renderer_3.hpp"

namespace kgs
{
	Renderer3::Renderer3()
		:Renderer()
	{

	}

	Renderer3::Renderer3(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera)
		:Renderer(pScene, pCamera)
	{

	}

	void Renderer3::_render()
	{
		Renderer::_render();

		std::shared_ptr<Scene<SpaceType::SPACE_3>> pScene;
		//Auto pScene = m_pScene;
		uint32_t visualObjectCount = pScene->getVisualObjectCount();

		//Filter visualObject is out of camera with its bounds.
		std::vector<std::shared_ptr<SceneType::VisualObjectType>> validVisualObjects(visualObjectCount); //allocate enough space for array to storage points.
		uint32_t validVisualObjectCount(0u);
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{

		}

		//Get queue count for each queue type.
		std::vector<uint32_t> queueCounts(static_cast<uint32_t>(RenderQueueType::RANGE_SIZE), 0u);
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{
			auto pVisualObject = pScene->getVisualObjectWithIndex(i);
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			++queueCounts[static_cast<size_t>(renderQueueType)];
		}

		std::vector<std::vector<std::shared_ptr<SceneType::VisualObjectType>>> queues(static_cast<uint32_t>(RenderQueueType::RANGE_SIZE));
		//Resize queues and reset to zero for preparing next use.
		for (uint32_t i = 0; i < static_cast<uint32_t>(RenderQueueType::RANGE_SIZE); ++i)
		{
			queues[i].resize(queueCounts[i], nullptr);
			queueCounts[i] = 0u;
		}

		//Update queues to point to visual object.
		for (uint32_t i = 0; i < visualObjectCount; ++i)
		{
			auto pVisualObject = pScene->getVisualObjectWithIndex(i);
			auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
			queues[static_cast<size_t>(renderQueueType)][queueCounts[static_cast<size_t>(renderQueueType)]++] = pVisualObject;
		}
	}

	Bool32 Renderer3::_checkVisualObjectInsideCameraView(SceneType::VisualObjectType visualObject)
	{
		/*auto pMesh = visualObject.getMesh();
		pMesh->getBounds();*/
		return KGS_FALSE;
	}
} //namespace kgs