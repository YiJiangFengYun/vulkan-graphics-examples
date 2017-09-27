#ifndef KGS_OBJECT_H
#define KGS_OBJECT_H

#include "graphics/global.hpp"
#include "graphics/scene/option.hpp"

namespace kgs
{
	template <SpaceType SPACE_TYPE>
	class Object
	{
	public:
		typedef typename SpaceTypeInfo<SPACE_TYPE>::VectorType VectorType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::PointType PointType;
		typedef typename SpaceTypeInfo<SPACE_TYPE>::MatrixType MatrixType;

	protected:
		PointType m_location;
		VectorType m_scale;
	};

} //namespace kgs

#endif // !KGS_OBJECT_H
