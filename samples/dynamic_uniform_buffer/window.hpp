#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define OBJECT_INSTANCE_COUNT 125

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
private:
    vg::Vector3 m_rotations[OBJECT_INSTANCE_COUNT];
	vg::Vector3 m_rotationSpeeds[OBJECT_INSTANCE_COUNT];

	struct OtherInfo {
		vg::Matrix4x4 *model = nullptr;
	} m_otherInfo;

    std::vector<vg::Vector3> m_tempPositions;
	std::vector<vg::Vector2> m_tempTexCoords;
	std::vector<vg::Vector3> m_tempNormals;
	std::vector<uint32_t> m_tempIndices;
	std::shared_ptr<vg::VisualObject3> m_pModel;
	std::shared_ptr<vg::DimSepMesh3> m_pMesh;
	std::shared_ptr<vg::Texture2D> m_pTexture;
	std::shared_ptr<vg::Shader> m_pShader;
	std::shared_ptr<vg::Pass> m_pPass;
	std::shared_ptr<vg::Material> m_pMaterial;
	
	void _init();
	void _loadModel();
	void _createMesh();
	void _createTexture();
	void _createMaterial();
	void _createModel();

	virtual void _onUpdate() override;
	virtual void _renderWithRenderer(vg::Renderer *pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
