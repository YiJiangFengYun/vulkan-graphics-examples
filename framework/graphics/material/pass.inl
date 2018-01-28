namespace vg
{
	template<typename T>
	void Pass::SpecializationData::init(const T &data
		, const vk::SpecializationInfo &info)
	{
		init(&data, stati_cast<uin32_t>(sizeof(T)), info);
	}

	template<typename T>
	T Pass::SpecializationData::getData() const
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
		info.updateSize(m_pData);
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
		m_applied = VG_FALSE;
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
	}

	template<typename T>
	void Pass::setSpecializationData(ShaderStageFlagBits shaderStage
		, const T &data
		, const vk::SpecializationInfo &info)
	{
		setSpecializationData(shaderStage, &data, static_cast<uin32_t>(sizeof(T)), info);
	}
} //namespace kgs
