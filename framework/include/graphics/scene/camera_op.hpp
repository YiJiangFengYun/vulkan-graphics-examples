#ifndef KGS_CAMERA_OP_H
#define KGS_CAMERA_OP_H

#include "graphics/scene/camera.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class CameraOP : public Camera<SPACE_TYPE>
	{
	public:
		typedef fd::Bounds<typename SpaceTypeInfo<SPACE_TYPE>::PointType> BoundsType;
		CameraOP()
			: Camera<SPACE_TYPE>()
			, m_viewBounds(BoundsType::ValueType(-1.0f), BoundsType::ValueType(1.0f))
		{
			apply();
		}

		void updateProj(BoundsType viewBounds)
		{
			m_viewBounds = viewBounds;
			apply();
		}

		virtual void apply()
		{

		}

		typename TransformType::MatrixType getProjMatrix() override
		{
			return m_projectMatrix;
		}
	protected:
		BoundsType m_viewBounds;
		typename TransformType::MatrixType m_projMatrix;
	};
} //namespace kgs

#endif // !KGS_CAMERA_OP_H
