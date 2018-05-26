#include "outline/window.hpp"

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
	, m_assimpScene()
	, m_pMaterial()
	, m_outlineWidth(1.0f)
	, m_outlineColor(vg::Color(1.0f))
{
	_init();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_assimpScene()
	, m_pMaterial()
	, m_outlineWidth(1.0f)
	, m_outlineColor(vg::Color(1.0f))
{
	_init();
}

void Window::_init()
{
	ParentWindowType::_init();
	_createModel();
	_createMaterial();
	_initScene();
}

void Window::_initState()
{
	ParentWindowType::_initState();
	m_cameraZoom = -10.0f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	m_outlineWidth = 0.05f;
	m_outlineColor = vg::Color(1.0f);
	m_otherInfo.lightPos = vg::Vector4(0.0f, 2.0f, -3.0f, 1.0);
}

void Window::_createModel()
{
	const uint32_t layoutCount = 4u;
	sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,		
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
	};
	sampleslib::AssimpScene::CreateInfo createInfo;
	createInfo.fileName = "models/venus.fbx";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(0.0f, -3.0f, 0.0f);
	createInfo.scale = vg::Vector3(0.3f, 0.3f, 0.3f);
	m_assimpScene.init(createInfo);
}

void Window::_createMaterial()
{
	auto & pApp = vg::pApp;
	{
		//material
		auto & pMaterial = m_pMaterial;
		pMaterial = std::shared_ptr<vge::MaterialOutline>(new vge::MaterialOutline());
		pMaterial->setRenderPriority(0u);
		pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

		pMaterial->setOutlineWidth(m_outlineWidth);
		pMaterial->setOutlineColor(m_outlineColor);

		//main pass.
		{
			auto pShader = pMaterial->getMainShader();
	        auto pPass = pMaterial->getMainPass();
	        
	        //shader
		    pShader->load("shaders/outline/toon.vert.spv",
		    	"shaders/outline/toon.frag.spv");
	        //pass
	        vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
	        		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	        		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
	        	};
	        vg::Pass::BuildInDataInfo buildInDataInfo;
	        buildInDataInfo.componentCount = 2u;
	        buildInDataInfo.pComponent = buildInDataCmps;
	        pPass->setBuildInDataInfo(buildInDataInfo);
	        pPass->setCullMode(vk::CullModeFlagBits::eNone);
	        pPass->setFrontFace(vk::FrontFace::eClockwise);
	        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	        depthStencilState.depthTestEnable = VG_TRUE;
	        depthStencilState.depthWriteEnable = VG_TRUE;
	        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
			depthStencilState.stencilTestEnable = VK_TRUE;
			depthStencilState.back.compareOp = vk::CompareOp::eAlways;
		    depthStencilState.back.failOp = vk::StencilOp::eReplace;
		    depthStencilState.back.depthFailOp = vk::StencilOp::eReplace;
		    depthStencilState.back.passOp = vk::StencilOp::eReplace;
		    depthStencilState.back.compareMask = 0xff;
		    depthStencilState.back.writeMask = 0xff;
		    depthStencilState.back.reference = 1;
		    depthStencilState.front = depthStencilState.back;
	        pPass->setDepthStencilInfo(depthStencilState);
			vg::Pass::VertexInputFilterInfo vertexInputFilter;
			vertexInputFilter.filterEnable = VG_TRUE;
			std::vector<uint32_t> filterLocations = {
				0,
				1,
				2,
			};
			vertexInputFilter.locationCount = static_cast<uint32_t>(filterLocations.size());
			vertexInputFilter.pLocations = filterLocations.data();
			pPass->setVertexInputFilterInfo(vertexInputFilter);
			vg::PassDataInfo otherDataInfo = {
				VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY,
				vk::ShaderStageFlagBits::eVertex,
			};
	        pPass->addData("other_info", otherDataInfo, m_otherInfo);
	        pPass->apply();
	        
		}

		//outline pass.
		{
	        auto pPass = pMaterial->getPassOutline();
			auto pShader = pPass->getShader();
			
	        //shader
		    pShader->load("shaders/outline/outline.vert.spv",
		    	"shaders/outline/outline.frag.spv");
	        //pass
	        vg::Pass::BuildInDataInfo::Component buildInDataCmps[1] = {
	        		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	        	};
	        vg::Pass::BuildInDataInfo buildInDataInfo;
	        buildInDataInfo.componentCount = 1u;
	        buildInDataInfo.pComponent = buildInDataCmps;
	        pPass->setBuildInDataInfo(buildInDataInfo);
	        pPass->setCullMode(vk::CullModeFlagBits::eNone);
	        pPass->setFrontFace(vk::FrontFace::eClockwise);
	        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	        depthStencilState.depthTestEnable = VG_TRUE;
	        depthStencilState.depthWriteEnable = VG_TRUE;
	        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
			depthStencilState.stencilTestEnable = VK_TRUE;
		    depthStencilState.back.compareOp = vk::CompareOp::eNotEqual;
		    depthStencilState.back.failOp = vk::StencilOp::eKeep;
		    depthStencilState.back.depthFailOp = vk::StencilOp::eKeep;
		    depthStencilState.back.passOp = vk::StencilOp::eReplace;
			depthStencilState.back.compareMask = 0xff;
		    depthStencilState.back.writeMask = 0xff;
		    depthStencilState.back.reference = 1;
		    depthStencilState.front = depthStencilState.back;
	        pPass->setDepthStencilInfo(depthStencilState);

			vg::Pass::VertexInputFilterInfo vertexInputFilter;
			vertexInputFilter.filterEnable = VG_TRUE;
			std::vector<uint32_t> filterLocations = {
				0,
				2,
			};
			vertexInputFilter.locationCount = static_cast<uint32_t>(filterLocations.size());
			vertexInputFilter.pLocations = filterLocations.data();
			pPass->setVertexInputFilterInfo(vertexInputFilter);
	        pPass->apply();
		}

        pMaterial->apply();
	}
}

void Window::_initScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
		object->setMaterialCount(1u);
		object->setMaterial(m_pMaterial.get());
	    m_pScene->addVisualObject(object.get());		
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();

#ifdef USE_IMGUI_BIND
	auto pos = m_lastWinPos;
	auto size = m_lastWinSize;
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::InputFloat("Outline width", &m_outlineWidth, 0.05f, 2)) {
		auto pMaterial = m_pMaterial;
		pMaterial->setOutlineWidth(m_outlineWidth);
		pMaterial->apply();
	}
	if (ImGui::ColorPicker4("Outline color", reinterpret_cast<float *>(&m_outlineColor))) {
		auto pMaterial = m_pMaterial;
		pMaterial->setOutlineColor(m_outlineColor);
		pMaterial->apply();
	}
	ImGui::End();
#endif //USE_IMGUI_BIND
}