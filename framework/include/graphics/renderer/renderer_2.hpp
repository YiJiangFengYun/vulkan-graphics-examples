#ifndef KGS_RENDERER_2_H
#define KGS_RENDERER_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/renderer/renderer.hpp"

namespace kgs
{
	class Renderer2 : public Renderer<SpaceType::SPACE_2>
	{
	public:
		Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
		);

		Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, uint32_t swapchainImageWidth
			, uint32_t swapchainImageHeight
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

		Renderer2(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		);

		Renderer2(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		);

	private:
		Renderer2() = delete;
		//void _update(UpdateInfo updateInfo) override;
		void _render(RenderInfo renderInfo) override;
		Bool32 _checkVisualObjectInsideCameraView(std::shared_ptr<typename SceneType::VisualObjectType> pVisualObject) override;
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H
