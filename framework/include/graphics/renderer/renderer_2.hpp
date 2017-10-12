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
		);

		Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
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
		void _render(RenderInfo renderInfo) override;
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H
