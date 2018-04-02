#ifndef VG_SCENE_2_H
#define VG_SCENE_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"

namespace vg
{
	class Scene2 : public Scene<SpaceType::SPACE_2>
	{
	public:
		Scene2();
		virtual Matrix4x4 getProjMatrix(const CameraType *pCamera) const override;
		virtual Bool32 isInView(const CameraType *pCamera
		    , TransformType *pTransform
		    , BoundsType bounds
			, fd::Rect2D *viewRect = nullptr) const override;
	private:
	};

} //namespace kgs

#endif // !VG_SCENE_2_H
