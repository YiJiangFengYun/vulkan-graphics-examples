#ifndef KGS_RENDERER_H
#define KGS_RENDERER_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/renderer/renderer_option.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Renderer
	{
	public:
		typedef Scene<SPACE_TYPE> SceneType;
		typedef Camera<SPACE_TYPE> CameraType;

		Renderer()
		{

		}

		Renderer(std::shared_ptr<SceneType> pScene, std::shared_ptr<CameraType> pCamera)
			: m_pScene(pScene)
			, m_pCamera(pCamera)
		{

		}

		void render()
		{
			_render();
		}

	protected:
		//compositions

		//aggregations
		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;
		inline virtual void _render() = 0;
		inline virtual Bool32 _checkVisualObjectInsideCameraView(typename SceneType::VisualObjectType visualObject) = 0;


	};
} //namespace kgs

#endif // !KGS_RENDERER_H
