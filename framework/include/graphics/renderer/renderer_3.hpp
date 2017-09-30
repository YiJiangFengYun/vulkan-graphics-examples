#ifndef KGS_RENDERER_3_H
#define KGS_RENDERER_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/renderer/renderer.hpp"

namespace kgs
{
	class Renderer3 : public Renderer<SpaceType::SPACE_3>
	{
	public:
		Renderer3();
		Renderer3(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera);
	private:
		void _render() override;
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H