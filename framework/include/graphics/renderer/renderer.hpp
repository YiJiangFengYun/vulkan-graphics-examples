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

		void render(RenderInfo renderInfo)
		{
			_render(renderInfo);
		}

	protected:
		//compositions
		/*vk::Extent2D m_framebufferExtent;
		vk::Format m_colorImageFormat;
		vk::Format m_depthImageFormat;*/

		//aggregations
		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;
		virtual void _render(RenderInfo renderInfo) = 0;
		virtual Bool32 _checkVisualObjectInsideCameraView(std::shared_ptr<typename SceneType::VisualObjectType> pVisualObject) = 0;

		inline virtual typename SpaceTypeInfo<SPACE_TYPE>::MatrixType _getMVPMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject)
		{
			//return m_pCamera->getTransform().getMatrixWorldToLocal() * pObject->getTransform().getMatrixLocalToWorld();
			return _getMVMatrix(pObject);
		}

		inline virtual typename SpaceTypeInfo<SPACE_TYPE>::MatrixType _getMVMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject)
		{
			return m_pCamera->getTransform().getMatrixWorldToLocal() * pObject->getTransform().getMatrixLocalToWorld();
		}
	};
} //namespace kgs

#endif // !KGS_RENDERER_H
