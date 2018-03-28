#ifndef VG_VISUAL_OBJECT_2_H
#define VG_VISUAL_OBJECT_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/visual_object.hpp"

namespace vg
{
	class VisualObject2 : public VisualObject<SpaceType::SPACE_2>
	{
	public:
		VisualObject2();

		template <class VisualizerType>
		VisualObject2()
		    : VisualObject<SpaceType::SPACE_2><VisualizerType>()
		{

		}

		virtual Matrix4x4 _getModelMatrix() override;
	};

} //namespace kgs

#endif // !VG_VISUAL_OBJECT_2_H
