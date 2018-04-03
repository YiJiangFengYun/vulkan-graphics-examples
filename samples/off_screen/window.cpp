#include "off_screen/window.hpp"

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
{
	_init();
	
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
{
	_init();
	
}

void Window::_init()
{
	ParentWindowType::_init();
	_createModel();
	_createTexture();
	_createMaterial();
	_initScene();
}

void Window::_initState()
{
	ParentWindowType::_initState();
	m_cameraPosition = { 0.0f, 1.0f, -6.0f };
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
	m_otherInfo.lightPos = vg::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::_createModel()
{
	const uint32_t layoutCount = 4u;
	sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_COLOR,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL			    				
	};
	sampleslib::AssimpScene::CreateInfo createInfo;
	createInfo.fileName = "models/chinesedragon.dae";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(0.0f, 1.5f, 0.0f);
	createInfo.scale = vg::Vector3(0.3f);
	m_assimpSceneModel.init(createInfo);

	createInfo.fileName = "models/plane.obj";
	createInfo.offset = vg::Vector3(0.0f, 0.0f, 2.0f);
	createInfo.scale = vg::Vector3(0.5f);

	m_assimpScenePlane.init(createInfo);
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
		fileName = "textures/darkmetal_bc3_unorm.ktx";
		format = vk::Format::eBc2UnormBlock;
	}
	else if (deviceFeatures.textureCompressionASTC_LDR)
	{
		fileName = "textures/darkmetal_astc_8x8_unorm.ktx";
		format = vk::Format::eAstc8x8UnormBlock;
	}
	else if (deviceFeatures.textureCompressionETC2)
	{
		fileName = "textures/darkmetal_etc2_unorm.ktx";
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
	m_pTexturePlane = std::shared_ptr<vg::Texture2D>(pTex);
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
	m_pTexturePlane->applyData(textureLayout, gliTex.data(), gliTex.size());

	m_pTexturePlane->setFilterMode(vg::FilterMode::TRILINEAR);
	m_pTexturePlane->setSamplerAddressMode(vg::SamplerAddressMode::REPEAT);

	auto pDevice = pApp->getDevice();
	auto pPhysicalDevice = pApp->getPhysicalDevice();
	if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
	{
		auto anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
		m_pTexturePlane->setAnisotropy(anisotropy);
	}
}

void Window::_createMaterial()
{
	{
	    auto & pShader = m_pShaderModel;
	    auto & pPass = m_pPassModel;
	    auto & pMaterial = m_pMaterialModel;
	    auto & pApp = vg::pApp;
	    //shader
	    pShader = std::shared_ptr<vg::Shader>(
	    	new vg::Shader("shaders/off_screen/phong.vert.spv", "shaders/off_screen/phong.frag.spv")
	    	// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
	    	);
	    //pass
	    pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	    vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW}
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
	    auto & pShader = m_pShaderPlane;
	    auto & pPass = m_pPassPlane;
	    auto & pMaterial = m_PMaterialPlane;
	    auto & pApp = vg::pApp;
	    //shader
	    pShader = std::shared_ptr<vg::Shader>(
	    	new vg::Shader("shaders/off_screen/mirror.vert.spv", "shaders/off_screen/mirror.frag.spv")
	    	// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
	    	);
	    //pass
	    pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	    vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	    	};
	    	vg::Pass::BuildInDataInfo buildInDataInfo;
	    	buildInDataInfo.componentCount = 3u;
	    	buildInDataInfo.pComponent = buildInDataCmps;
	    	pPass->setBuildInDataInfo(buildInDataInfo);
	    pPass->setCullMode(vg::CullModeFlagBits::NONE);
	    pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	    vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	    depthStencilState.depthTestEnable = VG_TRUE;
	    depthStencilState.depthWriteEnable = VG_TRUE;
	    depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	    pPass->setDepthStencilInfo(depthStencilState);
		pPass->setMainTexture(m_pTexturePlane.get());
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
	{
	    const auto &objects = m_assimpSceneModel.getObjects();
	    for (const auto &object : objects)
	    {
	    	object->setMaterial(m_pMaterialModel.get());
	        m_pScene->addVisualObject(object.get());		
	    }
	}
	{
	    const auto &objects = m_assimpScenePlane.getObjects();
	    for (const auto &object : objects)
	    {
	    	object->setMaterial(m_PMaterialPlane.get());
	        m_pScene->addVisualObject(object.get());		
	    }
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();

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