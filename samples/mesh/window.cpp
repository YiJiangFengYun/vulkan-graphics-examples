#include "mesh/window.hpp"

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
	, m_wireFrame(false)
	, m_assimpScene()
	, m_pTexture()
	, m_pShaderSolid()
	, m_pPassSolid()
	, m_pMaterialSolid()
	, m_pShaderWireframe()
	, m_pPassWireframe()
	, m_pMaterialWireframe()
{
	_init();
	_createModel();
	_createTexture();
	_createMaterial();
	_initScene();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_wireFrame(false)
	, m_assimpScene()
	, m_pTexture()
	, m_pShaderSolid()
	, m_pPassSolid()
	, m_pMaterialSolid()
	, m_pShaderWireframe()
	, m_pPassWireframe()
	, m_pMaterialWireframe()
{
	_init();
	_createModel();
	_createTexture();
	_createMaterial();
	_initScene();
}

void Window::_init()
{
	m_zoom = -15.0f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	m_otherInfo.lightPos = vg::Vector4(25.0f, -5.0f, 5.0f, 1.0f);
}

void Window::_createModel()
{
	const uint32_t layoutCount = 4u;
	sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR	    				
	};
	sampleslib::AssimpScene::CreateInfo createInfo;
	createInfo.fileName = "models/voyager/voyager.dae";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
	m_assimpScene.init(createInfo);
}

void Window::_createTexture()
{
	//load texture
	auto &pApp = vg::pApp;
	auto deviceFeatures = pApp->getPhysicalDeviceFeatures();
	std::string fileName;
	vk::Format format;
	if (deviceFeatures.textureCompressionBC) 
	{
		fileName = "models/voyager/voyager_bc3_unorm.ktx";
		format = vk::Format::eBc2UnormBlock;
	}
	else if (deviceFeatures.textureCompressionASTC_LDR)
	{
		fileName = "models/voyager/voyager_astc_8x8_unorm.ktx";
		format = vk::Format::eAstc8x8UnormBlock;
	}
	else if (deviceFeatures.textureCompressionETC2)
	{
		fileName = "models/voyager/voyager_etc2_unorm.ktx";
		format = vk::Format::eEtc2R8G8B8UnormBlock;
	}
	else
	{
		throw std::runtime_error("Device does not support any compressed texture format!");
	}

	gli::texture2d gliTex(gli::load(fileName));
	if (gliTex.empty()) {
		throw std::runtime_error("The texture do't exist! path: " + fileName);
	}

	auto pTex = new vg::Texture2D(format, VG_TRUE,
		gliTex[0].extent().x,
		gliTex[0].extent().y
	);
	m_pTexture = std::shared_ptr<vg::Texture2D>(pTex);
	uint32_t mipLevels = static_cast<uint32_t>(gliTex.levels());
	uint32_t count = mipLevels;
	vg::TextureDataInfo textureLayout;
	std::vector<vg::TextureDataInfo::Component> components(count);
	for (uint32_t level = 0; level < mipLevels; ++level) {
		uint32_t index = level;
		components[index].mipLevel = level;
	    components[index].baseArrayLayer = 0u;
	    components[index].layerCount = 1u;
	    components[index].size = gliTex[level].size();
	    components[index].hasImageExtent = VG_TRUE;
	    components[index].width = gliTex[level].extent().x;
	    components[index].height = gliTex[level].extent().y;
	    components[index].depth = 1u;
	}
	textureLayout.componentCount = components.size();
	textureLayout.pComponent = components.data();
	m_pTexture->applyData(textureLayout, gliTex.data(), gliTex.size());

	m_pTexture->setFilterMode(vg::FilterMode::TRILINEAR);
	m_pTexture->setSamplerAddressMode(vg::SamplerAddressMode::REPEAT);

	auto pDevice = pApp->getDevice();
	auto pPhysicalDevice = pApp->getPhysicalDevice();
	if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
	{
		auto anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
		m_pTexture->setAnisotropy(anisotropy);
	}
}

