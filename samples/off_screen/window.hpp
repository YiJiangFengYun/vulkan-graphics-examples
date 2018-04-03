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
	sampleslib::AssimpScene m_assimpSceneModel;
	sampleslib::AssimpScene m_assimpScenePlane;
	// std::shared_ptr<vg::Texture2D> m_pOffScreenTex;
	
	std::shared_ptr<vg::Shader> m_pShaderModel;
	std::shared_ptr<vg::Pass> m_pPassModel;
	std::shared_ptr<vg::Material> m_pMaterialModel;

	std::shared_ptr<vg::Texture2D> m_pTexturePlane;    
	std::shared_ptr<vg::Shader> m_pShaderPlane;
	std::shared_ptr<vg::Pass> m_pPassPlane;
	std::shared_ptr<vg::Material> m_PMaterialPlane;

	struct OtherInfo 
	{
		vg::Vector4 lightPos;
	} m_otherInfo;

	virtual void _init() override;
	virtual void _initState() override;
	void _createModel();
	void _createTexture();
	void _createMaterial();
	void _initScene();

	virtual void _onUpdate() override;
	virtual void _renderWithRenderer(vg::Renderer *pRenderer
		    , const vg::Renderer::RenderInfo &info
			, vg::Renderer::RenderResultInfo &resultInfo) override;
};

#endif // !WINDOW_
