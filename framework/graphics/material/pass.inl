namespace vg
{
	template<typename T>
	void Pass::SpecializationData::init(const T &data
		, const vk::SpecializationInfo &info)
	{
		init(&data, static_cast<uin32_t>(sizeof(T)), info);
	}

	template<typename T>
	T Pass::SpecializationData::getData() const
	{
		size_t size = sizeof(T);
		T data;
		memcpy(&data, m_pData, size);
		return data;
	}

	template<typename T>
	void Pass::PushConstantUpdate::init(const T &data
		, vk::ShaderStageFlags stageFlags 
		, uint32_t offset)
	{
		init(&data, static_cast<uint32_t>(sizeof(T)), stageFlags, offset);
	}

    template<typename T>
	T Pass::PushConstantUpdate::getData() const
	{
		size_t size = sizeof(T);
		T data;
		memcpy(&data, m_pData, size);
		return data;
	}

	template <typename T>
	T Pass::getDataValue(std::string name) const
	{
		return m_pData->getDataValue<T>(name);
	}

	template <typename T>
	std::vector<T> Pass::getDataValue(std::string name, uint32_t count) const
	{
		return m_pData->getDataValue(name);
	}

	template<typename T>
	void Pass::setDataValue(std::string name
		, const T &value
		, uint32_t binding
		, DescriptorType descriptorType
		, ShaderStageFlags stageFlags
	)
	{
		m_pData->setDataValue(name, value);
		//update layout binding information.
		uint32_t descriptorCount = 1u;
		LayoutBindingInfo info(
			name,
			VG_FALSE,
			binding,
			descriptorType,
			descriptorCount,
			stageFlags
		);
		info.updateSize(m_pData.get());
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		m_applied = VG_FALSE;
		m_dataChanged = VG_TRUE;
	}

	template<typename T>
	void Pass::setDataValue(std::string name
		, const std::vector<T> &values
		, uint32_t binding
		, DescriptorType descriptorType
		, ShaderStageFlags stageFlags
	)
	{
		m_pData->setDataValue(name, values);
		//update layout binding information.
		uint32_t descriptorCount = values.size();
		LayoutBindingInfo info(
			name,
			VG_FALSE,
			binding,
			descriptorType,
			descriptorCount,
			stageFlags
		);
		info.updateSize(m_pData);
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		m_applied = VG_FALSE;
		m_dataChanged = VG_TRUE;
	}

	template<typename T>
	void Pass::setSpecializationData(ShaderStageFlagBits shaderStage
		, const T &data
		, const vk::SpecializationInfo &info)
	{
		setSpecializationData(shaderStage, &data, static_cast<uin32_t>(sizeof(T)), info);
	}

    template<typename T>
	void Pass::setPushConstantUpdate(std::string name
		, const T &data
		, vk::ShaderStageFlags stageFlags 
		, uint32_t offset)
	{
		std::shared_ptr<PushConstantUpdate> pPushConstantUpdate(new PushConstantUpdate());
		pPushConstantUpdate->init(data, stageFlags, offset);
		setValue(name, pPushConstantUpdate, m_mapPPushConstantUpdates, m_arrPushConstantUpdateNames);
	}

	template <typename T>
	void Pass::_updateBuildInData(BuildInDataType type, T data)
	{
		uint32_t count = static_cast<uint32_t>(BuildInDataType::COUNT);
		uint32_t offset1 = 0u;
		uint32_t i = 0u;
		for (i = 0; i < count; ++i)
		{
			if (i != static_cast<uint32_t>(type))
			{
				offset1 += buildInDataTypeSizes[i];
			}
			else
			{
				break;
			}
		}

		uint32_t offset2 = 0u;
		uint32_t componentCount = m_buildInDataInfo.componentCount;
		for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
		{
			Pass::BuildInDataType comType = (*(m_buildInDataInfo.pComponent + componentIndex)).type;
			if (type != comType)
			{
				offset2 += buildInDataTypeSizes[static_cast<uint32_t>(comType)];
			}
			else
			{
				break;
			}
		}

		if ( i < count)
		{
			size_t size = sizeof(T);
			if (memcmp((char *)(&m_buildInDataCache) + offset1, &data, size) != 0) {
				memcpy((char *)(&m_buildInDataCache) + offset1, &data, size);
				m_pData->setDataValue(VG_M_BUILDIN_NAME
				    , &data
					, size
					, offset2);
				m_applied = VG_FALSE;
				m_dataChanged = VG_TRUE;
			}
		}
	}
	
} //namespace kgs
