#include "graphics/renderer/renderer_2.hpp"

namespace kgs
{
	Renderer2::Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
	)
		: Renderer(pSwapchainImageView
			, swapchainImageFormat)
	{

	}

	Renderer2::Renderer2(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: Renderer(pSwapchainImageView
			, swapchainImageFormat
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

	void Renderer2::_render(RenderInfo renderInfo)
	{
		Renderer::_render(renderInfo);
		LOG(plog::debug) << "test" << std::endl;
	}
} //namespace kgs