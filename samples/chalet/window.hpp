#ifndef WINDOW_H
#define WINDOW_H

#include <stb/stb_image.h>
#include <sampleslib/tiny_obj_loader.h>
#include <sampleslib/window.hpp>

namespace chalet
{
	class Window : public sampleslib::Window<vg::SpaceType::SPACE_3>
	{
	public:
		using ParentWindowType = sampleslib::Window<vg::SpaceType::SPACE_3>;
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

		Window(std::shared_ptr<GLFWwindow> pWindow
			, std::shared_ptr<vk::SurfaceKHR> pSurface
		);

	private:
		std::vector<vg::Vector3> m_tempPositions;
		std::vector<vg::Vector2> m_tempTexCoords;
		std::vector<vg::Color> m_tempColors;
		std::vector<uint32_t> m_tempIndices;


		std::shared_ptr<vg::VisualObject3> m_pModel;
		std::shared_ptr<vg::DimSepMesh3> m_pMesh;
		std::shared_ptr<vg::Material> m_pMaterial;
		std::shared_ptr<vg::Pass> m_pPass;
		std::shared_ptr<vg::Shader> m_pShader;
		std::shared_ptr<vg::Texture2D> m_pTexture;

		std::shared_ptr<vg::VisualObject3> m_pModelOfBounds;
		std::shared_ptr<vg::DimSepMesh3> m_pMeshOfBounds;
		std::shared_ptr<vg::Material> m_pMaterialOfBounds;
		std::shared_ptr<vg::Pass> m_pPassOfBounds;
		std::shared_ptr<vg::Shader> m_pShaderOfBounds;


		void _loadModel();
		void _createMesh();
		void _createTexture();
		void _createMaterial();
		void _createModel();
		void _createMeshOfBounds();
		void _createMaterialOfBounds();
		void _createModelOfBounds();

		virtual void _onUpdate() override;
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
