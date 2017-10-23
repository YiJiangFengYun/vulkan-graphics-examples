#include "graphics/renderer/renderer_2.hpp"

namespace kgs
{
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

	/*void Renderer2::_update(UpdateInfo updateInfo)
	{
		Renderer::_update(updateInfo);
	}*/

	void Renderer2::_render(RenderInfo renderInfo)
	{
		Renderer::_render(renderInfo);
	}

	Bool32 Renderer2::_checkVisualObjectInsideCameraView(std::shared_ptr<typename SceneType::VisualObjectType> pVisualObject)
	{
		return KGS_TRUE;
	}
} //namespace kgs