#include "triangle/window.hpp"

namespace triangle
{
	Window::Window(uint32_t width
		, uint32_t height
		, const char* title
	)
		: vgf::Window(width
			, height
			, title
		    )
		, m_zoom(0.0f)
		, m_rotation(0.0f)
	{
		_initZoom();
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
	}

	Window::Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	)
		: vgf::Window(pWindow
			, pSurface
		    )
		, m_zoom(0.0f)
		, m_rotation(0.0f)
	{
		_initZoom();
		_loadModel();
		_createMesh();
		_createMaterial();
		_createModel();
		_createCamera();
		_createScene();
	}

	void Window::_initZoom()
	{
		m_zoom = -2.5f;
	}

	void Window::_loadModel()
	{
		m_tempPositions = { vg::Vector3(1.0f, 1.0f, 0.0f)
		, vg::Vector3(-1.0f, 1.0f, 0.0f)
		, vg::Vector3(0.0f, -1.0f, 0.0f)
		};

		m_tempColors = { vg::Color32(255, 0, 0, 255)
		, vg::Color32(0, 255, 0, 255)
		, vg::Color32(0, 0, 255, 255)
		};

		m_tempIndices = {
			0, 1, 2
		};
	}

	void Window::_createMesh()
	{
		m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
		m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
		m_pMesh->setPositions(m_tempPositions);
		m_pMesh->setColors(m_tempColors);
		m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
		m_pMesh->apply(VG_TRUE);
	}

	void Window::_createMaterial()
	{
		//shader
		m_pShader = std::shared_ptr<vg::Shader>(
			new vg::Shader("shaders/triangle/triangle.vert.spv", "shaders/triangle/triangle.frag.spv")
			);

		//pass
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader));
		m_pPass->setMainColor(vg::Color(1.0f, 1.0f, 1.0f, 1.0f));
		m_pPass->setPolygonMode(vg::PolygonMode::FILL);
		m_pPass->setCullMode(vg::CullModeFlagBits::NONE);
		m_pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);

		vk::PipelineColorBlendAttachmentState attachmentState[1] = {};
		attachmentState[0].colorWriteMask = vk::ColorComponentFlagBits::eR
			| vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB
			| vk::ColorComponentFlagBits::eA;
		attachmentState[0].blendEnable = VG_FALSE;
		vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = attachmentState;
		m_pPass->setColorBlendInfo(colorBlendState);

		vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.depthTestEnable = VG_TRUE;
		depthStencilState.depthWriteEnable = VG_TRUE;
		depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
		m_pPass->setDepthStencilInfo(depthStencilState);

		//material
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		m_pMaterial->apply();
	}

	void Window::_createModel()
	{
		m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
		m_pModel->setMesh(m_pMesh);
		m_pModel->setMaterial(m_pMaterial);
	}

	void Window::_createCamera()
	{
		m_pCamera = std::shared_ptr<vg::Camera3>(new vg::Camera3());
		_updateCamera();
	}

	void Window::_createScene()
	{
		m_pScene = std::shared_ptr<vg::Scene3>(new vg::Scene3());
		m_pScene->addCamera(m_pCamera);
		m_pScene->addVisualObject(m_pModel);
	}

	void Window::_updateCamera()
	{
		m_pCamera->updateProj(glm::radians(60.0f), (float)m_width / (float)m_height, 0.1f, 256.0f);
		auto &transform = m_pCamera->getTransform();

		transform->setLocalPosition(glm::vec3(0.0f, 0.0f, m_zoom));
		transform->setLocalRotation(m_rotation);
		m_pCamera->apply();		
	}	

	void Window::_onResize()
	{
		_updateCamera();
	}

	void Window::_onPreReCreateSwapchain()
	{

	}

	void Window::_onPostReCreateSwapchain()
	{
	}

	void Window::_onPreUpdate()
	{

	}

	void Window::_onUpdate()
	{
		_updateCamera();
	}

	void Window::_onPostUpdate()
	{

	}

	void Window::_onPreRender()
	{

	}

	void Window::_onPostRender()
	{

	}

	void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
	{
		vg::Renderer::SceneAndCamera sceneAndCamera;
		sceneAndCamera.pScene = m_pScene.get();
		sceneAndCamera.pCamera = m_pCamera.get();
		auto myInfo = info;
		myInfo.sceneAndCameraCount = 1u;
		myInfo.pSceneAndCamera = &sceneAndCamera;
		pRenderer->render(myInfo, resultInfo);
	}
}