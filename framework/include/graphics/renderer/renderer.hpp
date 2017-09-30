#ifndef KGS_RENDERER_H
#define KGS_RENDERER_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"

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
			: m_PScene(pScene)
			, m_pCamera(pCamera)
		{

		}

		void render()
		{

		}
	protected:
		//aggregations
		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;

	};
} //namespace kgs

#endif // !KGS_RENDERER_H
