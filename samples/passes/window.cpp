#include "passes/window.hpp"

#include <iostream>


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
	// _createTexture();
	_createMaterial();
	_initScene();
}

void Window::_initState()
{
	ParentWindowType::_initState();
	m_cameraZoom = -0.0f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
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
	createInfo.fileName = "models/samplebuilding.dae";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
	m_assimpScene.init(createInfo);
}

// void Window::_createTexture()
// {
// 	//load texture
// 	auto &pApp = vg::pApp;
// 	auto deviceFeatures = pApp->getPhysicalDeviceFeatures();
// 	std::string fileName;
// 	vk::Format format;
// 	if (deviceFeatures.textureCompressionBC) 
// 	{
// 		fileName = "models/voyager/voyager_bc3_unorm.ktx";
// 		format = vk::Format::eBc2UnormBlock;
// 	}
// 	else if (deviceFeatures.textureCompressionASTC_LDR)
// 	{
// 		fileName = "models/voyager/voyager_astc_8x8_unorm.ktx";
// 		format = vk::Format::eAstc8x8UnormBlock;
// 	}
// 	else if (deviceFeatures.textureCompressionETC2)
// 	{
// 		fileName = "models/voyager/voyager_etc2_unorm.ktx";
// 		format = vk::Format::eEtc2R8G8B8UnormBlock;
// 	}
// 	else
// 	{
// 		throw std::runtime_error("Device does not support any compressed texture format!");
// 	}

// 	gli::texture2d gliTex(gli::load(fileName));
// 	if (gliTex.empty()) {
// 		throw std::runtime_error("The texture do't exist! path: " + fileName);
// 	}

// 	auto pTex = new vg::Texture2D(format, VG_TRUE,
// 		gliTex[0].extent().x,
// 		gliTex[0].extent().y
// 	);
// 	m_pTexture = std::shared_ptr<vg::Texture2D>(pTex);
// 	uint32_t mipLevels = static_cast<uint32_t>(gliTex.levels());
// 	uint32_t count = mipLevels;
// 	vg::TextureDataInfo textureLayout;
// 	std::vector<vg::TextureDataInfo::Component> components(count);
// 	for (uint32_t level = 0; level < mipLevels; ++level) {
// 		uint32_t index = level;
// 		components[index].mipLevel = level;
// 	    components[index].baseArrayLayer = 0u;
// 	    components[index].layerCount = 1u;
// 	    components[index].size = gliTex[level].size();
// 	    components[index].hasImageExtent = VG_TRUE;
// 	    components[index].width = gliTex[level].extent().x;
// 	    components[index].height = gliTex[level].extent().y;
// 	    components[index].depth = 1u;
// 	}
// 	textureLayout.componentCount = components.size();
// 	textureLayout.pComponent = components.data();
// 	m_pTexture->applyData(textureLayout, gliTex.data(), gliTex.size());

// 	auto pDevice = pApp->getDevice();
// 	auto pPhysicalDevice = pApp->getPhysicalDevice();
// 	vk::Bool32 enableAnisotropy = VK_FALSE;
// 	float anisotropy = 0.0f;
// 	if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
// 	{
// 		enableAnisotropy = VK_TRUE;
// 		anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
// 	}

// 	vg::Texture::SamplerCreateInfo info = {
// 		vk::SamplerCreateFlags(),
// 		vk::Filter::eLinear,
// 		vk::Filter::eLinear,
// 		vk::SamplerMipmapMode::eLinear,
// 		vk::SamplerAddressMode::eRepeat,
// 		vk::SamplerAddressMode::eRepeat,
// 		vk::SamplerAddressMode::eRepeat,
// 		0.0f,
// 		enableAnisotropy,
// 		anisotropy,
// 		0.0f,
// 		(float)(m_pTexture->getImage()->getInfo().mipLevels),
// 	};
// 	m_pTexture->createSampler("other_sampler", info);
// }

void Window::_createMaterial()
{
	{
		//material
	    m_pMaterialOfScene = std::shared_ptr<MaterialDeferred>(new MaterialDeferred());
	    m_pMaterialOfScene->setRenderPriority(0u);
	    m_pMaterialOfScene->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		{
	        auto pPass = m_pMaterialOfScene->getPassDeferred();
			auto pShader = pPass->getShader();

			//shader
	        pShader->load("shaders/passes/gbuffer.vert.spv", 
	            "shaders/passes/gbuffer.frag.spv");

			//pass
			pPass->setPolygonMode(vg::PolygonMode::FILL);
	        pPass->setCullMode(vg::CullModeFlagBits::BACK);
	        pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);
			vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
	    		{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD}
	    	};
	        vg::Pass::BuildInDataInfo buildInDataInfo;
	        buildInDataInfo.componentCount = 2u;
	        buildInDataInfo.pComponent = buildInDataCmps;
	        pPass->setBuildInDataInfo(buildInDataInfo);
			vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	        depthStencilState.depthTestEnable = VG_TRUE;
	        depthStencilState.depthWriteEnable = VG_TRUE;
	        depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	        pPass->setDepthStencilInfo(depthStencilState);
		}
		{
			auto pPass = m_pMaterialOfScene->getPassComposition();
			auto pShader = pPass->getShader();
			//shader
	        pShader->load("shaders/passes/composition.vert.spv", 
	            "shaders/passes/composition.frag.spv");

		    vk::SpecializationMapEntry specializationMapEntriy;
    
		    specializationMapEntriy.constantID = 0;
		    specializationMapEntriy.size = sizeof(uint32_t);
		    specializationMapEntriy.offset = 0;

			uint32_t specializationData = NUM_LIGHTS;
    
		    // Prepare specialization info block for the shader stage
		    vk::SpecializationInfo specializationInfo{};
		    specializationInfo.mapEntryCount = 1u;
		    specializationInfo.pMapEntries = &specializationMapEntriy;
		    specializationInfo.dataSize = sizeof(specializationData);			
		    specializationInfo.pData = &specializationData;
    
		    pPass->setSpecializationData(vg::ShaderStageFlagBits::FRAGMENT, specializationInfo);
    
			//pass
			pPass->setDataValue("other_info", m_otherInfo, 3u);
		}
		{
			auto pShader = m_pMaterialOfScene->getMainShader();
	        auto pPass = m_pMaterialOfScene->getMainPass();
	        //shader
	        pShader->load("shaders/passes/to_trunk.vert.spv", 
	            "shaders/passes/to_trunk.frag.spv");
	        //pass
		}
    
	    m_pMaterialOfScene->apply();
	}
	
}

void Window::_initScene()
{
	const auto &objects = m_assimpScene.getObjects();
	for (const auto &object : objects)
	{
		object->setMaterial(m_pMaterialOfScene.get());
	    m_pScene->addVisualObject(object.get());		
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
}