void Window::_createMaterial()
{
	{
	    auto & pShader = m_pShaderSolid;
	    auto & pPass = m_pPassSolid;
	    auto & pMaterial = m_pMaterialSolid;
	    auto & pApp = vg::pApp;
	    //shader
	    pShader = std::shared_ptr<vg::Shader>(
	    	new vg::Shader("shaders/mesh/mesh.vert.spv", "shaders/mesh/mesh.frag.spv")
	    	// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
	    	);
	    //pass
	    pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	    vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	    		{vg::Pass::BuildInDataType::MAIN_CLOLOR},
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD}
	    	};
	    	vg::Pass::BuildInDataInfo buildInDataInfo;
	    	buildInDataInfo.componentCount = 3u;
	    	buildInDataInfo.pComponent = buildInDataCmps;
	    	pPass->setBuildInDataInfo(buildInDataInfo);
	    pPass->setCullMode(vg::CullModeFlagBits::BACK);
	    pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_TRUE;
	    depthStencilState.depthWriteEnable = VG_TRUE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	    pPass->setDepthStencilInfo(depthStencilState);
	    pPass->setMainTexture(m_pTexture.get());
	    pPass->setDataValue("other_info", m_otherInfo, 2u);
	    pPass->apply();
	    //material
	    pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	    pMaterial->addPass(pPass.get());
	    pMaterial->setRenderPriority(0u);
	    pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	    pMaterial->apply();
	}

	{
	    auto & pShader = m_pShaderWireframe;
	    auto & pPass = m_pPassWireframe;
	    auto & pMaterial = m_pMaterialWireframe;
	    auto & pApp = vg::pApp;
	    //shader
	    pShader = std::shared_ptr<vg::Shader>(
	    	new vg::Shader("shaders/mesh/mesh.vert.spv", "shaders/mesh/mesh.frag.spv")
	    	// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
	    	);
	    //pass
	    pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	    vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	    		{vg::Pass::BuildInDataType::MAIN_CLOLOR},
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD}
	    	};
	    	vg::Pass::BuildInDataInfo buildInDataInfo;
	    	buildInDataInfo.componentCount = 3u;
	    	buildInDataInfo.pComponent = buildInDataCmps;
	    	pPass->setBuildInDataInfo(buildInDataInfo);
	    pPass->setCullMode(vg::CullModeFlagBits::BACK);
	    pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);

		if (pApp->getPhysicalDeviceFeatures().fillModeNonSolid) {
			pPass->setPolygonMode(vg::PolygonMode::LINE);
			pPass->setLineWidth(1.0f);
		}

	    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_TRUE;
	    depthStencilState.depthWriteEnable = VG_TRUE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	    pPass->setDepthStencilInfo(depthStencilState);
	    pPass->setMainTexture(m_pTexture.get());
	    pPass->setDataValue("other_info", m_otherInfo, 2u);
	    pPass->apply();
	    //material
	    pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	    pMaterial->addPass(pPass.get());
	    pMaterial->setRenderPriority(0u);
	    pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	    pMaterial->apply();
	}

}

void Window::_initScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
		object->setMaterial(m_pMaterialSolid.get());
	    m_pScene->addVisualObject(object.get());		
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();

	auto pos = m_lastWinPos;
	auto size = m_lastWinSize;
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Checkbox("wireframe", &m_wireFrame)) {
		const auto &objects = m_assimpScene.getObjects();
	    for (const auto &object : objects)
	    {
			if (m_wireFrame) 
			{
				object->setMaterial(m_pMaterialWireframe.get());
			} 
			else 
			{
				object->setMaterial(m_pMaterialSolid.get());
			}
	    	
	        m_pScene->addVisualObject(object.get());		
	    }
	}
	ImGui::End();
}

void Window::_renderWithRenderer(vg::Renderer *pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

	ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);	
}