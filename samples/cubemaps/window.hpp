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
    bool m_displaySkybox = true;
	sampleslib::AssimpScene m_skyBoxObject;
	std::vector<sampleslib::AssimpScene> m_objects;
	int32_t m_objectIndex;
	int32_t m_currIndex;
	std::vector<std::string> m_arrObjectNames;
	std::shared_ptr<vg::TextureCube> m_pCubeMapTex;
	std::shared_ptr<vg::Material> m_pMaterialSkybox;

	std::shared_ptr<vg::Material> m_pMaterialReflect;
	struct OtherInfo 
	{
	    float lodBias;
		OtherInfo();
		OtherInfo(float lodBias);
	} m_otherInfo;
	
	virtual void _init() override;
	virtual void _initState() override;
	void _createTexture();
	void _createMaterial();
	void _createModel();
	void _initScene();

	void _updateScene();

	virtual void _onUpdate() override;
};
#endif // !WINDOW_H
