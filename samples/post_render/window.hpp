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
    enum class MaterialType {
		MUTILPLY_COLOR,
		MATEERIAL_COUNT,
	};

	const std::string MATERIAL_NAMES[static_cast<uint32_t>(MaterialType::MATEERIAL_COUNT)] = {
		"Mutilply Color",
	};

    bool m_wireFrame;
	sampleslib::AssimpScene m_assimpScene;
	std::shared_ptr<vg::Texture2D> m_pTexture;
	
	std::shared_ptr<vg::Material> m_pMaterialSolid;

	std::shared_ptr<vg::Material> m_pMaterialWireframe;

	std::shared_ptr<vg::PostRender> m_pPostRender;

	std::vector<std::pair<std::string, std::shared_ptr<vg::Material>>> m_postRenderMaterials;

	int32_t m_currPostRenderMaterialIndex;

	struct OtherInfo 
	{
		vg::Vector4 lightPos;
	} m_otherInfo;

	struct MutiplyColorInfo
	{
		vg::Color color;
	} m_mutiplyColorInfo;

	virtual void _init() override;
	virtual void _initState() override;
	void _createModel();
	void _createTexture();
	void _createMaterial();
	void _initScene();
	void _initPostRender();

	virtual void _onUpdate() override;

	virtual void _onPostReCreateSwapchain() override;

	static std::vector<vg::Renderer::SceneInfo> mySceneInfos;
	virtual void _onPreRender(vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
