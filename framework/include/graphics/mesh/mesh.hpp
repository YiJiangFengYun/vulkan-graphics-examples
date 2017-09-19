#ifndef KGS_MESH_H
#define KGS_MESH_H

#include <glm/glm.hpp>
#include "graphics/global.hpp"
#include "graphics/mesh/mesh_option.hpp"
#include "graphics/mesh/mesh_data.hpp"

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
	template <MeshType meshType>
	class Mesh
	{
	public:
		typedef typename MeshData::DataTypeInfo<MeshTypeInfo<meshType>::baseType>::value_t base_data_t;
		typedef typename MeshData::DataTypeInfo<MeshTypeInfo<meshType>::arrType>::value_t arr_data_t;
		struct LayoutInfo
		{
			std::string name;
			MeshData::DataType dataType;
			uint32_t location;

			LayoutInfo(std::string name,
				MeshData::DataType dataType,
				uint32_t location):
				name(name), 
				dataType(dataType), 
				location(location)
			{

			}

			Bool32 operator ==(const LayoutInfo& target) const
			{
				return name == target.name && dataType == target.dataType && location == target.location;
			}
		};


		Mesh()
		{

		}

		~Mesh()
		{

		}

		MeshType getMeshType() const
		{
			return m_meshType;
		}

		uint32_t getVertexCount() const
		{
			return m_vertexCount;
		}

		void setVertexCount(uint32_t value)
		{
			m_vertexCount = value;
		}

		//vertex
		arr_data_t getVertices() const
		{
			
		}

		void setVertices(arr_data_t vertices);

		//color
		std::vector<Color32> getColors() const;
		void setColors(std::vector<Color32> colors);

		//normal
		arr_data_t getNormals() const;
		void setNormals(arr_data_t normals);

		//tangent
		arr_data_t getTangents() const;
		void setTangents(arr_data_t tangents);

		//uv
		template<UVType uvType>
		typename MeshData::DataTypeInfo<UVTypeInfo<uvType>::arrType>::value_t getUVs(uint32_t uvIndex);

		template<UVType uvType>
		void setUVs(typename MeshData::DataTypeInfo<UVTypeInfo<uvType>::arrType>::value_t uvs, uint32_t uvIndex);

		//index
		std::vector<uint32_t> getIndices(uint32_t subMeshIndex) const;
		void setIndices(std::vector<uint32_t> indices, PrimitiveTopology topology, uint32_t subMeshIndex);

		uint32_t getIndexCount(uint32_t subMeshIndex) const;
		uint32_t getIndexStart(uint32_t subMeshIndex) const;

		PrimitiveTopology getTopology(uint32_t subMeshIndex) const;

		uint32_t getSubMeshCount() const;
		void setSubMeshCount(uint32_t value);

		/*The bounding volume of the mesh*/
		fd::Bounds<base_data_t> getBounds();

		/**Vertex colors of the Mesh multiplied to verties*/
		Color getMultipliedColor() const;
		/**Vertex colors of the Mesh multiplied to verties*/
		void setMultipliedColor(Color32 value);

		/**Vertex colors of the Mesh added to verties*/
		Color getAddedColor() const;
		/**Vertex colors of the Mesh added to verties*/
		void setAddedColor(Color value);


	private:
		MeshType m_meshType = meshType;
		uint32_t m_vertexCount;
		std::shared_ptr<MeshData> m_pData;
		fd::Bounds<base_data_t> m_bounds;
		Color m_multipliedColor;
		Color m_addedColor;
	};
}

#endif // !KGS_MESH_H
