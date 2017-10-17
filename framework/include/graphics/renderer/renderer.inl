namespace kgs
{
	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
		:BaseRenderer(pSwapchainImageView
			, swapchainImageFormat
			, swapchainImageWidth
			, swapchainImageHeight)
	{
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		:BaseRenderer(pSwapchainImageView
			, swapchainImageFormat
			, swapchainImageWidth
			, swapchainImageHeight)
		, m_pScene(pScene)
		, m_pCamera(pCamera)
	{
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		:BaseRenderer(pColorAttachmentTex)
	{
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: BaseRenderer(pColorAttachmentTex)
		, m_pScene(pScene)
		, m_pCamera(pCamera)
	{
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::reset(std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
	{
		m_pScene = pScene;
		m_pCamera = pCamera;
	}

	template <SpaceType SPACE_TYPE>
	Bool32 Renderer<SPACE_TYPE>::_isValidForRender()
	{
		if (m_pScene == nullptr || m_pCamera == nullptr)
		{
			if (m_pScene == nullptr)
			{
				//throw new std::runtime_error("Scene is not specified.");
				LOG(plog::warning) << "Scene is not specified." << std::endl;
			}

			if (m_pCamera == nullptr)
			{
				//throw new std::runtime_error("Camera is not specified.");
				LOG(plog::warning) << "Camera is not specified." << std::endl;
			}
			return KGS_FALSE;
		}
		return BaseRenderer::_isValidForRender();
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::_render(RenderInfo renderInfo)
	{
		if (m_pScene == nullptr)
		{
			throw new std::runtime_error("Scene is not specified.");
		}

		if (m_pCamera == nullptr)
		{
			throw new std::runtime_error("Camera is not specified.");
		}

		BaseRenderer::_render(renderInfo);
	}

	template <SpaceType SPACE_TYPE>
	typename SpaceTypeInfo<SPACE_TYPE>::MatrixType Renderer<SPACE_TYPE>::_getMVPMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject)
	{
		//return m_pCamera->getTransform().getMatrixWorldToLocal() * pObject->getTransform().getMatrixLocalToWorld();
		return _getMVMatrix(pObject);
	}

	template <SpaceType SPACE_TYPE>
	typename SpaceTypeInfo<SPACE_TYPE>::MatrixType Renderer<SPACE_TYPE>::_getMVMatrix(std::shared_ptr<typename SceneType::ObjectType> pObject)
	{
		return m_pCamera->getTransform().getMatrixWorldToLocal() * pObject->getTransform().getMatrixLocalToWorld();
	}

} //namespace kgs