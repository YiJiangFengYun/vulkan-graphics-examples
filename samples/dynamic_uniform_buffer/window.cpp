#include "dynamic_uniform_buffer/window.hpp"

#include <iostream>
#include <gli/gli.hpp>
#include <random>
#include <functional>

#define M_PI 3.14159265358979323846

Window::Window(uint32_t width
	, uint32_t height
	, const char* title
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(width
		, height
		, title
	    )
	, m_tempPositions()
	, m_tempIndices()
	, m_pMesh()
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
	_loadModel();
	_createMesh();
	_initObjectsStateData();
	_createDynamicUniformData();
	_createMaterial();
	_createModel();
	_updateModelState();
}

Window::Window(std::shared_ptr<GLFWwindow> pWindow
	, std::shared_ptr<vk::SurfaceKHR> pSurface
)
	: sampleslib::Window<vg::SpaceType::SPACE_3>(pWindow
		, pSurface
	    )
	, m_tempPositions()
	, m_tempIndices()
	, m_pMesh()
	, m_pShader()
	, m_pPass()
	, m_pMaterial()
{
	_init();
	_loadModel();
	_createMesh();
	_initObjectsStateData();
	_createDynamicUniformData();
	_createMaterial();
	_createModel();
	_updateModelState();
}

Window::~Window()
{
	if (m_pUniformMemory != nullptr)
	    free(m_pUniformMemory);
}

void Window::_init()
{
	m_zoom = -25.0f;
	// m_zoom = -0.15f;
	/// Build a quaternion from euler angles (pitch, yaw, roll), in radians.
	m_rotation = vg::Vector3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f));
}

