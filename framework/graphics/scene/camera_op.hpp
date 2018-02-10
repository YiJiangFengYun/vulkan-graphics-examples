#ifndef VG_CAMERA_OP_H
#define VG_CAMERA_OP_H

#include "graphics/scene/camera.hpp"

namespace vg
{
	template <SpaceType SPACE_TYPE>
	class CameraOP : public Camera<SPACE_TYPE>
	{
	public:
		CameraOP();
		void updateProj(BoundsType viewBounds);
		virtual void apply();
		typename TransformType::MatrixType getProjMatrix() const override;
		BoundsType getViewBounds() const;
		Bool32 isInView(Transform<SPACE_TYPE> *pTransform, BoundsType bounds, fd::Rect2D *viewRect = nullptr) const override;
	protected:
		BoundsType m_viewBounds;
		typename TransformType::MatrixType m_projMatrix;

	private:
		
	};
} //namespace kgs

#include "graphics/scene/camera_op.inl"

#endif // !VG_CAMERA_OP_H
