#include "pipelines/window.hpp"

#include <iostream>


Window::Window(uint32_t width
	, uint32_t height
	, const char* title
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(width
		, height
		, title
	    )
	, m_assimpScene()
	, m_pShaders()
	, m_pPasses()
	, m_pMaterials()
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
	, m_pShaders()
	, m_pPasses()
	, m_pMaterials()
{
	_init();
	_loadAssimpScene();	
	_createMaterial();
	_fillScene();
}

void Window::_init()
{
	m_cameraAspect = (float)m_width / 3.0f / (float)m_height;
	m_zoom = -10.5f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Vector3(glm::radians(25.0f), glm::radians(15.0f), glm::radians(0.0f));

	m_lightInfo.lightPos = vg::Vector4(0.0f, 2.0f, -1.0f, 1.0f);
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

void Window::_createMaterial()
{
	std::string vertShaderPaths[SCENE_COUNT] = {
		"shaders/pipelines/phong.vert.spv",
		"shaders/pipelines/toon.vert.spv",
		"shaders/pipelines/wireframe.vert.spv"
	};

	std::string fragShaderPaths[SCENE_COUNT] = {
		"shaders/pipelines/phong.frag.spv",
		"shaders/pipelines/toon.frag.spv",
		"shaders/pipelines/wireframe.frag.spv"
	};

	auto& pShaders = m_pShaders;
	auto& pPasses = m_pPasses;
	auto& pMaterials = m_pMaterials;
	auto& pApp = vg::pApp;
	for (uint32_t i = 0; i < SCENE_COUNT; ++i)
	{
		//shader
	    pShaders[i] = std::shared_ptr<vg::Shader>(
	    	new vg::Shader(vertShaderPaths[i], fragShaderPaths[i])
	    	);
	    //pass
	    pPasses[i] = std::shared_ptr<vg::Pass>(new vg::Pass(pShaders[i]));
		pPasses[i]->setCullMode(vg::CullModeFlagBits::BACK);
		pPasses[i]->setFrontFace(vg::FrontFaceType::CLOCKWISE);
		pPasses[i]->setViewport(fd::Viewport(static_cast<float>(i) / static_cast<float>(SCENE_COUNT),
		     0.0f,
		 	1.0f / static_cast<float>(SCENE_COUNT),
		 	1.0f));
		vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_TRUE;
	    depthStencilState.depthWriteEnable = VG_TRUE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	    pPasses[i]->setDepthStencilInfo(depthStencilState);
		if (i == 1u && pApp->getPhysicalDeviceFeatures().wideLines)
		{
			pPasses[i]->setLineWidth(2.0f);
		}

		if (i == 2u)
		{
			pPasses[i]->setPolygonMode(vg::PolygonMode::LINE);
		}

		pPasses[i]->setDataValue("light_info", m_lightInfo, 2u);
		pPasses[i]->apply();

	    //material
	    pMaterials[i] = std::shared_ptr<vg::Material>(new vg::Material());
	    pMaterials[i]->addPass(pPasses[i]);
		pMaterials[i]->setRenderPriority(0u);
	    pMaterials[i]->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	    pMaterials[i]->apply();
	}

}
void Window::_fillScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
	    m_pScene->addVisualObject(object);		
	}
}

void Window::_setMaterialToObjects(std::shared_ptr<vg::Material> pMaterial)
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
	    object->setMaterial(pMaterial);
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
	const auto &pApp = vg::pApp;
	if (pApp->getPhysicalDeviceFeatures().fillModeNonSolid == VK_FALSE)
	{
	    auto pos = ImGui::GetWindowPos();
	    auto size = ImGui::GetWindowSize();
	    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	    ImGui::SetNextWindowSize(ImVec2(0, 0));
	    ImGui::Begin("Extra Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	    ImGui::TextUnformatted("Non solid fill modes not supported!");
	    ImGui::End();
	}
}

void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

    // _setMaterialToObjects(m_pMaterials[0]);
	// ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);
	const auto &pApp = vg::pApp;
	for (uint32_t i = 0u; i < SCENE_COUNT; ++i)
	{
		_setMaterialToObjects(m_pMaterials[i]);
		if (i != 2u || pApp->getPhysicalDeviceFeatures().fillModeNonSolid)
		{
			if (i != 2u)
			{
			vg::Renderer::SceneAndCamera sceneAndCamera;
			sceneAndCamera.pScene = m_pScene.get();
			sceneAndCamera.pCamera = m_pCamera.get();
			auto addedInfo = info;
			addedInfo.sceneAndCameraCount = 1u;
			addedInfo.pSceneAndCamera = &sceneAndCamera;
               pRenderer->render(addedInfo, resultInfo);
			}
			else
			{
				ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);
			}
		}
		else if (i == 2u)
		{
			ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);
		}
	}
	
	
	m_lastDrawCount += resultInfo.drawCount;	
	
}