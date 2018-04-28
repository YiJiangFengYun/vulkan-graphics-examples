#include "graphics/mesh/mesh.hpp"

#include <cstdlib>

namespace vg
{
	BaseMesh::BaseMesh()
	    : Base(BaseType::MESH)
	{

	}

	BaseMesh::~BaseMesh()
	{
		
	}

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

	

	InternalContentMesh::InternalContentMesh()
	    : ContentMesh()
	{
		m_pVertexData = std::shared_ptr<VertexData>(new VertexData());
		m_pIndexData = std::shared_ptr<IndexData>(new IndexData());
	}

	InternalContentMesh::InternalContentMesh(MemoryPropertyFlags bufferMemoryPropertyFlags)
	    : ContentMesh()
	{
		m_pVertexData = std::shared_ptr<VertexData>(new VertexData(bufferMemoryPropertyFlags));
		m_pIndexData = std::shared_ptr<IndexData>(new IndexData(bufferMemoryPropertyFlags));
	}


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


	SepMesh::LayoutBindingInfo::LayoutBindingInfo()
	{

	}

	SepMesh::LayoutBindingInfo::LayoutBindingInfo(std::string name,
		MeshData::DataType dataType,
		uint32_t bindingPriority)
		: name(name)
		, dataType(dataType)
		, bindingPriority(bindingPriority)
	{

	}

	SepMesh::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo &target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

	SepMesh::LayoutBindingInfo &SepMesh::LayoutBindingInfo::operator=(const LayoutBindingInfo &target)
	{
		name = target.name;
		dataType = target.dataType;
		bindingPriority = target.bindingPriority;
		return *this;
	}


	SepMesh::LayoutBindingInfo::LayoutBindingInfo(const LayoutBindingInfo &&target)
		: name(target.name)
		, dataType(target.dataType)
		, bindingPriority(target.bindingPriority)
	{

	}

	Bool32 SepMesh::LayoutBindingInfo::operator ==(const LayoutBindingInfo &target) const
	{
		return name == target.name && dataType == target.dataType && bindingPriority == target.bindingPriority;
	}

	Bool32 SepMesh::LayoutBindingInfo::operator<(const LayoutBindingInfo &target) const
	{
		return bindingPriority < target.bindingPriority;
	}

	SepMesh::SepMesh()
		: InternalContentMesh()
		, m_vertexCount(0u)
		, m_pData()
	    , m_arrLayoutBindingInfoNames()
	    , m_mapLayoutBindingInfos()
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

	SepMesh::SepMesh(MemoryPropertyFlags bufferMemoryPropertyFlags)
		: InternalContentMesh(bufferMemoryPropertyFlags)
		, m_vertexCount(0u)
		, m_pData()
		, m_arrLayoutBindingInfoNames()
		, m_mapLayoutBindingInfos()
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

	const std::vector<Color32> &SepMesh::getColor32s() const
	{
		return _getData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME);
	}

	void SepMesh::setColor32s(const std::vector<Color32> &colors)
	{
		_setData<MeshData::DataType::COLOR_32_ARRAY>(VG_VERTEX_COLOR_NAME, colors, VG_VERTEX_BINDING_PRIORITY_COLOR);
	}

	const std::vector<Color> &SepMesh::getColors() const
	{
		return _getData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME);
	}

	void SepMesh::setColors(const std::vector<Color> &colors)
	{
		_setData<MeshData::DataType::COLOR_ARRAY>(VG_VERTEX_COLOR_NAME, colors, VG_VERTEX_BINDING_PRIORITY_COLOR);
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
			std::vector<std::string>().swap(m_arrLayoutBindingInfoNames);
			//clear layout binding info map with reallocate.
			std::unordered_map<std::string, LayoutBindingInfo>().swap(m_mapLayoutBindingInfos);
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
			size += MeshData::getDataBaseTypeSize(layoutInfo.dataType);
		}
		//get vertex buffer size.
		uint32_t vertexBufferSize = size * vertexCount;
		std::vector<vk::VertexInputBindingDescription> bindingdescs(m_layoutBindingInfos.size());
		uint32_t index = 0u;
		for (const auto& info : m_layoutBindingInfos)
		{
			bindingdescs[index].binding = index;
			bindingdescs[index].stride = MeshData::getDataBaseTypeSize(info.dataType);
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
			m_pData->memCopyDataValue(layoutInfo.name, layoutInfo.dataType, stagingMemory, offset, 0u, vertexCount);
			offset += MeshData::getDataBaseTypeSize(layoutInfo.dataType) * vertexCount;
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
}