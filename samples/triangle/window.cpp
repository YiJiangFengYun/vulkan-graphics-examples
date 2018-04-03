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
	m_pMesh->setPositions(m_tempPositions);
	m_pMesh->setColor32s(m_tempColors);
	m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
	m_pMesh->apply(VG_TRUE);
}
void Window::_createMaterial()
{
	//shader
	m_pShader = std::shared_ptr<vg::Shader>(
		new vg::Shader("shaders/triangle/triangle.vert.spv", "shaders/triangle/triangle.frag.spv")
		);
	//pass
	m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader.get()));
	m_pPass->setMainColor(vg::Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPass->setPolygonMode(vg::PolygonMode::FILL);
	m_pPass->setCullMode(vg::CullModeFlagBits::NONE);
	m_pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);
	vk::PipelineColorBlendAttachmentState attachmentState[1] = {};
	attachmentState[0].colorWriteMask = vk::ColorComponentFlagBits::eR
		| vk::ColorComponentFlagBits::eG
		| vk::ColorComponentFlagBits::eB
		| vk::ColorComponentFlagBits::eA;
	attachmentState[0].blendEnable = VG_FALSE;
	vk::PipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = attachmentState;
	m_pPass->setColorBlendInfo(colorBlendState);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	m_pPass->setDepthStencilInfo(depthStencilState);
	//material
	m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	m_pMaterial->addPass(m_pPass.get());
	m_pMaterial->setRenderPriority(0u);
	m_pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	m_pMaterial->apply();
}
void Window::_createModel()
{
	m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
	m_pModel->setMesh(m_pMesh.get());
	m_pModel->setMaterial(m_pMaterial.get());
	m_pScene->addVisualObject(m_pModel.get());
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
}