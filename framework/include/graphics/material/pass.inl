namespace kgs
{
	template <MaterialData::DataType dataType>
	const typename MaterialData::DataTypeInfo<dataType>::ValueType Pass::getData(std::string name) const
	{
		return m_pData->getDataValue<dataType>(name);
	}

	template <MaterialData::DataType dataType>
	void Pass::setData(std::string name
		, const typename MaterialData::DataTypeInfo<dataType>::ValueType value
		, uint32_t binding
		, DescriptorType descriptorType
		, ShaderStageFlags stageFlags
	)
	{
		m_pData->setDataValue<dataType>(name, value);
		//update layout binding information.
		uint32_t descriptorCount = static_cast<uint32_t>(sizeof(value) / sizeof(typename MaterialData::DataTypeInfo<dataType>::BaseType));
		LayoutBindingInfo info(
			name,
			dataType,
			binding,
			descriptorType,
			descriptorCount,
			stageFlags
		);
		setValue(name, info, m_mapLayoutBinds, m_arrLayoutBindNames);
	}
} //namespace kgs
