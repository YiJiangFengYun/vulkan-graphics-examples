#include "texture/window.hpp"

#include <iostream>

Window::OtherInfo::OtherInfo(vg::Vector3 viewPos, float lodBias)
    : viewPos(viewPos)
	, lodBias(lodBias)
{

}

Window::Window(uint32_t width
	, uint32_t height
	, const char* title
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(width
		, height
		, title
	    )
	, m_assimpScene()
	, m_pTexture()
	, m_pShader()
	, m_pPasse()
	, m_pMaterial()
{
	_init();
	_loadAssimpScene();
	_createMaterial();
	_fillScene();
}
Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_assimpScene()
	, m_pTexture()
	, m_pShader()
	, m_pPasse()
	, m_pMaterial()
{
	_init();
	_loadAssimpScene();	
	_createMaterial();
	_fillScene();
}

void Window::_init()
{
	m_zoom = -2.5f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Vector3(glm::radians(0.0f), glm::radians(15.0f), glm::radians(0.0f));
}

void Window::_loadAssimpScene()
{
	const uint32_t layoutCount = 4u;
	sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR	    				
	};
	sampleslib::AssimpScene::CreateInfo createInfo;
	createInfo.fileName = "models/treasure_smooth.dae";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(3.0f, -6.0f, -1.0f);
	m_assimpScene.init(createInfo);
}

void Window::_createTexture()
{

	// m_pTexture = std::shared_ptr<vg::Texture2D>(new vg::Texture2D());
}

void Window::_createMaterial()
{

	auto & pShader = m_pShader;
	auto & pPass = m_pPasse;
	auto & pMaterial = m_pMaterial;
	auto & pApp = vg::pApp;
	//shader
	pShader = std::shared_ptr<vg::Shader>(
		new vg::Shader("shaders/texture/texture.vert.spv", "shaders/texture/texture.frag.spv")
		);
	//pass
	pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader));
	pPass->setCullMode(vg::CullModeFlagBits::BACK);
	pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	pPass->setDepthStencilInfo(depthStencilState);
	pPass->setDataValue("other_info", m_otherInfo, 2u);
	pPass->apply();
	//material
	pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	pMaterial->addPass(pPass);
	pMaterial->setRenderPriority(0u);
	pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	pMaterial->apply();

}
void Window::_fillScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
	    m_pScene->addVisualObject(object);		
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
}

void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

	ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);	
}