#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define OBJECT_INSTANCE_COUNT 125


/**
 * This sample explain we can use a uniform buffer
 * to render plenty of objects via external unform buffer of pass.
 **/
class Window : public sampleslib::Window<vg::SpaceType::SPACE_3>
{
public:
    using ParentWindowType = sampleslib::Window<vg::SpaceType::SPACE_3>;
	
	Window(uint32_t width
		, uint32_t height
		, const char* title
	);
	Window(std::shared_ptr<GLFWwindow> pWindow
		, std::shared_ptr<vk::SurfaceKHR> pSurface
	);

	~Window();
private:
    vg::Vector3 m_rotations[OBJECT_INSTANCE_COUNT];
	vg::Vector3 m_rotationSpeeds[OBJECT_INSTANCE_COUNT];

    std::vector<vg::Vector3> m_tempPositions;
	std::vector<vg::Color> m_tempColors;
	std::vector<uint32_t> m_tempIndices;
	std::shared_ptr<vg::DimSepMesh3> m_pMesh;
	std::shared_ptr<vg::UniformBufferData> m_pExtUniformData;
	uint32_t m_sizeOneObject;
	uint32_t m_bufferSizeOneObject;
	uint32_t m_uniformMemorySize;
	void *m_pUniformMemory;

	std::shared_ptr<vg::Shader> m_pShader;

	std::shared_ptr<vg::Material> m_pMaterials[OBJECT_INSTANCE_COUNT];

	std::shared_ptr<vg::VisualObject3> m_pModels[OBJECT_INSTANCE_COUNT];	
	
	virtual void _init() override;
	virtual void _initState() override;
	void _loadModel();
	void _createMesh();
	void _initObjectsStateData();
	void _createDynamicUniformData();
	void _createMaterial();
	void _createModel();

	void _updateModelState();
	void _updateExtUniformBuffer();

	virtual void _onUpdate() override;
	virtual void _render(const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
