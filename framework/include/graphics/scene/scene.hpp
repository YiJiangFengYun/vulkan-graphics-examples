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
	class BaseScene
	{
	public:

	protected:

	};

	template <SpaceType SPACE_TYPE>
	class Scene : public BaseScene
	{
	public:
		typedef Object<SPACE_TYPE> ObjectType;
		typedef VisualObject<SPACE_TYPE> VisualObjectType;
		typedef Camera<SPACE_TYPE> CameraType;
		typedef Light<SPACE_TYPE> LightType;

		Scene();

		uint32_t getVisualObjectCount();

		const std::shared_ptr<VisualObjectType> &getVisualObjectWithIndex(uint32_t index) const;

		const std::shared_ptr<VisualObjectType> &getVisualObjectWithName(std::string name) const;

		void setVisualObjectWithName(std::string name, const std::shared_ptr<VisualObjectType> &value);

		uint32_t getCameraCount();

		const std::shared_ptr<CameraType> &getCameraWithIndex(uint32_t index) const;

		const std::shared_ptr<CameraType> &getCameraWithName(std::string name) const;

		void setCameraWithName(std::string name, const std::shared_ptr<CameraType> &value);

		uint32_t getLightCount();

		const std::shared_ptr<LightType> &getLightWithIndex(uint32_t index) const;

		const std::shared_ptr<LightType> &getLightWithName(std::string name) const;

		void setLightWithName(std::string name, const std::shared_ptr<LightType> &value);

	protected:

		//aggregations
		std::vector<std::string> m_arrVisualObjectNames;
		std::unordered_map<std::string, std::shared_ptr<VisualObjectType>> m_mapPVisualObjects;
		std::vector<std::string> m_arrCameraNames;
		std::unordered_map<std::string, std::shared_ptr<CameraType>> m_mapPCameras;
		std::vector<std::string> m_arrLightNames;
		std::unordered_map<std::string, std::shared_ptr<LightType>> m_mapPLights;

	};
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !KGS_SCENE_H
