#ifndef VG_SCENE_H
#define VG_SCENE_H

#include <vector>
#include <unordered_map>
#include "graphics/util/util.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/scene/visual_object.hpp"
#include "graphics/scene/camera.hpp"
#include "graphics/scene/light.hpp"

namespace vg
{
	class BaseScene : public Base
	{
	public:
		BaseScene();
		SpaceType getSpaceType() const;
		Bool32 getIsRightHand() const;
		Bool32 getIsLeftHand() const;
		void setIsRightHand(Bool32 isRightHand);
		void setIsLeftHand(Bool32 isLeftHand);
	protected:
	    SpaceType m_spaceType;
		Bool32 m_isRightHand;
	};

	template <SpaceType SPACE_TYPE>
	class Scene : public BaseScene
	{
	public:
		using ObjectType = Object<SPACE_TYPE>;
		using VisualObjectType = VisualObject<SPACE_TYPE>;
		using CameraType = Camera<SPACE_TYPE>;
		using LightType = Light<SPACE_TYPE>;
		using TransformType = Transform<SPACE_TYPE>;

		const std::shared_ptr<TransformType> pRootTransformForVisualObject;
		const std::shared_ptr<TransformType> pRootTransformForCamera;
		const std::shared_ptr<TransformType> pRootTransformForLight;
		// const std::shared_ptr<TransformType> pRootTransform;


		Scene();

		uint32_t getVisualObjectCount() const;
		VisualObjectType *getVisualObjectWithIndex(uint32_t index) const;
		VisualObjectType *getVisualObjectWithTransform(const TransformType *pTransform) const;
		Bool32 isHasVisualObject(const VisualObjectType *pTarget) const;
		void addVisualObject(VisualObjectType *pTarget, VisualObjectType *pParent = nullptr);
		void removeVisualObject(VisualObjectType *pTarget);

		uint32_t getCameraCount() const;
		CameraType *getCameraWithIndex(uint32_t index) const;
		CameraType *getCameraWithTransform(const TransformType *pTransform) const;
		Bool32 isHasCamera(const CameraType *pTarget) const;
		void addCamera(CameraType *pTarget, CameraType *pParent = nullptr);
		void removeCamera(CameraType *pTarget);

		uint32_t getLightCount() const;
		LightType *getLightWithIndex(uint32_t index) const;
		LightType *getLightWithTransform(const TransformType *pTransform) const;
		Bool32 isHasLight(const LightType *pTarget) const;
		void addLight(LightType *pTarget, LightType *pParent = nullptr);
		void removeLight(LightType *pTarget);


	protected:

		//aggregations
		std::vector<VisualObjectType *> m_arrPVisualObjects;
		std::unordered_map<InstanceID, VisualObjectType *> m_mapPVisualObjects;
		std::unordered_map<InstanceID, VisualObjectType *> m_mapTransformIdToVisualObjects;
		std::vector<CameraType *> m_arrPCameras;
		std::unordered_map<InstanceID, CameraType *> m_mapPCameras;
		std::unordered_map<InstanceID, CameraType *> m_mapTransformIdToCameras;
		std::vector<LightType *> m_arrPLights;
		std::unordered_map<InstanceID, LightType *> m_mapPLights;
		std::unordered_map<InstanceID, LightType *> m_mapTransformIdToLights;

		virtual void _addVisualObject(VisualObjectType *pTarget
			, VisualObjectType *pParent);
		virtual void _removeVisualObject(VisualObjectType *pTarget);
	private:
		template <typename T>
		inline Bool32 _isHasObject(const T *pTarget
			, const std::unordered_map<InstanceID, T *> &map
		) const;

		template <typename T>
		inline void _addObject(T *pTarget
			, std::vector<T *> &arr
			, std::unordered_map<InstanceID, T *> &map
			, std::unordered_map<InstanceID, T *> &mapTransformToObjs
			, TransformType *root
			, T *pParent = nullptr
		);

		template <typename T>
		inline void _removeObject(T *pTarget
			, std::vector<T *> &arr
			, std::unordered_map<InstanceID, T *> &map
			, std::unordered_map<InstanceID, T *> &mapTransformToObjs
		);
	};
} //namespace kgs

#include "graphics/scene/scene.inl"

#endif // !VG_SCENE_H
