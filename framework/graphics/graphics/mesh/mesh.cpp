#include "graphics/mesh/mesh.hpp"

#include <cstdlib>

namespace vg
{
//BaseMesh
	BaseMesh::BaseMesh()
	    : Base(BaseType::MESH)
	{

	}

	void BaseMesh::beginRecord() const
	{
		_beginRecord();
	}
		
	void BaseMesh::endRecord() const
	{
		_endRecord();
	}

	void BaseMesh::_beginRecord() const
	{

	}
		
	void BaseMesh::_endRecord() const
	{

	}


//Template Mesh
    template<MeshDimType meshDimType>
	Mesh<meshDimType>::Mesh()
		: DimensionMesh()
		, m_meshDimType(meshDimType)
		, m_hasBounds(VG_FALSE)
		, m_bounds()
	{
	}

	template<MeshDimType meshDimType>
	MeshDimType Mesh<meshDimType>::getMeshDimType() const
	{
		return m_meshDimType;
	}

	template<MeshDimType meshDimType>
	Bool32 Mesh<meshDimType>::getIsHasBounds() const
	{
		return m_hasBounds;
	}

	template<MeshDimType meshDimType>
	fd::Bounds<typename Mesh<meshDimType>::PointType> Mesh<meshDimType>::getBounds() const
	{
		return m_bounds;
	}

	//template instantiation
	template class Mesh<MeshDimType::SPACE_2>;
	template class Mesh<MeshDimType::SPACE_3>;


//ContentMesh
	ContentMesh::ContentMesh()
	    : m_pVertexData(nullptr)
		, m_pIndexData(nullptr)
	{

	}

	ContentMesh::~ContentMesh()
	{

	}

	uint32_t ContentMesh::getSubMeshOffset() const
	{
		return 0u;
	}

	uint32_t ContentMesh::getSubMeshCount() const
	{
		return m_pIndexData->getSubIndexDataCount();
	}

	VertexData *ContentMesh::getVertexData() const
	{
		return m_pVertexData.get();
	}

	IndexData *ContentMesh::getIndexData() const
	{
		return m_pIndexData.get();
	}

	
//InternalContentMesh
	InternalContentMesh::InternalContentMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
	    : ContentMesh()
	{
		m_pVertexData = std::shared_ptr<VertexData>(new VertexData(bufferMemoryPropertyFlags));
		m_pIndexData = std::shared_ptr<IndexData>(new IndexData(bufferMemoryPropertyFlags));
	}


//ExternalContentMesh
	ExternalContentMesh::ExternalContentMesh()
	    : ContentMesh()
		, m_subIndexDataOffset(0u)
		, m_subIndexDataCount(0u)
	{

	}

	ExternalContentMesh::ExternalContentMesh(
		std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
	)
	    : ContentMesh()
		, m_subIndexDataOffset(subIndexDataOffset)
		, m_subIndexDataCount(subIndexDataCount)
	{
		m_pVertexData = pVertexData;
		m_pIndexData = pIndexData;
	}

	void ExternalContentMesh::init(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			)
	{
		m_pVertexData = pVertexData;
		m_pIndexData = pIndexData;
		m_subIndexDataOffset = subIndexDataOffset;
		m_subIndexDataCount = subIndexDataCount;
	}

	uint32_t ExternalContentMesh::getSubMeshOffset() const
	{
		return m_subIndexDataOffset;
	}

	uint32_t ExternalContentMesh::getSubMeshCount() const
	{
		return m_subIndexDataCount;
	}

	uint32_t ExternalContentMesh::getSubIndexDataOffset() const
    {
		return m_subIndexDataOffset;
	}

	void ExternalContentMesh::setSubIndexDataOffset(uint32_t offset)
	{
		m_subIndexDataOffset = offset;
	}

	uint32_t ExternalContentMesh::getSubIndexDaTaCount() const
	{
		return m_subIndexDataCount;
	}

