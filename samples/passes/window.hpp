#ifndef WINDOW_H
#define WINDOW_H

#include "sampleslib/window.hpp"
#include "sampleslib/scene_assimp.hpp"
#include "passes/material_deferred.hpp"

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
	std::shared_ptr<MaterialDeferred> m_pMaterialOfScene;

	struct Light {
		vg::Vector4 position;
		vg::Vector4 color;
		float radius;
	};
	struct OtherInfo 
	{
		vg::Vector4 vewPos;
		Light lights[NUM_LIGHTS];
	} m_otherInfo;

	virtual void _init() override;
	
	virtual void _init() override;
	virtual void _initState() override;
	void _createModel();
	void _createTexture();
	void _createMaterial();
	void _initScene();

	virtual void _onUpdate() override;
};

#endif // !WINDOW_
