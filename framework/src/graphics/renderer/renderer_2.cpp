#include "graphics/renderer/renderer_2.hpp"

namespace kgs
{
	Renderer2::Renderer2()
		:Renderer()
	{

	}

	Renderer2::Renderer2(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera)
		:Renderer(pScene, pCamera)
	{

	}

	void Renderer2::_render(RenderInfo renderInfo)
	{
		Renderer::_render(renderInfo);
		LOG(plog::debug) << "test" << std::endl;
	}
} //namespace kgs