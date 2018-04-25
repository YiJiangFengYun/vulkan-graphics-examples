#ifndef WINDOW_H
#define WINDOW_H

#include <graphics_ext/graphics_ext.hpp>
#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"

#define NUM_LIGHTS 64

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
	std::shared_ptr<vge::MaterialDeferred> m_pMaterialOfScene;
	std::shared_ptr<vg::Material> m_pMaterialOfSceneTest;

	sampleslib::AssimpScene m_assimpSceneGlass;
	std::shared_ptr<vg::Texture2D> m_pTextureOfSceneGlass;
	const vg::Texture::Sampler *m_pSamplerOfSceneGlass;
	std::shared_ptr<vg::Material> m_pMaterialOfSceneGlass;

	struct Light {
		vg::Vector4 position;
		vg::Vector3 color;
		float radius;
	};
	struct OtherInfo 
	{
		vg::Vector4 vewPos;
		Light lights[NUM_LIGHTS];
	} m_otherInfo;

	virtual void _init() override;
	virtual void _initState() override;
	void _initLights();
	void _createModel();
	void _createTexture();
	void _createMaterial();
	void _initScene();

	virtual void _onUpdate() override;
};

#endif // !WINDOW_
