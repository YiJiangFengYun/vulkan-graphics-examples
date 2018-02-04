#ifndef VG_MESH_H
#define VG_MESH_H

#define NOMINMAX
#include <set>
#include <utility>
#include <glm/glm.hpp>
#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/module.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/vertex_data/vertex_data_option.hpp"
#include "graphics/vertex_data/vertex_data.hpp"
#include "graphics/vertex_data/index_data.hpp"
#include "graphics/mesh/mesh_option.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
	class BaseMesh : public Base
	{
    public:
        BaseMesh();
		virtual ~BaseMesh();

    protected:
    private:
	};

	class DimensionMesh : public BaseMesh
	{
    public:

	protected:
	};

    template <MeshDimType meshDimType>
	class Mesh : public DimensionMesh
	{
    public:
	    typedef typename MeshTypeInfo<meshDimType>::PointType PointType;

		Mesh();

		MeshDimType getMeshDimType() const;

        virtual Bool32 getIsHasBounds() = 0;
		/*The bounding volume of the mesh*/
		virtual fd::Bounds<PointType> getBounds() = 0;
	protected:
		MeshDimType m_meshDimType;
	};
	

	class ContentMesh
	{
	public:
        ContentMesh();
        ContentMesh(MemoryPropertyFlags bufferMemoryPropertyFlags);
		virtual ~ContentMesh();
		const std::shared_ptr<VertexData> &getVertexData() const;
		const std::shared_ptr<IndexData> &getIndexData() const;
		virtual uint32_t getSubMeshCount() const;
	protected:
        std::shared_ptr<VertexData> m_pVertexData;
		std::shared_ptr<IndexData> m_pIndexData;
	};
    
	//This mesh class for separation atrtribute layout of vertex and sub mesh feature.
	class SepMesh : public ContentMesh
	{
	public:
		struct LayoutBindingInfo
		{
			std::string name;
			MeshData::DataType dataType;
			uint32_t bindingPriority;

			LayoutBindingInfo();

			LayoutBindingInfo(std::string name,
				MeshData::DataType dataType,
				uint32_t bindingPriority);

			LayoutBindingInfo(const LayoutBindingInfo &);

			LayoutBindingInfo(const LayoutBindingInfo &&);

			LayoutBindingInfo &operator=(const LayoutBindingInfo &);

			Bool32 operator ==(const LayoutBindingInfo &target) const;

			Bool32 operator<(const LayoutBindingInfo &target) const;
		};

		struct SubMeshInfo
		{
			PrimitiveTopology topology;
			std::vector<uint32_t> indices;
		};

		SepMesh();
		SepMesh(MemoryPropertyFlags bufferMemoryPropertyFlags);

		~SepMesh();

		uint32_t getVertexCount() const;

		void setVertexCount(uint32_t value);

		//color
		const std::vector<Color32> &getColors() const;

		void setColors(const std::vector<Color32> &colors);

		//index
		uint32_t getSubMeshCount() const;

		void setSubMeshCount(uint32_t value);

		const std::vector<uint32_t> &getIndices(uint32_t subMeshIndex) const;

		void setIndices(const std::vector<uint32_t> &indices, PrimitiveTopology topology, uint32_t subMeshIndex = 0u);

		uint32_t getIndexCount(uint32_t subMeshIndex) const;

		uint32_t getIndexStart(uint32_t subMeshIndex) const;

		PrimitiveTopology getTopology(uint32_t subMeshIndex) const;

		/**Vertex colors of the Mesh multiplied to verties*/
		Color getMultipliedColor() const;

		/**Vertex colors of the Mesh multiplied to verties*/
		void setMultipliedColor(Color value);

		/**Vertex colors of the Mesh added to verties*/
		Color getAddedColor() const;

		/**Vertex colors of the Mesh added to verties*/
		void setAddedColor(Color value);

		virtual void apply(Bool32 makeUnreadable);

		//uv
		template<UVType uvType, UVIndex uvIndex>
		const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &getUVs() const;

		template<UVType uvType, UVIndex uvIndex>
		void setUVs(const typename MeshData::DataTypeInfo<UVConstInfo<uvType>::ARRAY_TYPE>::ValueType &uvs);

		template<MeshData::DataType dataType>
		const typename MeshData::DataTypeInfo<dataType>::ValueType &getData(std::string name) const;

		template <MeshData::DataType dataType>
		void setData(const std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority = VG_VERTEX_BINDING_PRIORITY_OTHER_MIN);

	protected:
		uint32_t m_vertexCount;
		std::shared_ptr<MeshData> m_pData;
		std::vector<std::string> m_arrLayoutBindingInfoNames;
		std::unordered_map<std::string, LayoutBindingInfo> m_mapLayoutBindingInfos;
		uint32_t m_subMeshCount;
		std::vector<SubMeshInfo> m_subMeshInfos;

		Color m_multipliedColor;
		Color m_addedColor;

		Bool32 m_applied;
		uint32_t m_appliedVertexCount; //save vertex count to render.
		uint32_t m_appliedSubMeshCount;
		std::set<LayoutBindingInfo> m_layoutBindingInfos;
		std::vector<SubMeshInfo> m_usingSubMeshInfos; //save sub mesh info to render.

		void _createMeshData();

		void _createVertexData();

		void _createIndexData();

		template<MeshData::DataType dataType>
		inline const typename MeshData::DataTypeInfo<dataType>::ValueType &_getData(std::string name) const;

		template <MeshData::DataType dataType>
		inline void _setData(std::string name, const typename MeshData::DataTypeInfo<dataType>::ValueType &value, uint32_t bindingPriority = VG_VERTEX_BINDING_PRIORITY_OTHER_MIN);


		inline void _sortLayoutBindingInfos();
		//tool methods
#ifdef DEBUG
		inline void _verifySubMeshIndex(uint32_t subMeshIndex) const;
#endif // DEBUG

		inline uint32_t _getIndexCount(uint32_t subMeshIndex) const;

	};

	template <MeshDimType meshDimType>
	class DimSepMesh : public SepMesh, public Mesh<meshDimType>
	{
	public:
	    static const MeshData::DataType ARRAY_DATA_TYPE = MeshConstInfo<meshDimType>::ARRAY_TYPE;
		typedef typename MeshData::DataTypeInfo<ARRAY_DATA_TYPE>::BaseType BaseValueType;
		typedef typename MeshData::DataTypeInfo<ARRAY_DATA_TYPE>::ValueType ArrayValueType;

		DimSepMesh();
		DimSepMesh(MemoryPropertyFlags bufferMemoryPropertyFlags);

		virtual ~DimSepMesh();

		//position
		virtual const ArrayValueType &getPositions() const;

		virtual void setPositions(const ArrayValueType &vertices);

		//normal
		virtual const ArrayValueType &getNormals() const;

		virtual void setNormals(const ArrayValueType &normals);

		//tangent
		virtual const ArrayValueType &getTangents() const;

		virtual void setTangents(const ArrayValueType &tangents);

		virtual void apply(Bool32 makeUnreadable) override;

        virtual Bool32 getIsHasBounds() override;

		/*The bounding volume of the mesh*/
		virtual fd::Bounds<PointType> getBounds() override;

	private:
		
		fd::Bounds<PointType> m_bounds;

		inline void _updateBounds();
	};

	class SimpleMesh : public ContentMesh
	{
	public:
	    SimpleMesh();
	    SimpleMesh(MemoryPropertyFlags bufferMemoryPropertyFlags);
	protected:
	};

	template <MeshDimType meshDimType>
	class DimSimpleMesh : public SimpleMesh, public Mesh<meshDimType>
	{
	public: 
        DimSimpleMesh();
        DimSimpleMesh(MemoryPropertyFlags bufferMemoryPropertyFlags);
	    virtual Bool32 getIsHasBounds() override;
		/*The bounding volume of the mesh*/
		fd::Bounds<PointType> getBounds() override;
	private:
	};
}

#include "graphics/mesh/mesh.inl"

#endif // !VG_MESH_H
