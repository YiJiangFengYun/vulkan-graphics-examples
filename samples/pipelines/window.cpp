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
{
	_init();
	_loadModel();
	_createMesh();
	_createMaterial();
	_createModel();
}
Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
{
	_init();
	_loadModel();
	_createMesh();
	_createMaterial();
	_createModel();
}

void Window::_init()
{
	m_zoom = -10.5f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Quaternion(vg::Vector3(glm::radians(-25.0f), glm::radians(15.0f), glm::radians(0.0f)));
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
	m_pMesh->setColors(m_tempColors);
	m_pMesh->setIndices(m_tempIndices, vg::PrimitiveTopology::TRIANGLE_LIST, 0u);
	m_pMesh->apply(VG_TRUE);
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
		pPasses[i]->setViewport(fd::Viewport(0.0f, 0.0f, static_cast<float>(i / SCENE_COUNT), 1.0f));
		if (i == 1u && pApp->getPhysicalDeviceFeatures().wideLines)
		{
			pPasses[i]->setLineWidth(2.0f);
		}

	    //material
	    pMaterials[i] = std::shared_ptr<vg::Material>(new vg::Material());
	    pMaterials[i]->addPass(pPasses[i]);
	    pMaterials[i]->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	    pMaterials[i]->apply();
	}

}
void Window::_createModel()
{
	m_pModel = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
	m_pModel->setMesh(m_pMesh);
	m_pScene->addVisualObject(m_pModel);
}
void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();
}

void Window::_renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo)
{
	auto &pModel = m_pModel;
	auto &pApp = vg::pApp;
	for (uint32_t i = 0; i < SCENE_COUNT; ++i)
	{
		pModel->setMaterial(m_pMaterials[i]);
		if (i != 2u || pApp->getPhysicalDeviceFeatures().fillModeNonSolid)
		{
	        ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);
		}
	}
	
}