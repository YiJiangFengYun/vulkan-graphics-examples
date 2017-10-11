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

		Renderer3();
		Renderer3(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera);
	private:
		inline void _render() override;
		inline Bool32 _checkVisualObjectInsideCameraView(typename SceneType::VisualObjectType visualObject) override;
	};
} //namespace kgs

#endif // !KGS_RENDERER_2_H