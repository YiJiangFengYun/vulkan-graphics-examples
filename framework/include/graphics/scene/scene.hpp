#ifndef KGS_SCENE_H
#define KGS_SCENE_H

#include <vector>
#include <unordered_map>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Scene
	{
	public:
		typedef Object<SPACE_TYPE> ObjectType;
		typedef VisualObject<SPACE_TYPE> VisualObjectType;
		typedef Camera<SPACE_TYPE> CameraType;
		typedef Light<SPACE_TYPE> LightType;

		uint32_t getVisualObjectCount()
		{
			return m_arrPVisualObjects.size();
		}

		std::shared_ptr<VisualObjectType> getVisualObjectWithIndex(uint32_t index)
		{
			return m_arrPVisualObjects[index];
		}

		std::shared_ptr<VisualObjectType> getVisualObjectWithName(std::string name)
		{
			getValue(name, m_mapPVisualObjects, m_arrPVisualObjects);
		}

		void setVisualObjectWithName(std::string name, std::shared_ptr<VisualObjectType> value)
		{
			setValue(name, value, m_mapPVisualObjects, m_arrPVisualObjects);
		}

		uint32_t getCameraCount()
		{
			return m_arrPCameras.size();
		}

		std::shared_ptr<CameraType> getCameraWithIndex(uint32_t index)
		{
			return m_arrPCameras[index];
		}

		std::shared_ptr<CameraType> getCameraWithName(std::string name)
		{
			getValue(name, m_mapPCameras, m_arrPCameras);
		}

		void setCameraWithName(std::string name, std::shared_ptr<CameraType> value)
		{
			setValue(name, value, m_mapPCameras, m_arrPCameras);
		}

		uint32_t getLightCount()
		{
			return m_arrPLights.size();
		}

		std::shared_ptr<LightType> getLightWithIndex(uint32_t index)
		{
			return m_arrPLights[index];
		}

		std::shared_ptr<LightType> getLightWithName(std::string name)
		{
			getValue(name, m_mapPLights, m_arrPLights);
		}

		void setLightWithName(std::string name, std::shared_ptr<LightType> value)
		{
			setValue(name, value, m_mapPLights, m_arrPLights);
		}

	protected:

		//aggregations
		std::vector<std::shared_ptr<VisualObjectType>> m_arrPVisualObjects;
		std::unordered_map<std::string, std::shared_ptr<VisualObjectType>> m_mapPVisualObjects;
		std::vector<std::shared_ptr<CameraType>> m_arrPCameras;
		std::unordered_map<std::string, std::shared_ptr<CameraType>> m_mapPCameras;
		std::vector<std::shared_ptr<LightType>> m_arrPLights;
		std::unordered_map<std::string, std::shared_ptr<LightType>> m_mapPLights;

	};
} //namespace kgs

#endif // !KGS_SCENE_H
