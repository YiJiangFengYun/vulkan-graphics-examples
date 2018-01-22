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
		typename TransformType::MatrixType getProjMatrix() override;
		BoundsType getViewBounds();
		Bool32 isInView(Transform<SPACE_TYPE> *pTransform, BoundsType bounds) override;
	protected:
		BoundsType m_viewBounds;
		typename TransformType::MatrixType m_projMatrix;

	private:
		
	};
} //namespace kgs

#include "graphics/scene/camera_op.inl"

#endif // !VG_CAMERA_OP_H
