namespace kgs
{
	template <SpaceType SPACE_TYPE>
	Scene<SPACE_TYPE>::Scene()
		: BaseScene()
	{

	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getVisualObjectCount()
	{
		return static_cast<uint32_t>(m_arrVisualObjectNames.size());
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::VisualObjectType> &Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
	{
		auto name = m_arrVisualObjectNames[index];
		return getValue(name, m_mapPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::VisualObjectType> &Scene<SPACE_TYPE>::getVisualObjectWithName(std::string name) const
	{
		return getValue(name, m_mapPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setVisualObjectWithName(std::string name, const std::shared_ptr<VisualObjectType> &value)
	{
		setValue(name, value, m_mapPVisualObjects, m_arrVisualObjectNames);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getCameraCount()
	{
		return m_arrCameraNames.size();
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::CameraType> &Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
	{
		auto name = m_arrCameraNames[index];
		return getValue(name, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::CameraType> &Scene<SPACE_TYPE>::getCameraWithName(std::string name) const
	{
		return getValue(name, m_mapPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setCameraWithName(std::string name, const std::shared_ptr<CameraType> &value)
	{
		setValue(name, value, m_mapPCameras, m_arrCameraNames);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getLightCount()
	{
		return m_arrLightNames.size();
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::LightType> &Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
	{
		auto name = m_arrLightNames[index];
		return getValue(name, m_mapPLights);
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::LightType> &Scene<SPACE_TYPE>::getLightWithName(std::string name) const
	{
		return getValue(name, m_mapPLights);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setLightWithName(std::string name, const std::shared_ptr<LightType> &value)
	{
		setValue(name, value, m_mapPLights, m_arrLightNames);
	}
} //namespace kgs