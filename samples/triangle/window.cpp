#include "triangle/window.hpp"

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
	m_cameraZoom = -5.0f;
	_loadModel();
	_createMesh();
	_createMaterial();
	_createModel();
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
	m_pMesh->addPositions(m_tempPositions);
	m_pMesh->addColor32s(m_tempColors);
	m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
	m_pMesh->apply(VG_TRUE);
}
void Window::_createMaterial()
{
	//material
	m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	m_pMaterial->setRenderPriority(0u);
	m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);

	auto pShader = m_pMaterial->getMainShader();
	auto pPass = m_pMaterial->getMainPass();
	//shader
	pShader->load("shaders/triangle/triangle.vert.spv", "shaders/triangle/triangle.frag.spv");
	//pass
	pPass->setMainColor(vg::Color(1.0f, 1.0f, 1.0f, 1.0f));
	pPass->setPolygonMode(vk::PolygonMode::eFill);
	pPass->setCullMode(vk::CullModeFlagBits::eNone);
	pPass->setFrontFace(vk::FrontFace::eCounterClockwise);
	vk::PipelineColorBlendAttachmentState attachmentState[1] = {};
	attachmentState[0].colorWriteMask = vk::ColorComponentFlagBits::eR
		| vk::ColorComponentFlagBits::eG
		| vk::ColorComponentFlagBits::eB
		| vk::ColorComponentFlagBits::eA;
	attachmentState[0].blendEnable = VG_FALSE;
	vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = attachmentState;
	pPass->setColorBlendInfo(colorBlendState);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	pPass->setDepthStencilInfo(depthStencilState);
	
	m_pMaterial->apply();
}
void Window::_createModel()
{
	m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
	m_pModel->setMesh(m_pMesh.get());
	m_pModel->setMaterialCount(1u);
	m_pModel->setMaterial(m_pMaterial.get());
	m_pScene->addVisualObject(m_pModel.get());
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
}