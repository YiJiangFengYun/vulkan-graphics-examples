#ifndef KGS_RENDERER_2_H
#define KGS_RENDERER_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/renderer/renderer.hpp"

namespace kgs
{
	class Renderer2 : public Renderer<SpaceType::SPACE_2>
	{
	public:
		Renderer2();
		Renderer2(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera);
	private:
		void _render() override;
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H