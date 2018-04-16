#include "cubemaps/window.hpp"

#include <iostream>
#include <gli/gli.hpp>

Window::OtherInfo::OtherInfo()
    : lodBias(0.0f)
{

}

Window::OtherInfo::OtherInfo(float lodBias)
    : lodBias(lodBias)
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
	, m_displaySkybox(true)
	, m_skyBoxObject()
	, m_objects()
	, m_objectIndex(0)
	, m_currIndex(-1)
	, m_arrObjectNames()
	, m_pCubeMapTex()
	, m_pMaterialSkybox()
	, m_pMaterialReflect()
{
	_init();
	
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_displaySkybox(true)
	, m_skyBoxObject()
	, m_objects()
	, m_objectIndex(0)
	, m_currIndex(-1)
	, m_arrObjectNames()
	, m_pCubeMapTex()
	, m_pMaterialSkybox()
	, m_pMaterialReflect()
{
	_init();
	
}

void Window::_init()
{
	ParentWindowType::_init();
	_createTexture();
	_createMaterial();
	_createModel();
	_initScene();
}

void Window::_initState()
{
	ParentWindowType::_initState();
	m_cameraZoom = -4.0f;
	m_rotationSpeed = 0.05f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_cameraRotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
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
		fileName = "textures/cubemap_yokohama_bc3_unorm.ktx";
		format = vk::Format::eBc2UnormBlock;
	}
	else if (deviceFeatures.textureCompressionASTC_LDR)
	{
		fileName = "textures/cubemap_yokohama_astc_8x8_unorm.ktx";
		format = vk::Format::eAstc8x8UnormBlock;
	}
	else if (deviceFeatures.textureCompressionETC2)
	{
		fileName = "textures/cubemap_yokohama_etc2_unorm.ktx";
		format = vk::Format::eEtc2R8G8B8UnormBlock;
	}
	else
	{
		throw std::runtime_error("Device does not support any compressed texture format!");
	}

	gli::texture_cube gliTex2D(gli::load(fileName));
	if (gliTex2D.empty()) {
		throw std::runtime_error("The texture do't exist! path: " + fileName);
	}

	auto pTex = new vg::TextureCube(format, VG_TRUE, 
		gliTex2D[0].extent().x, 
		gliTex2D[0].extent().y
	);
	m_pCubeMapTex = std::shared_ptr<vg::TextureCube>(pTex);
	uint32_t mipLevels = static_cast<uint32_t>(gliTex2D.levels());
	uint32_t faces = static_cast<uint32_t>(gliTex2D.faces());
	uint32_t count = mipLevels * faces;
	vg::TextureDataInfo textureLayout;
	std::vector<vg::TextureDataInfo::Component> components(count);
	for (uint32_t face = 0; face < faces; ++face) {
		for (uint32_t level = 0; level < mipLevels; ++level) {
			uint32_t index = face * mipLevels + level;
			components[index].mipLevel = level;
		    components[index].baseArrayLayer = face;
		    components[index].layerCount = 1u;
		    components[index].size = gliTex2D[face][level].size();
		    components[index].hasImageExtent = VG_TRUE;
		    components[index].width = gliTex2D[face][level].extent().x;
		    components[index].height = gliTex2D[face][level].extent().y;
		    components[index].depth = 1u;
		}
	}
	textureLayout.componentCount = components.size();
	textureLayout.pComponent = components.data();
	m_pCubeMapTex->applyData(textureLayout, gliTex2D.data(), gliTex2D.size());

	auto pDevice = pApp->getDevice();
	auto pPhysicalDevice = pApp->getPhysicalDevice();
	vk::Bool32 enableAnisotropy = VK_FALSE;
	float anisotropy = 0.0f;
	if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
	{
		enableAnisotropy = VK_TRUE;
		anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
	}

	vg::Texture::SamplerCreateInfo info = {
		vk::SamplerCreateFlags(),
		vk::Filter::eLinear,
		vk::Filter::eLinear,
		vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		0.0f,
		enableAnisotropy,
		anisotropy,
		0.0f,
		(float)(m_pCubeMapTex->getImage()->getInfo().mipLevels),
	};
	m_pCubeMapTex->createSampler("other_sampler", info);
}

