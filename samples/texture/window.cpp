#include "texture/window.hpp"

#include <iostream>
#include <gli/gli.hpp>

Window::OtherInfo::OtherInfo()
    : viewPos(vg::Vector4())
	, lodBias(0.0f)
{

}

Window::OtherInfo::OtherInfo(vg::Vector4 viewPos, float lodBias)
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
	, m_tempPositions()
	, m_tempTexCoords()
	, m_tempNormals()
	, m_tempIndices()
	, m_pModel()
	, m_pMesh()
	, m_pTexture()
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
	_loadModel();
	_createMesh();
	_createTexture();
	_createMaterial();
	_createModel();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_tempPositions()
	, m_tempTexCoords()
	, m_tempNormals()
	, m_tempIndices()
	, m_pModel()
	, m_pMesh()
	, m_pTexture()
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
	_loadModel();
	_createMesh();
	_createTexture();
	_createMaterial();
	_createModel();	
}

void Window::_init()
{
	m_zoom = -2.5f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Vector3(glm::radians(0.0f), glm::radians(15.0f), glm::radians(0.0f));
}

void Window::_loadModel()
{
	m_tempPositions = { vg::Vector3(1.0f, 1.0f, 1.0f)
	    , vg::Vector3(-1.0f, 1.0f, 1.0f)
	    , vg::Vector3(-1.0f, -1.0f, 1.0f)
	    , vg::Vector3(1.0f, -1.0f, 1.0f)
	};
	m_tempTexCoords = { vg::Vector2(1.0f, 1.0f)
	    , vg::Vector2(0.0f, 1.0f)
	    , vg::Vector2(0.0f, 0.0f)
	    , vg::Vector2(1.0f, 0.0f)
	};
	m_tempNormals = {vg::Vector3(0.0f, 0.0f, -1.0f)
	    , vg::Vector3(0.0f, 0.0f, -1.0f)
	    , vg::Vector3(0.0f, 0.0f, -1.0f)
	    , vg::Vector3(0.0f, 0.0f, -1.0f)
	};
	m_tempIndices = {
		0, 1, 2, 2, 3, 0
	};
}

void Window::_createMesh()
{
	m_pMesh = static_cast<std::shared_ptr<vg::DimSepMesh3>>(new vg::DimSepMesh3());
	m_pMesh->setVertexCount(static_cast<uint32_t>(m_tempPositions.size()));
	m_pMesh->setPositions(m_tempPositions);
	m_pMesh->setNormals(m_tempNormals);
	m_pMesh->setTextureCoordinates<vg::TextureCoordinateType::VECTOR_2, vg::TextureCoordinateIndex::TextureCoordinate_0>(m_tempTexCoords);
	m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
	m_pMesh->apply(VG_TRUE);
}

void Window::_createTexture()
{
	//load texture
	std::string fileName = "textures/metalplate01_rgba.ktx";
	vg::TextureFormat format = vg::TextureFormat::R8G8B8A8_UNORM;
	gli::texture2d gliTex2D(gli::load(fileName));
	if (gliTex2D.empty()) {
		throw std::runtime_error("The texture do't exist! path: " + fileName);
	}

	auto pTex = new vg::Texture2D(format, VG_TRUE, 
		gliTex2D[0].extent().x, 
		gliTex2D[0].extent().y
	);
	m_pTexture = std::shared_ptr<vg::Texture2D>(pTex);
	uint32_t mipLevels = static_cast<uint32_t>(gliTex2D.levels());
	vg::TextureDataLayout textureLayout;
	std::vector<vg::TextureDataLayout::Component> components(mipLevels);
	for (uint32_t i = 0; i < mipLevels; ++i)
	{
		components[i].mipLevel = i;
		components[i].baseArrayLayer = 0u;
		components[i].layerCount = m_pTexture->getArrayLayerCount();
		components[i].size = gliTex2D[i].size();
		components[i].hasImageExtent = VG_TRUE;
		components[i].width = gliTex2D[i].extent().x;
		components[i].height = gliTex2D[i].extent().y;
		components[i].depth = 1u;
	}
	textureLayout.componentCount = components.size();
	textureLayout.pComponent = components.data();
	m_pTexture->applyData(textureLayout, gliTex2D.data(), gliTex2D.size());

	m_pTexture->setFilterMode(vg::FilterMode::LINEAR);
	m_pTexture->setSamplerAddressMode(vg::SamplerAddressMode::REPEAT);

	auto &pApp = vg::pApp;
	auto &pDevice = pApp->getDevice();
	auto &pPhysicalDevice = pApp->getPhysicalDevice();
	if (pApp->getPhysicalDeviceFeatures().samplerAnisotropy)
	{
		auto anisotropy = pPhysicalDevice->getProperties().limits.maxSamplerAnisotropy;
		m_pTexture->setAnisotropy(anisotropy);
	}
}

void Window::_createMaterial()
{

	auto & pShader = m_pShader;
	auto & pPass = m_pPass;
	auto & pMaterial = m_pMaterial;
	auto & pApp = vg::pApp;
	//shader
	pShader = std::shared_ptr<vg::Shader>(
		new vg::Shader("shaders/texture/texture.vert.spv", "shaders/texture/texture.frag.spv")
		// new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
		);
	//pass
	pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader));
	pPass->setCullMode(vg::CullModeFlagBits::FRONT);
	pPass->setFrontFace(vg::FrontFaceType::CLOCKWISE);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	pPass->setDepthStencilInfo(depthStencilState);
	pPass->setMainTexture(m_pTexture);
	pPass->setDataValue("other_info", m_otherInfo, 2u);
	pPass->apply();
	//material
	pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	pMaterial->addPass(pPass);
	pMaterial->setRenderPriority(0u);
	pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	pMaterial->apply();

}

void Window::_createModel()
{
	m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
	m_pModel->setMesh(m_pMesh);
	m_pModel->setMaterial(m_pMaterial);
	m_pScene->addVisualObject(m_pModel);
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
	m_otherInfo.viewPos = vg::Vector4(0.0f, 0.0f, m_zoom, 1.0f);

	auto pos = m_lastWinPos;
	auto size = m_lastWinSize;
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + size.y + 10));
	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::SliderFloat("LOD bias", &m_otherInfo.lodBias, 0.0f, (float)m_pTexture->getMipmapLevels())) {
		m_pPass->setDataValue("other_info", m_otherInfo, 2u);
	    m_pPass->apply();
	}
	ImGui::End();
}

void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{

	ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);	
}