	void ExternalContentMesh::setSubIndexDataCount(uint32_t count)
	{
		m_subIndexDataCount = count;
	}

//SepMesh
	SepMesh::SepMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
		: InternalContentMesh(bufferMemoryPropertyFlags)
		, m_vertexCount(0u)
		, m_pData()
		, m_subMeshCount()
		, m_subMeshInfos()
		, m_multipliedColor(COLOR_WHITE) //default multiplied color should be (1, 1, 1, 1)
		, m_addedColor()
		, m_applied(VG_FALSE)
		, m_appliedVertexCount(0u)
		, m_appliedSubMeshCount(0u)
		, m_layoutBindingInfos()
		, m_usingSubMeshInfos()
	{
		_createMeshData();
		setSubMeshCount(1u);
	}

	SepMesh::~SepMesh()
	{

	}

	uint32_t SepMesh::getVertexCount() const
	{
		return m_vertexCount;
	}

	void SepMesh::setVertexCount(uint32_t value)
	{
		m_vertexCount = value;
		m_applied = VG_FALSE;
	}

    Bool32 SepMesh::hasColor32s() const
	{
		return hasData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME);
	}
		
	void SepMesh::addColor32s(const std::vector<Color32> &color32s)
	{
		addData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME, color32s, VG_VERTEX_BINDING_PRIORITY_COLOR);
	}
		
	void SepMesh::removeColor32s()
	{
		removeData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME);
	}
		
	const std::vector<Color32> SepMesh::getColor32s() const
	{
		return getData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME);
	}
		
	void SepMesh::setColor32s(const std::vector<Color32> &color32s)
	{
		setData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME, color32s);
	}

	Bool32 SepMesh::hasColors() const
	{
		return hasData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME);
	}
		
	void SepMesh::addColors(const std::vector<Color> &colors)
	{
		addData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME, colors, VG_VERTEX_BINDING_PRIORITY_COLOR);
	}
		
	void SepMesh::removeColors()
	{
		removeData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME);
	}

	const std::vector<Color> SepMesh::getColors() const
	{
		return getData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME);
	}

	void SepMesh::setColors(const std::vector<Color> &colors)
	{
		setData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME, colors);
	}

	uint32_t SepMesh::getSubMeshCount() const
	{
		return m_subMeshCount;
	}

	void SepMesh::setSubMeshCount(uint32_t value)
	{
		m_subMeshCount = value;
		if (static_cast<uint32_t>(m_subMeshInfos.size()) < value)
		{
			m_subMeshInfos.resize(value);
		}
		m_applied = VG_FALSE;
	}

	const std::vector<uint32_t> &SepMesh::getIndices(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].indices;
	}

	void SepMesh::setIndices(const std::vector<uint32_t> &indices, PrimitiveTopology topology, uint32_t subMeshIndex)
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		SubMeshInfo& subMeshInfo = m_subMeshInfos[subMeshIndex];
		subMeshInfo.topology = topology;
		subMeshInfo.indices = indices;
		m_applied = VG_FALSE;
	}

	uint32_t SepMesh::getIndexCount(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return _getIndexCount(subMeshIndex);
	}

	uint32_t SepMesh::getIndexStart(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		uint32_t start = 0;
		for (uint32_t i = 0; i < subMeshIndex; ++i)
		{
			start += _getIndexCount(i);
		}
		return start;
	}

	PrimitiveTopology SepMesh::getTopology(uint32_t subMeshIndex) const
	{
#ifdef DEBUG
		_verifySubMeshIndex(subMeshIndex);
#endif // DEBUG
		return m_subMeshInfos[subMeshIndex].topology;
	}

	Color SepMesh::getMultipliedColor() const
	{
		return m_multipliedColor;
	}

	void SepMesh::setMultipliedColor(Color value)
	{
		m_multipliedColor = value;
		// m_applied = VG_FALSE;
	}

	Color SepMesh::getAddedColor() const
	{
		return m_addedColor;
	}

	void SepMesh::setAddedColor(Color value)
	{
		m_addedColor = value;
		// m_applied = VG_FALSE;
	}

	void SepMesh::apply(Bool32 makeUnreadable)
	{
		if (m_applied == VG_FALSE)
		{
			m_appliedVertexCount = m_vertexCount;

			m_usingSubMeshInfos = m_subMeshInfos;
			m_appliedSubMeshCount = m_subMeshCount;

			//sort layout binding infos
			_sortLayoutBindingInfos();

			//create vertex buffer
			_createVertexData();

			//create index buffer
			_createIndexData();

			m_applied = VG_TRUE;
		}

		if (makeUnreadable)
		{
			m_vertexCount = 0u;

			m_subMeshCount = 0u;

			//clear data with new mesh data.
			_createMeshData();
			//clear layout binding info array with reallocate.
			// std::vector<std::string>().swap(m_arrLayoutBindingInfoNames);
			//clear layout binding info map with reallocate.
			// std::unordered_map<std::string, LayoutBindingInfo>().swap(m_mapLayoutBindingInfos);
			//clear sub mesh info with reallocate.
			std::vector<SubMeshInfo>().swap(m_subMeshInfos);
			//reset sub mesh count to 1.
			setSubMeshCount(1u);
		}
	}

	void SepMesh::_createMeshData()
	{
		m_pData = std::shared_ptr<MeshData>(new MeshData());
	}

	void SepMesh::_createVertexData()
	{
		auto vertexCount = m_appliedVertexCount;
		vk::PipelineVertexInputStateCreateInfo createInfo;
		//get size of every vertex
		uint32_t size = 0u;
		for (const auto& layoutInfo : m_layoutBindingInfos)
		{
			size += MeshData::getDataBaseSize(layoutInfo.dataType);
		}
		//get vertex buffer size.
		uint32_t vertexBufferSize = size * vertexCount;
		std::vector<vk::VertexInputBindingDescription> bindingdescs(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			bindingdescs[index].binding = index;
			bindingdescs[index].stride = MeshData::getDataBaseSize(info.dataType);
			bindingdescs[index].inputRate = vk::VertexInputRate::eVertex;
			++index;
		}

		std::vector<vk::VertexInputAttributeDescription> attriDescs(m_layoutBindingInfos.size());
		index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			attriDescs[index].binding = index;
			attriDescs[index].location = index;
			attriDescs[index].format = MeshData::getBaseFormatWithDataType(info.dataType);
			attriDescs[index].offset = 0u;
			++index;
		}

		createInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingdescs.size());
        createInfo.pVertexBindingDescriptions = bindingdescs.data();
        createInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attriDescs.size());
        createInfo.pVertexAttributeDescriptions = attriDescs.data();

		void *stagingMemory = malloc(vertexBufferSize);
		uint32_t offset = 0u;
		for (const auto& layoutInfo : m_layoutBindingInfos)
		{
			m_pData->memoryCopyData(layoutInfo.dataType, layoutInfo.name, stagingMemory, offset, 0u, vertexCount);
			offset += MeshData::getDataBaseSize(layoutInfo.dataType) * vertexCount;
		}

		m_pVertexData->init(vertexCount, stagingMemory, vertexBufferSize, VG_FALSE, createInfo);

		free(stagingMemory);
	}

	void SepMesh::_createIndexData()
	{
		//get index buffer size
		uint32_t subCount = m_usingSubMeshInfos.size();
		uint32_t indexBufferSize = 0u;		
		std::vector<IndexData::SubIndexData> subDatas(subCount);
		for (uint32_t i = 0; i < subCount; ++i) {
			const std::vector<uint32_t>& indices = m_usingSubMeshInfos[i].indices;
			uint32_t subBufferSize = static_cast<uint32_t>(indices.size() * sizeof(uint32_t));
			subDatas[i].indexType = vk::IndexType::eUint32;
			subDatas[i].bufferSize = subBufferSize;
			subDatas[i].indexCount = indices.size();
			subDatas[i].inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;
			subDatas[i].inputAssemblyStateInfo.topology = tranPrimitiveTopologyTypeToVK(m_usingSubMeshInfos[i].topology);
			indexBufferSize += subBufferSize;
		}

        void *stagingMemory = malloc(indexBufferSize);
		uint32_t offset = 0u;
		for (const auto& subMeshInfo : m_usingSubMeshInfos)
		{
			const std::vector<uint32_t>& indices = subMeshInfo.indices;
			size_t size = indices.size() * sizeof(uint32_t);
			memcpy((char*)stagingMemory + offset, indices.data(), size);
			offset += static_cast<uint32_t>(size);
		}

		m_pIndexData->init(subCount, subDatas.data(), stagingMemory, indexBufferSize, VG_FALSE);

		free(stagingMemory);
	}

	void SepMesh::_sortLayoutBindingInfos()
	{
		m_layoutBindingInfos.clear();
		
		const auto &datas = m_pData->datas;
	    uint32_t dataSize = datas.size();
		for (uint32_t i = 0; i < dataSize; ++i) {
			const auto &data = datas[i];
			const auto &arrNames = data.arrDataNames;
			for (const auto& name : arrNames) {
				const auto &dataInfo = data.getDataInfo(name);
				m_layoutBindingInfos.insert(dataInfo);
			}
		}
	}

