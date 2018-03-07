#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define SCENE_COUNT 3u
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
    sampleslib::AssimpScene m_assimpScene;
	std::array<std::shared_ptr<vg::Shader>, SCENE_COUNT> m_pShaders;
	std::array<std::shared_ptr<vg::Pass>, SCENE_COUNT> m_pPasses;
	std::array<std::shared_ptr<vg::Material>, SCENE_COUNT> m_pMaterials;
	void _init();
	void _loadAssimpScene();
	void _createMaterial();
	void _fillScene();

    void _setMaterialToObjects(std::shared_ptr<vg::Material> pMaterial);

	virtual void _onUpdate() override;
	virtual void _renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
