#ifndef KGS_MESH_H
#define KGS_MESH_H

#include <glm/glm.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_option.hpp"

#define KGS_VERTEX_POSTION_NAME "_Position"
#define KGS_VERTEX_COLOR_NAME "_Color"
#define KGS_VERTEX_NORMAL_NAME "_Normal"
#define KGS_VERTEX_TANGENT_NAME "_Tangent"
//#define KGS_VERTEX_UV

#define KGS_VERTEX_POSITION_LOCATION 0
#define KGS_VERTEX_COLOR_LOCATION 1
#define KGS_VERTEX_NORMAL_LOCATION 2
#define KGS_VERTEX_TANGENT_LOCATION 3

namespace kgs
{
	template <typename VecType>
	class Mesh
	{
	public:
		typedef VecType type_value;

		struct LayoutInfo
		{
			std::string name;
		};


		Mesh();
		~Mesh();

		uint32_t getVertexCount() const;

		//vertex
		std::vector<type_value> getVertices() const;
		void setVertices(std::vector<type_value> vertices);

		//color
		std::vector<Color32> getColors() const;
		void setColors(std::vector<Color32> colors);

		//normal
		std::vector<type_value> getNormals() const;
		void setNormals(std::vector<type_value> normals);

		//tangent
		std::vector<type_value> getTangents() const;
		void setTangents(std::vector<type_value> tangents);

		//uv
		template<typename UVVecType>
		std::vector<UVVecType> getUVs(uint32_t uvIndex);

		template<typename UVVecType>
		void setUVs(std::vector<UVVecType> uvs, uint32_t uvIndex);

		//index
		std::vector<uint32_t> getIndices(uint32_t subMeshIndex) const;
		void setIndices(std::vector<uint32_t> indices, PrimitiveTopology topology, uint32_t subMeshIndex);

		uint32_t getIndexCount(uint32_t subMeshIndex) const;
		uint32_t getIndexStart(uint32_t subMeshIndex) const;

		PrimitiveTopology getTopology(uint32_t subMeshIndex) const;

		uint32_t getSubMeshCount() const;
		void setSubMeshCount(uint32_t value);

		/*The bounding volume of the mesh*/
		fd::Bounds<type_value> getBounds();

		/**Vertex colors of the Mesh multiplied to verties*/
		Color getMultipliedColor() const;
		/**Vertex colors of the Mesh multiplied to verties*/
		void setMultipliedColor(Color32 value);

		/**Vertex colors of the Mesh added to verties*/
		Color getAddedColor() const;
		/**Vertex colors of the Mesh added to verties*/
		void setAddedColor(Color value);


	private:
		fd::Bounds<type_value> m_bounds;
		Color m_multipliedColor;
		Color m_addedColor;
	};
}

#endif // !KGS_MESH_H