#ifdef DEBUG
	void SepMesh::_verifySubMeshIndex(uint32_t subMeshIndex) const
	{
		if (subMeshIndex >= m_subMeshCount)
			throw std::range_error("The subMeshIndex out of range of the actual sub mesh count.");
	}
#endif // DEBUG

	uint32_t SepMesh::_getIndexCount(uint32_t subMeshIndex) const
	{
		SubMeshInfo subMeshInfo = m_subMeshInfos[subMeshIndex];
		return static_cast<uint32_t>(subMeshInfo.indices.size());
	}


//DimSepMesh
	template <MeshDimType meshDimType>
	DimSepMesh<meshDimType>::DimSepMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags) 
		: SepMesh(bufferMemoryPropertyFlags)
		, Mesh<meshDimType>()
	{
		m_hasBounds = VG_TRUE;
	}

	template <MeshDimType meshDimType>
	DimSepMesh<meshDimType>::~DimSepMesh()
	{

	}

	template <MeshDimType meshDimType>
	Bool32 DimSepMesh<meshDimType>::hasPositions() const
	{
		return hasData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::addPositions(const ArrayValueType &vertices)
	{
		addData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME, vertices, VG_VERTEX_BINDING_PRIORITY_POSITION);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::removePositions()
	{
		removeData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType DimSepMesh<meshDimType>::getPositions() const
	{
		return getData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setPositions(const ArrayValueType &vertices)
	{
		setData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME, vertices);
	}

	template <MeshDimType meshDimType>
	Bool32 DimSepMesh<meshDimType>::hasNormals() const
	{
		return hasData<ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME);
	}
	
	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::addNormals(const ArrayValueType &normals)
	{
		addData<ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME, normals, VG_VERTEX_BINDING_PRIORITY_NORMAL);
	}
	
	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::removeNormals()
	{
		removeData<ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME);
	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType DimSepMesh<meshDimType>::getNormals() const
	{
		return getData<ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setNormals(const ArrayValueType &normals)
	{
		setData<ARRAY_DATA_TYPE>(VG_VERTEX_NORMAL_NAME, normals);
	}

	template <MeshDimType meshDimType>
	Bool32 DimSepMesh<meshDimType>::hasTangents() const
	{
		return hasData<ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::addTangents(const ArrayValueType &tangents)
	{
         addData<ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME, tangents, VG_VERTEX_BINDING_PRIORITY_TANGENT);
	}
	
	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::removeTangents()
	{
		removeData<ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME);
	}

	template <MeshDimType meshDimType>
	const typename DimSepMesh<meshDimType>::ArrayValueType DimSepMesh<meshDimType>::getTangents() const
	{
		return getData<ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setTangents(const ArrayValueType &tangents)
	{
		setData<ARRAY_DATA_TYPE>(VG_VERTEX_TANGENT_NAME, tangents);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::apply(Bool32 makeUnreadable)
	{
		//caculate bounds
		_updateBounds();

		SepMesh::apply(makeUnreadable);
	}

	template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setIsHasBounds(Bool32 isHasBounds)
	{
		m_hasBounds = isHasBounds;
	}

    template <MeshDimType meshDimType>
	void DimSepMesh<meshDimType>::setBounds(fd::Bounds<PointType> bounds)
	{
		m_bounds = bounds;
	}

	template <MeshDimType meshDimType>
	inline void DimSepMesh<meshDimType>::_updateBounds()
	{
		if (m_vertexCount == 0u)
		{
			m_bounds = {PointType(0), PointType(0)};
			return;
		}

		auto vertices = m_pData->getData<ARRAY_DATA_TYPE>(VG_VERTEX_POSITION_NAME);
		BaseValueType minPos;
		BaseValueType maxPos;
		BaseValueType::length_type len = BaseValueType::length();
		for (BaseValueType::length_type i = 0; i < len; ++i)
		{
			typename BaseValueType::value_type min = std::numeric_limits<typename BaseValueType::value_type>::max(), max = -std::numeric_limits<typename BaseValueType::value_type>::max();
			for (uint32_t j = 0; j < m_vertexCount && j < static_cast<uint32_t>(vertices.size()); ++j)
			{
				if (min > vertices[j][i])min = vertices[j][i];
				if (max < vertices[j][i])max = vertices[j][i];
			}
			minPos[i] = min;
			maxPos[i] = max;
		}
		m_bounds.setMinMax(minPos, maxPos);
	}

	//template instantiation
	template class DimSepMesh<MeshDimType::SPACE_2>;
	template class DimSepMesh<MeshDimType::SPACE_3>;
	

//DimSimpleMesh
	template <MeshDimType meshDimType>
	DimSimpleMesh<meshDimType>::DimSimpleMesh(vk::MemoryPropertyFlags bufferMemoryPropertyFlags)
	    : InternalContentMesh(bufferMemoryPropertyFlags)
		, Mesh<meshDimType>()
	{

	}

	template <MeshDimType meshDimType>
	void DimSimpleMesh<meshDimType>::setIsHasBounds(Bool32 isHasBounds)
	{
		m_hasBounds = isHasBounds;
	}

    template <MeshDimType meshDimType>
	void DimSimpleMesh<meshDimType>::setBounds(fd::Bounds<PointType> bounds)
	{
		m_bounds = bounds;
	}

	//template instantiation
	template class  DimSimpleMesh<MeshDimType::SPACE_2>;
	template class  DimSimpleMesh<MeshDimType::SPACE_3>;


//DimSharedContentMesh
    template <MeshDimType meshDimType>
	DimSharedContentMesh<meshDimType>::DimSharedContentMesh()
	    : ExternalContentMesh()
		, Mesh<meshDimType>()
	{

	}

	template <MeshDimType meshDimType>
	DimSharedContentMesh<meshDimType>::DimSharedContentMesh(std::shared_ptr<VertexData> pVertexData
		    , std::shared_ptr<IndexData> pIndexData
			, uint32_t subIndexDataOffset
			, uint32_t subIndexDataCount
			)
	{

	}

	template <MeshDimType meshDimType>
	void DimSharedContentMesh<meshDimType>::setIsHasBounds(Bool32 isHasBounds)
	{
		m_hasBounds = isHasBounds;
	}

    template <MeshDimType meshDimType>
	void DimSharedContentMesh<meshDimType>::setBounds(fd::Bounds<PointType> bounds)
	{
		m_bounds = bounds;
	}

	//template instantiation
	template class DimSharedContentMesh<MeshDimType::SPACE_2>;
	template class DimSharedContentMesh<MeshDimType::SPACE_3>;
}