#ifndef VG_MESH_H
#define VG_MESH_H

#define NOMINMAX
#include <set>
#include <utility>
#include <glm/glm.hpp>
#include <foundation/foundation.hpp>
#include "graphics/global.hpp"
#include "graphics/app/app.hpp"
#include "graphics/util/find_memory.hpp"
#include "graphics/util/single_time_command.hpp"
#include "graphics/buffer_data/buffer_data_option.hpp"
#include "graphics/buffer_data/vertex_data.hpp"
#include "graphics/buffer_data/index_data.hpp"
#include "graphics/mesh/mesh_option.hpp"
#include "graphics/mesh/mesh_data.hpp"

namespace vg
{
    class BaseMesh : public Base
    {
    public:
        BaseMesh();
        void beginRecord() const;
        void endRecord() const;
    protected:
        virtual void _beginRecord() const;
        virtual void _endRecord() const;
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
        using PointType = typename MeshTypeInfo<meshDimType>::PointType;

        Mesh();

        MeshDimType getMeshDimType() const;

        virtual Bool32 getIsHasBounds() const;
        /*The bounding volume of the mesh*/
        virtual fd::Bounds<PointType> getBounds() const;
    protected:
        MeshDimType m_meshDimType;
        Bool32 m_hasBounds;
        fd::Bounds<PointType> m_bounds;        
    };
    

    class ContentMesh
    {
    public:
        ContentMesh();
        virtual ~ContentMesh();
        VertexData *getVertexData() const;
        IndexData *getIndexData() const;
        virtual uint32_t getSubMeshOffset() const;
        virtual uint32_t getSubMeshCount() const;
    protected:
        std::shared_ptr<VertexData> m_pVertexData;
        std::shared_ptr<IndexData> m_pIndexData;
    };

    class InternalContentMesh : public ContentMesh
    {
    public:
        InternalContentMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);
    protected:


