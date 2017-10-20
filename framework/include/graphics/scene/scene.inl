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
		return static_cast<uint32_t>(m_arrPVisualObjects.size());
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::VisualObjectType> &Scene<SPACE_TYPE>::getVisualObjectWithIndex(uint32_t index) const
	{
		return m_arrPVisualObjects[index];
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::VisualObjectType> &Scene<SPACE_TYPE>::getVisualObjectWithName(std::string name) const
	{
		getValue(name, m_mapPVisualObjects, m_arrPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setVisualObjectWithName(std::string name, const std::shared_ptr<VisualObjectType> &value)
	{
		setValue(name, value, m_mapPVisualObjects, m_arrPVisualObjects);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getCameraCount()
	{
		return m_arrPCameras.size();
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::CameraType> &Scene<SPACE_TYPE>::getCameraWithIndex(uint32_t index) const
	{
		return m_arrPCameras[index];
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::CameraType> &Scene<SPACE_TYPE>::getCameraWithName(std::string name) const
	{
		getValue(name, m_mapPCameras, m_arrPCameras);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setCameraWithName(std::string name, const std::shared_ptr<CameraType> &value)
	{
		setValue(name, value, m_mapPCameras, m_arrPCameras);
	}

	template <SpaceType SPACE_TYPE>
	uint32_t Scene<SPACE_TYPE>::getLightCount()
	{
		return m_arrPLights.size();
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::LightType> &Scene<SPACE_TYPE>::getLightWithIndex(uint32_t index) const
	{
		return m_arrPLights[index];
	}

	template <SpaceType SPACE_TYPE>
	const std::shared_ptr<typename Scene<SPACE_TYPE>::LightType> &Scene<SPACE_TYPE>::getLightWithName(std::string name) const
	{
		getValue(name, m_mapPLights, m_arrPLights);
	}

	template <SpaceType SPACE_TYPE>
	void Scene<SPACE_TYPE>::setLightWithName(std::string name, const std::shared_ptr<LightType> &value)
	{
		setValue(name, value, m_mapPLights, m_arrPLights);
	}
} //namespace kgs