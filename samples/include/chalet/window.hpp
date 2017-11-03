#ifndef WINDOW_H
#define WINDOW_H

#include <framework/framework.hpp>
#include "sampleslib/stb_image.h"
#include "sampleslib/tiny_obj_loader.h"

namespace chalet
{
	class Window : public gfw::Window
	{
	public:
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec3 color;
			glm::vec2 texCoord;

			bool operator==(const Vertex& other) const;
		};

		Window(uint32_t width
			, uint32_t height
			, const char* title
		);

		Window(uint32_t width
			, uint32_t height
			, const char* title
			, RenderType renderType
		);

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

		Window(RenderType renderType
			, std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

	private:
		std::vector<kgs::Vector3> m_tempPositions;
		std::vector<kgs::Vector2> m_tempTexCoords;
		std::vector<kgs::Color> m_tempColors;
		std::vector<uint32_t> m_tempIndices;


		std::shared_ptr<kgs::Scene3> m_pScene;
		std::shared_ptr<kgs::Camera3> m_pCamera;
		std::shared_ptr<kgs::VisualObject3> m_pModel;
		std::shared_ptr<kgs::Mesh3> m_pMesh;
		std::shared_ptr<kgs::Material> m_pMaterial;
		std::shared_ptr<kgs::Pass> m_pPass;
		std::shared_ptr<kgs::Shader> m_pShader;
		std::shared_ptr<kgs::Texture2D> m_pTexture;


		void _loadModel();
		void _createMesh();
		void _createTexture();
		void _createMaterial();
		void _createModel();
		void _createCamera();
		void _createScene();
		void _fillRenderer();

		void _onPreUpdate() override;
		void _update() override;
		void _onPostUpdate() override;

		void _onPreRender() override;
	    void _onPostRender() override;
	};

}

namespace std {
	template<> 
	struct hash<chalet::Window::Vertex> {
		size_t operator()(const chalet::Window::Vertex& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

#endif // !WINDOW_H
