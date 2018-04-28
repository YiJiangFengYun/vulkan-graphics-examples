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
		typename TransformType::MatrixType getProjMatrix() const override;
		BoundsType getViewBounds() const;
	protected:
		BoundsType m_viewBounds;
		typename TransformType::MatrixType m_projMatrix;
		virtual void _applyProj() = 0;
	private:
		
	};
} //namespace kgs

#include "graphics/scene/camera_op.inl"

#endif // !VG_CAMERA_OP_H
