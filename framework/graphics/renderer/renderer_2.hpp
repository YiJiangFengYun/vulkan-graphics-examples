#ifndef VG_RENDERER_2_H
#define VG_RENDERER_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/renderer/renderer.hpp"

namespace vg
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
		void _render(const RenderInfo &info, RenderResultInfo &resultInfo) override;
	};
} //namespace kgs

#endif // !VG_RENDERER_2_H
