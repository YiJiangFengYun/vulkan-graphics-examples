#ifndef KGS_RENDERER_H
#define KGS_RENDERER_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/renderer/renderer_option.hpp"
#include "graphics/texture/texture_color_attachment.hpp"
#include "graphics/texture/texture_depth_stencil_attachment.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Renderer
	{
	public:
		typedef Scene<SPACE_TYPE> SceneType;
		typedef Camera<SPACE_TYPE> CameraType;

		Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
		)
			: m_pSwapchainImageView(pSwapchainImageView)
			, m_swapchainImageFormat(swapchainImageFormat)
		{

		}

		Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
			, vk::Format swapchainImageFormat
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		)
			: m_pSwapchainImageView(pSwapchainImageView)
			, m_swapchainImageFormat(swapchainImageFormat)
			, m_pScene(pScene)
			, m_pCamera(pCamera)
		{

		}

		Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		)
			: m_pColorTexture(pColorAttachmentTex)
		{

		}

		Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		)
			: m_pColorTexture(pColorAttachmentTex)
			, m_pScene(pScene)
			, m_pCamera(pCamera)
		{

		}

		void render(RenderInfo renderInfo
			, std::shared_ptr<SceneType> pScene
			, std::shared_ptr<CameraType> pCamera
		)
		{
			m_pScene = pScene;
			m_pCamera = pCamera;
			_render(renderInfo);
		}

		void render(RenderInfo renderInfo)
		{
			_render(renderInfo);
		}

	protected:
		//compositions
		vk::Extent2D m_framebufferExtent;
		vk::Format m_colorImageFormat;
		vk::Format m_depthImageFormat;
		std::shared_ptr<vk::RenderPass> m_pRenderPass;
		std::shared_ptr<TextureDepthStencilAttachment> m_pDepthStencilTexture;

		//aggregations
		//Renderer will use swapchain image when color attachment texture is null.
		std::vector<std::shared_ptr<vk::ImageView>> m_pSwapchainImageView;
		vk::Format m_swapchainImageFormat;
		//Renderer will render to color texture when it is not null.
		std::shared_ptr<TextureColorAttachment> m_pColorTexture;

		std::shared_ptr<SceneType> m_pScene;
		std::shared_ptr<CameraType> m_pCamera;
		virtual void _render(RenderInfo renderInfo)
		{
			if (m_pScene == nullptr)
			{
				throw new std::runtime_error("Scene is not specified.");
			}

			if (m_pCamera == nullptr)
			{
				throw new std::runtime_error("Camera is not specified.");
			}
		}

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

	private:
		Renderer() = delete;
	};
} //namespace kgs

#endif // !KGS_RENDERER_H
