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
	std::shared_ptr<vg::Texture> m_pTexture;
	std::shared_ptr<vg::Shader> m_pShader;
	std::shared_ptr<vg::Pass> m_pPasse;
	std::shared_ptr<vg::Material> m_pMaterial;
	struct OtherInfo 
	{
		vg::Vector3 viewPos;
	    float lodBias;
		OtherInfo(vg::Vector3 viewPos = vg::Vector3(), float lodBias = 0.0f);
	} m_otherInfo;
	void _init();
	void _loadAssimpScene();
	void _createTexture();
	void _createMaterial();
	void _fillScene();

	virtual void _onUpdate() override;
	virtual void _renderWithRenderer(const std::shared_ptr<vg::Renderer> &pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