void Window::_loadModel()
{
	m_tempPositions = { 
		{ -1.0f, -1.0f,  1.0f }, 
		{  1.0f, -1.0f,  1.0f }, 
		{  1.0f,  1.0f,  1.0f }, 
		{ -1.0f,  1.0f,  1.0f }, 
		{ -1.0f, -1.0f, -1.0f }, 
		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
	};

	m_tempColors = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	
	m_tempIndices = {
		0,1,2, 2,3,0, 1,5,6, 6,2,1, 7,6,5, 5,4,7, 4,0,3, 3,7,4, 4,5,1, 1,0,4, 3,2,6, 6,7,3,
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

void Window::_initObjectsStateData()
{
	auto pPhysicalDevice = vg::pApp->getPhysicalDevice();
	uint32_t minUboAlignment = static_cast<uint32_t>(pPhysicalDevice->getProperties().limits.minUniformBufferOffsetAlignment);
	uint32_t size = sizeof(vg::Matrix4x4);
	uint32_t bufferSize = size;
	if (minUboAlignment > 0) {
		bufferSize = (size + minUboAlignment - 1) & ~(minUboAlignment - 1);
	}
	m_sizeOneObject = size;
	m_bufferSizeOneObject = bufferSize;

	size_t totalBufferSize = OBJECT_INSTANCE_COUNT * bufferSize;
	void *memory = malloc(totalBufferSize);
	m_pUniformMemory = memory;
	m_uniformMemorySize = totalBufferSize;

	std::mt19937 rndGen(static_cast<uint32_t>(time(0)));
	std::normal_distribution<float> rndDist(-1.0f, 1.0f);
	for (uint32_t i = 0; i < OBJECT_INSTANCE_COUNT; i++)
	{
		m_rotations[i] = vg::Vector3(rndDist(rndGen), rndDist(rndGen), rndDist(rndGen)) * 2.0f * (float)M_PI;
		m_rotationSpeeds[i] = vg::Vector3(rndDist(rndGen), rndDist(rndGen), rndDist(rndGen));
	}

}



void Window::_createDynamicUniformData()
{
	m_pDynamicUniformData = std::shared_ptr<vg::UniformBufferData>{ new vg::UniformBufferData() };
	vg::UniformBufferData::SubDataInfo subDataInfo;

	vk::DescriptorSetLayoutBinding bindingInfo;
	bindingInfo.binding = 2u;
	bindingInfo.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
	bindingInfo.descriptorCount = 1u;
	bindingInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;

	subDataInfo.layoutBindingCount = 1u;
	subDataInfo.pLayoutBindings = &bindingInfo;
	subDataInfo.bufferOffset = 0u;

	vg::UniformBufferData::DescriptorBufferInfo bufferInfo;
	bufferInfo.range = m_sizeOneObject;
	bufferInfo.bufferRange = m_bufferSizeOneObject;

	subDataInfo.pDescriptorInfos = &bufferInfo;

	m_pDynamicUniformData->init(1u, &subDataInfo, m_pUniformMemory, m_uniformMemorySize);
}

void Window::_createMaterial()
{

	auto & pShader = m_pShader;
	auto & pPass = m_pPass;
	auto & pMaterial = m_pMaterial;
	auto & pApp = vg::pApp;
	//shader
	pShader = std::shared_ptr<vg::Shader>(
		new vg::Shader("shaders/dynamic_uniform_buffer/base.vert.spv", "shaders/dynamic_uniform_buffer/base.frag.spv")
		 //new vg::Shader("shaders/test.vert.spv", "shaders/test.frag.spv")
		);
	//pass
	pPass = std::shared_ptr<vg::Pass>(new vg::Pass(pShader.get()));
	vg::Pass::BuildInDataInfo::Component buildInDataCmps[2] = {
			{vg::Pass::BuildInDataType::MATRIX_PROJECTION},
			{vg::Pass::BuildInDataType::MATRIX_VIEW}
		};
		vg::Pass::BuildInDataInfo buildInDataInfo;
		buildInDataInfo.componentCount = 2u;
		buildInDataInfo.pComponent = buildInDataCmps;
	pPass->setBuildInDataInfo(buildInDataInfo);
	pPass->setCullMode(vg::CullModeFlagBits::NONE);
	pPass->setFrontFace(vg::FrontFaceType::COUNTER_CLOCKWISE);
	vk::PipelineDepthStencilStateCreateInfo depthStencilState = {};
	depthStencilState.depthTestEnable = VG_TRUE;
	depthStencilState.depthWriteEnable = VG_TRUE;
	depthStencilState.depthCompareOp = vk::CompareOp::eLessOrEqual;
	pPass->setDepthStencilInfo(depthStencilState);
	vg::Pass::ExternalUniformBufferInfo externalUniformBufferInfo;
	externalUniformBufferInfo.pData = m_pDynamicUniformData.get();
	externalUniformBufferInfo.subDataOffset = 0u;
	externalUniformBufferInfo.subDataCount = 1u;
	pPass->setExternalUniformBufferData(externalUniformBufferInfo);
	pPass->apply();
	//material
	pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
	pMaterial->addPass(pPass.get());
	pMaterial->setRenderPriority(0u);
	pMaterial->setRenderQueueType(vg::MaterialShowType::OPAQUE);
	pMaterial->apply();

}

void Window::_createModel()
{
	for (uint32_t i = 0; i < OBJECT_INSTANCE_COUNT; ++i)
	{
		m_pModels[i] = std::shared_ptr<vg::VisualObject3>(new vg::VisualObject3());
		m_pModels[i]->setMesh(m_pMesh.get());
		m_pModels[i]->setMaterial(m_pMaterial.get());
		//m_pModels[i]->setIsVisibilityCheck(VG_FALSE);
		m_pScene->addVisualObject(m_pModels[i].get());
	}
}

void Window::_updateModelState()
{
	float time = m_frameTimer;

	// Dynamic ubo with per-object model matrices indexed by offsets in the command buffer
	uint32_t dim = static_cast<uint32_t>(pow(OBJECT_INSTANCE_COUNT, (1.0f / 3.0f)));
	vg::Vector3 offset(5.0f);
	for (uint32_t x = 0; x < dim; x++)
	{
		for (uint32_t y = 0; y < dim; y++)
		{
			for (uint32_t z = 0; z < dim; z++)
			{
				uint32_t index = x * dim * dim + y * dim + z;
				// Aligned offset
				vg::Matrix4x4* modelMat = (vg::Matrix4x4*)(((uint64_t)m_pUniformMemory + (index * m_bufferSizeOneObject)));
				// Update rotations
				m_rotations[index] += time * m_rotationSpeeds[index];
				// Update matrices
				vg::Vector3 pos = vg::Vector3(-((dim * offset.x) / 2.0f) + offset.x / 2.0f + x * offset.x, -((dim * offset.y) / 2.0f) + offset.y / 2.0f + y * offset.y, -((dim * offset.z) / 2.0f) + offset.z / 2.0f + z * offset.z);
				//vg::Vector3 pos = vg::Vector3(1.0f);
				vg::Matrix4x4 matrix = glm::translate(vg::Matrix4x4(1.0f), pos);
				matrix = glm::rotate(matrix, m_rotations[index].x, vg::Vector3(1.0f, 1.0f, 0.0f));
				matrix = glm::rotate(matrix, m_rotations[index].y, vg::Vector3(0.0f, 1.0f, 0.0f));
				matrix = glm::rotate(matrix, m_rotations[index].z, vg::Vector3(0.0f, 0.0f, 1.0f));

                auto model = m_pModels[index];
				model->getTransform()->setLocalMatrix(matrix);
				*modelMat = matrix;
			}
		}
	}

}

void Window::_updateDynamicUniformBuffer()
{
	m_pDynamicUniformData->updateBuffer(m_pUniformMemory, m_uniformMemorySize);
}

void Window::_updateObjectDynamicOffset(vg::BaseVisualObject * pVisualObject)
{
	for (uint32_t i = 0; i < OBJECT_INSTANCE_COUNT; ++i)
	{
		if (m_pModels[i]->getID() == dynamic_cast<vg::BaseObject *>(pVisualObject)->getID())
		{
			auto &subData = *m_pDynamicUniformData->getSubDatas();
			vg::UniformBufferData::SubDataInfo subDataInfo;
			subDataInfo.bufferOffset = subData.getBufferOffset();
			subDataInfo.layoutBindingCount = subData.getLayoutBindingCount();

			auto binding = *subData.getLayoutBindings();
			subDataInfo.pLayoutBindings = &binding;
			auto bufferInfo = *subData.getDescriptorInfos();
			subDataInfo.pDescriptorInfos = &bufferInfo;
			bufferInfo.dynamicOffset = i * m_bufferSizeOneObject;
			m_pDynamicUniformData->updateDesData(1u, &subDataInfo);
			m_pPass->applyUniformBufferDynamicOffsets();
			break;
		}
	}
}

void Window::_onUpdate()
{
	ParentWindowType::_onUpdate();

	_updateModelState();
	_updateDynamicUniformBuffer();
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
	pRenderer->setPreObjectRecordingCallBack(std::bind(&Window::_updateObjectDynamicOffset, this, std::placeholders::_1));
	ParentWindowType::_renderWithRenderer(pRenderer, info, resultInfo);	
}