#include "push_constants/window.hpp"

#include <iostream>
#include <gli/gli.hpp>

Window::Window(uint32_t width
	, uint32_t height
	, const char* title
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(width
		, height
		, title
	    )
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
	
}

void Window::_init()
{
	ParentWindowType::_init();
	_loadAssimpScene();
	_createMaterial();
	_fillScene();
}

void Window::_initState()
{
	ParentWindowType::_initState();
	m_cameraZoom = -20.0f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	m_timerSpeedFactor *= 0.5f;
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
	createInfo.fileName = "models/samplescene.dae";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.scale = vg::Vector3(0.35f, 0.35f, 0.35f);
	createInfo.offset = vg::Vector3(0.0f, 0.0f, -0.0f);
	m_assimpScene.init(createInfo);
}

void Window::_createMaterial()
{
	auto & pShader = m_pShader;
	auto & pPass = m_pPass;
	auto & pMaterial = m_pMaterial;
	auto & pApp = vg::pApp;
	//shader
	pShader = std::shared_ptr<vg::Shader>(
		new vg::Shader("shaders/push_constants/lights.vert.spv", "shaders/push_constants/lights.frag.spv")
		// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
		);
	//pass
	pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
			{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
			{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD}
		};
		vg::Pass::BuildInDataInfo buildInDataInfo;
		buildInDataInfo.componentCount = 2u;
		buildInDataInfo.pComponent = buildInDataCmps;
	pPass->setBuildInDataInfo(buildInDataInfo);
	pPass->setCullMode(vg::CullModeFlagBits::BACK);
	pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	pPass->setDepthStencilInfo(depthStencilState);
	pPass->setPushConstantRange("push_constants", vk::ShaderStageFlagBits::eVertex, 0u, static_cast<uint32_t>(sizeof(m_pushConstants)));
	pPass->apply();
	//material
	pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	pMaterial->addPass(pPass.get());
	pMaterial->setRenderPriority(0u);
	pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	pMaterial->apply();

}

void Window::_fillScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
		object->setMaterial(m_pMaterial.get());
	    m_pScene->addVisualObject(object.get());		
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();

	// Update light positions
	// w component = light radius scale
#define r 7.5f
#define sin_t sin(glm::radians(m_passedTime * 360))
#define cos_t cos(glm::radians(m_passedTime * 360))
#define x 20.0f
#define y -20.0f
	m_pushConstants[0] = glm::vec4(x + r * 1.1 * sin_t,   y, r * 1.1 * cos_t, 1.0f);
	m_pushConstants[1] = glm::vec4(x + -r * sin_t,        y, -r * cos_t, 1.0f);
	m_pushConstants[2] = glm::vec4(x + r * 0.85f * sin_t, y, -sin_t * 2.5f, 1.5f);
	m_pushConstants[3] = glm::vec4(x + 0.0f,              y, r * 1.25f * cos_t, 1.5f);
	m_pushConstants[4] = glm::vec4(x + r * 2.25f * cos_t, y, 0.0f, 1.25f);
	m_pushConstants[5] = glm::vec4(x + r * 2.5f * cos_t,  y, r * 2.5f * sin_t, 1.25f);
#undef r
#undef x
#undef y
#undef sin_t
#undef cos_t
    
	m_pPass->setPushConstantUpdate("push_constants", m_pushConstants.data(), 
	    static_cast<uint32_t>(sizeof(m_pushConstants)), vk::ShaderStageFlagBits::eVertex, 0u);

	// auto pos = m_lastWinPos;
	// auto size = m_lastWinSize;
	// ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	// ImGui::SetNextWindowSize(ImVec2(0, 0));
	// ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	// ImGui::End();
}

void Window::_renderWithRenderer(vg::Renderer *pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

	ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);	
}