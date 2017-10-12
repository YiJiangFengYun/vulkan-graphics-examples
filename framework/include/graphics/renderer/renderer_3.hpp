#ifndef KGS_RENDERER_3_H
#define KGS_RENDERER_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/renderer/renderer.hpp"

namespace kgs
{
	class Renderer3 : public Renderer<SpaceType::SPACE_3>
	{
	public:
		enum class RenderQueueType
		{
			BACKGROUND,
			OPAQUE,
			TRANSPARENT,
			OVERLAY,
			BEGIN_RANGE = BACKGROUND,
			END_RANGE = OVERLAY,
			RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
		};

		inline RenderQueueType tranMaterialShowTypeToRenderQueueType(MaterialShowType showType)
		{
			return static_cast<RenderQueueType>(showType);
		}

		Renderer3(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
		);

		Renderer3(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

		Renderer3(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		);

		Renderer3(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);
	private:
		Renderer3() = delete;
		void _render(RenderInfo renderInfo) override;
		Bool32 _checkVisualObjectInsideCameraView(std::shared_ptr<typename SceneType::VisualObjectType> pVisualObject) override;
		Bool32 _sortObjectsWithCameraZ(std::shared_ptr<typename SceneType::ObjectType> pObject1, std::shared_ptr<typename SceneType::ObjectType> pObject2);

		inline SpaceTypeInfo<SpaceType::SPACE_3>::MatrixType _getMVPMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject) override
		{
			return m_pCamera->getProjMatrix() * m_pCamera->getTransform().getMatrixWorldToLocal() * pObject->getTransform().getMatrixLocalToWorld();
		}
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H