void Window::_createMaterial()
{
	auto & pApp = vg::pApp;
    {
		//material
		auto & pMaterial = m_pMaterialSkybox;
		pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		pMaterial->setRenderPriority(0u);
		pMaterial->setRenderQueueType(vg::MaterialShowType::BACKGROUND);
		auto pShader = pMaterial->getMainShader();
	    auto pPass = pMaterial->getMainPass();
	    
	    //shader
	    pShader->load("shaders/cubemaps/skybox.vert.spv",
			"shaders/cubemaps/skybox.frag.spv");
	    //pass
		vg::Pass::BuildInDataInfo::Component buildInDataCmps[3] = {
			{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD},
			{vg::Pass::BuildInDataType::MATRIX_VIEW},
			{vg::Pass::BuildInDataType::MATRIX_PROJECTION}
		};
		vg::Pass::BuildInDataInfo buildInDataInfo;
		buildInDataInfo.componentCount = 3u;
		buildInDataInfo.pComponent = buildInDataCmps;
		pPass->setBuildInDataInfo(buildInDataInfo);
	    pPass->setCullMode(vg::CullModeFlagBits::FRONT);
	    pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	    pPass->setMainTexture(m_pCubeMapTex.get(),
			vg::ShaderStageFlagBits::FRAGMENT,
			vg::DescriptorType::COMBINED_IMAGE_SAMPLER,
			nullptr,
			m_pCubeMapTex->getSampler("other_sampler")
			);
	    pPass->apply();
	    
	    pMaterial->apply();
	}
	
	{
		//material
		auto & pMaterial = m_pMaterialReflect;
		pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		pMaterial->setRenderPriority(0u);
		pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
		auto pShader = pMaterial->getMainShader();
	    auto pPass = pMaterial->getMainPass();
	    
	    //shader
		pShader->load("shaders/cubemaps/reflect.vert.spv",
			"shaders/cubemaps/reflect.frag.spv");
	    //pass
		vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
			{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
			{vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW},
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
	    pPass->setMainTexture(m_pCubeMapTex.get(),
			vg::ShaderStageFlagBits::FRAGMENT,
			vg::DescriptorType::COMBINED_IMAGE_SAMPLER,
			nullptr,
			m_pCubeMapTex->getSampler("other_sampler")
		    );
	    pPass->setDataValue("other_info", m_otherInfo, 2u);	
	    pPass->apply();
	    
	    pMaterial->apply();
	}
}

void Window::_createModel()
{
	const uint32_t layoutCount = 3u;
	sampleslib::AssimpScene::VertexLayoutComponent layouts[layoutCount] = {
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_POSITION,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_NORMAL,
		sampleslib::AssimpScene::VertexLayoutComponent::VERTEX_COMPONENT_UV,
	};
	//Sky box
	sampleslib::AssimpScene::CreateInfo createInfo;
	createInfo.fileName = "models/cube.obj";
	createInfo.isCreateObject = VG_TRUE;
	createInfo.layoutComponentCount = layoutCount;
	createInfo.pLayoutComponent = layouts;
	createInfo.offset = vg::Vector3(0.0f, 0.0f, 0.0f);
	createInfo.scale = vg::Vector3(0.05f);
	m_skyBoxObject.init(createInfo);

	//Other objects.
	std::vector<std::string> fileNames = { "models/sphere.obj", "models/teapot.dae", "models/torusknot.obj", "models/venus.fbx" };
	m_arrObjectNames = { "Sphere", "Teapot", "Torusknot", "Venus" };
	m_objects.resize(fileNames.size());
	size_t count = fileNames.size();
	for (size_t i = 0; i < count; ++i)
	{
		createInfo.fileName = fileNames[i].c_str();
		createInfo.scale = vg::Vector3(0.05f * (fileNames[i] == "models/venus.fbx" ? 3.0f : 1.0f));
		m_objects[i].init(createInfo);
	}
}

void Window::_initScene()
{
	{
		auto &objects = m_skyBoxObject.getObjects();
		for (auto &object : objects)
		{
			object->setMaterial(m_pMaterialSkybox.get());
			object->setIsVisibilityCheck(VG_FALSE);
			m_pScene->addVisualObject(object.get());
		}
	}
	{
		uint32_t objectSceneCount = static_cast<uint32_t>(m_objects.size());
		for (uint32_t i = 0; i < objectSceneCount; ++i)
		{
			auto &objectScene = m_objects[i];
			auto &objects = objectScene.getObjects();
			for (auto &object : objects)
			{
				object->setMaterial(m_pMaterialReflect.get());
				if (i == m_objectIndex) 
					m_pScene->addVisualObject(object.get());
			}
		}
		m_currIndex = m_objectIndex;
		
	}
}

void Window::_updateScene()
{
	{
		auto &objects = m_skyBoxObject.getObjects();
		for (auto &object : objects)
		{
			if (m_displaySkybox) 
			{
				m_pScene->addVisualObject(object.get());
			}
			else
			{
				if (object->getTransform()->getRoot() != nullptr) {
					m_pScene->removeVisualObject(object.get());
				}
			}
		}
	}
	{
		if (m_currIndex >= 0) {
			auto &objectScene = m_objects[m_currIndex];
			auto &objects = objectScene.getObjects();
			for (auto &object : objects)
			{
				m_pScene->removeVisualObject(object.get());
			}
		}

		m_currIndex = m_objectIndex;
		auto &objectScene = m_objects[m_currIndex];
		auto &objects = objectScene.getObjects();
		for (auto &object : objects)
		{
			m_pScene->addVisualObject(object.get());
		}
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
	if (ImGui::SliderFloat("LOD bias", &m_otherInfo.lodBias, 0.0f, (float)m_pCubeMapTex->getImage()->getInfo().mipLevels)) {
		auto pPass = m_pMaterialReflect->getMainPass();
		pPass->setDataValue("other_info", m_otherInfo, 2u);
		pPass->apply();
	}
	uint32_t count = static_cast<uint32_t>(m_arrObjectNames.size());
	std::vector<const char *> charItems(m_arrObjectNames.size());
	for (size_t i = 0; i < count; ++i)
	{
		charItems[i] = m_arrObjectNames[i].c_str();
	}
	if (ImGui::Combo("Object type", &m_objectIndex,
		charItems.data(), count, count)) {
		_updateScene();
	}
	if (ImGui::Checkbox("Skybox", &m_displaySkybox)) {
		_updateScene();
	}
	ImGui::End();
}

void Window::_render(const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

	ParentWindowType::_render(info, resultInfo);	
}