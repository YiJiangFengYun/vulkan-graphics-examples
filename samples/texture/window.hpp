#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

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
    std::vector<vg::Vector3> m_tempPositions;
	std::vector<vg::Vector2> m_tempTexCoords;
	std::vector<vg::Vector3> m_tempNormals;
	std::vector<uint32_t> m_tempIndices;
	std::shared_ptr<vg::VisualObject3> m_pModel;
	std::shared_ptr<vg::DimSepMesh3> m_pMesh;
	std::shared_ptr<vg::Texture2D> m_pTexture;
	std::shared_ptr<vg::Material> m_pMaterial;
	struct OtherInfo 
	{
		vg::Vector4 viewPos;
	    float lodBias;
		OtherInfo();
		OtherInfo(vg::Vector4 viewPos, float lodBias);
	} m_otherInfo;
	virtual void _init() override;
	virtual void _initState() override;
	void _loadModel();
	void _createMesh();
	void _createTexture();
	void _createMaterial();
	void _createModel();

	virtual void _onUpdate() override;
	virtual void _render(const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
