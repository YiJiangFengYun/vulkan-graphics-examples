#ifndef KGS_VISUAL_OBJECT_3_H
#define KGS_VISUAL_OBJECT_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/visual_object.hpp"

namespace kgs
{
	class VisualObject3 : public VisualObject<SpaceType::SPACE_3>
	{
	public:
		VisualObject3();
	};

} //namespace kgs

#endif // !KGS_VISUAL_OBJECT_3_H