    private:

    };

    class ExternalContentMesh : public ContentMesh
    {
    public:
        ExternalContentMesh();
        ExternalContentMesh(std::shared_ptr<VertexData> pVertexData
            , std::shared_ptr<IndexData> pIndexData
            , uint32_t subIndexDataOffset
            , uint32_t subIndexDataCount
            );

        void init(std::shared_ptr<VertexData> pVertexData
            , std::shared_ptr<IndexData> pIndexData
            , uint32_t subIndexDataOffset
            , uint32_t subIndexDataCount
            );

        virtual uint32_t getSubMeshOffset() const override;
        virtual uint32_t getSubMeshCount() const override;
        uint32_t getSubIndexDataOffset() const;
        void setSubIndexDataOffset(uint32_t offset);
        uint32_t getSubIndexDaTaCount() const;
        void setSubIndexDataCount(uint32_t count);
    protected:
        uint32_t m_subIndexDataOffset;
        uint32_t m_subIndexDataCount;
    private:

    };
    
    //This mesh class for separation atrtribute layout of vertex and sub mesh feature.
    class SepMesh : public InternalContentMesh
    {
    public:
        struct SubMeshInfo
        {
            PrimitiveTopology topology;
            std::vector<uint32_t> indices;
        };

        SepMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);

        virtual ~SepMesh();

        uint32_t getVertexCount() const;

        void setVertexCount(uint32_t value);

        //color32
        Bool32 hasColor32s() const;
        void addColor32s(const std::vector<Color32> &color32s);
        void removeColor32s();
        const std::vector<Color32> getColor32s() const;
        void setColor32s(const std::vector<Color32> &color32s);

        //color
        Bool32 hasColors() const;
        void addColors(const std::vector<Color> &colors);
        void removeColors();
        const std::vector<Color> getColors() const;
        void setColors(const std::vector<Color> &colors);

        //index
        virtual uint32_t getSubMeshCount() const override;

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

        //texture coordinate
        template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
        Bool32 hasTextureCoordinates() const;

        template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
        void addTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates);

        template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
        void removeTextureCoordinates();

        template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
        const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType getTextureCoordinates() const;

        template<TextureCoordinateType textureCoordinateType, TextureCoordinateIndex textureCoordinateIndex>
        void setTextureCoordinates(const typename MeshData::DataTypeInfo<TextureCoordinateConstInfo<textureCoordinateType>::ARRAY_TYPE>::ValueType &textureCoordinates);

        template<MeshData::DataType dataType>
        Bool32 hasData(const SlotType &slot) const;

        template<MeshData::DataType dataType>
        void addData(const SlotType &slot
            , const typename MeshData::DataTypeInfo<dataType>::ValueType &value
            );

        template<MeshData::DataType dataType>
        void removeData(const SlotType &slot);

        template<MeshData::DataType dataType>
        const typename MeshData::DataTypeInfo<dataType>::ValueType getData(const SlotType &slot) const;

        template <MeshData::DataType dataType>
        void setData(const SlotType &slot
            , const typename MeshData::DataTypeInfo<dataType>::ValueType &value
            );

    protected:
        uint32_t m_vertexCount;
        std::shared_ptr<MeshData> m_pData;
        uint32_t m_subMeshCount;
        std::vector<SubMeshInfo> m_subMeshInfos;

        Color m_multipliedColor;
        Color m_addedColor;

        Bool32 m_applied;
        uint32_t m_appliedVertexCount; //save vertex count to render.
        uint32_t m_appliedSubMeshCount;
        std::set<MeshData::DataInfo> m_layoutBindingInfos;
        std::vector<SubMeshInfo> m_usingSubMeshInfos; //save sub mesh info to render.

        void _createMeshData();

        void _createVertexData();

        void _createIndexData();


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
        using BaseValueType = typename MeshData::DataTypeInfo<ARRAY_DATA_TYPE>::BaseType;
        using ArrayValueType = typename MeshData::DataTypeInfo<ARRAY_DATA_TYPE>::ValueType;

        DimSepMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);

        virtual ~DimSepMesh();

        //position        
        Bool32 hasPositions() const;
        void addPositions(const ArrayValueType &vertices);
        void removePositions();
        const ArrayValueType getPositions() const;
        void setPositions(const ArrayValueType &vertices);

        //normal
        Bool32 hasNormals() const;
        void addNormals(const ArrayValueType &normals);
        void removeNormals();
        const ArrayValueType getNormals() const;
        void setNormals(const ArrayValueType &normals);

        //tangent
        Bool32 hasTangents() const;
        void addTangents(const ArrayValueType &tangents);
        void removeTangents();
        const ArrayValueType getTangents() const;
        void setTangents(const ArrayValueType &tangents);

        virtual void apply(Bool32 makeUnreadable) override;

        void setIsHasBounds(Bool32 isHasBounds);
        void setBounds(fd::Bounds<PointType> bounds);

    private:
        inline void _updateBounds();
    };

    template <MeshDimType meshDimType>
    class DimSimpleMesh : public InternalContentMesh, public Mesh<meshDimType>
    {
    public: 
        DimSimpleMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal);
        void setIsHasBounds(Bool32 isHasBounds);
        void setBounds(fd::Bounds<PointType> bounds);
    private:
    };

    template <MeshDimType meshDimType>
    class DimSharedContentMesh : public ExternalContentMesh, public Mesh<meshDimType>
    {
    public:
        DimSharedContentMesh();
        DimSharedContentMesh(std::shared_ptr<VertexData> pVertexData
            , std::shared_ptr<IndexData> pIndexData
            , uint32_t subIndexDataOffset
            , uint32_t subIndexDataCount
            );
        void setIsHasBounds(Bool32 isHasBounds);
        void setBounds(fd::Bounds<PointType> bounds);
    private:

    };
}

#include "graphics/mesh/mesh.inl"

#endif // !VG_MESH_H
