#ifndef KGS_CAMERA_OP_H
#define KGS_CAMERA_OP_H

#include "graphics/scene/camera.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class CameraOP : public Camera<SPACE_TYPE>
	{
	public:
		CameraOP();
		void updateProj(BoundsType viewBounds);
		virtual void apply() = 0;
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

#endif // !KGS_CAMERA_OP_H
