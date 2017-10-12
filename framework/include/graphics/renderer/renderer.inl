namespace kgs
{
	template <SpaceType SPACE_TYPE>
	const vk::Format Renderer<SPACE_TYPE>::DEFAULT_DEPTH_STENCIL_FORMAT(vk::Format::eD32SfloatS8Uint);

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
	)
		: m_pSwapchainImageView(pSwapchainImageView)
		, m_swapchainImageFormat(swapchainImageFormat)
		, m_colorImageFormat(swapchainImageFormat)
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(swapchainImageWidth)
		, m_framebufferHeight(swapchainImageHeight)
	{
		_init();
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<vk::ImageView> pSwapchainImageView
		, vk::Format swapchainImageFormat
		, uint32_t swapchainImageWidth
		, uint32_t swapchainImageHeight
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: m_pSwapchainImageView(pSwapchainImageView)
		, m_swapchainImageFormat(swapchainImageFormat)
		, m_colorImageFormat(swapchainImageFormat)
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(swapchainImageWidth)
		, m_framebufferHeight(swapchainImageHeight)
		, m_pScene(pScene)
		, m_pCamera(pCamera)
	{
		_init();
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
	)
		: m_pColorTexture(pColorAttachmentTex)
		, m_colorImageFormat(pColorAttachmentTex->_getVKFormat())
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(pColorAttachmentTex->getWidth())
		, m_framebufferHeight(pColorAttachmentTex->getHeight())
	{
		_init();
	}

	template <SpaceType SPACE_TYPE>
	Renderer<SPACE_TYPE>::Renderer(std::shared_ptr<TextureColorAttachment> pColorAttachmentTex
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
		: m_pColorTexture(pColorAttachmentTex)
		, m_colorImageFormat(pColorAttachmentTex->_getVKFormat())
		, m_depthStencilImageFormat(DEFAULT_DEPTH_STENCIL_FORMAT)
		, m_framebufferWidth(pColorAttachmentTex->getWidth())
		, m_framebufferHeight(pColorAttachmentTex->getHeight())
		, m_pScene(pScene)
		, m_pCamera(pCamera)
	{
		_init();
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::render(RenderInfo renderInfo
		, std::shared_ptr<SceneType> pScene
		, std::shared_ptr<CameraType> pCamera
	)
	{
		m_pScene = pScene;
		m_pCamera = pCamera;
		_render(renderInfo);
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::render(RenderInfo renderInfo)
	{
		_render(renderInfo);
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::_createRenderPass()
	{
		vk::AttachmentDescription colorAttachment = {
			vk::AttachmentDescriptionFlags(),
			m_colorImageFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			m_pColorTexture == nullptr ? vk::ImageLayout::ePresentSrcKHR : vk::ImageLayout::eShaderReadOnlyOptimal
		};

		vk::AttachmentDescription depthAttachment = {
			vk::AttachmentDescriptionFlags(),
			m_depthStencilImageFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal
		};

		vk::AttachmentReference colorAttachmentRef = {
			uint32_t(0),
			vk::ImageLayout::eColorAttachmentOptimal
		};

		vk::AttachmentReference depthAttachmentRef = {
			uint32_t(1),
			vk::ImageLayout::eDepthStencilAttachmentOptimal
		};

		vk::SubpassDescription subpass = {
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			1,
			&colorAttachmentRef,
			nullptr,
			&depthAttachmentRef,
			0,
			nullptr
		};

		vk::SubpassDependency dependency = {
			VK_SUBPASS_EXTERNAL,
			0,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::AccessFlags(),
			vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
			vk::DependencyFlags()
		};

		std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		vk::RenderPassCreateInfo createInfo = {
			vk::RenderPassCreateFlags(),
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			static_cast<uint32_t>(1),
			&subpass,
			static_cast<uint32_t>(1),
			&dependency
		};

		auto pDevice = pContext->getNativeDevice();
		m_pRenderPass = fd::createRenderPass(pDevice, createInfo);
	}

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::_createFramebuffer()
	{
		std::array<vk::ImageView, 2> attachments;
		if (m_pColorTexture != nullptr)
		{
			attachments = { *m_pColorTexture->_getImageView(), *m_pDepthStencilTexture->_getImageView() };
		}
		else
		{
			attachments = { *m_pSwapchainImageView, *m_pDepthStencilTexture->_getImageView() };
		}

		vk::FramebufferCreateInfo createInfo = {
			vk::FramebufferCreateFlags(),
			*m_pRenderPass,
			static_cast<uint32_t>(attachments.size()),
			attachments.data(),
			m_framebufferWidth,
			m_framebufferHeight,
			1u
		};

		auto pDevice = pContext->getNativeDevice();
		m_pFrameBuffer = fd::createFrameBuffer(pDevice, createInfo);
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

	template <SpaceType SPACE_TYPE>
	void Renderer<SPACE_TYPE>::_init()
	{
		_createRenderPass();
		_createFramebuffer();
	}

} //namespace kgs