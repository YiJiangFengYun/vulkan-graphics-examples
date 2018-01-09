#ifndef KGS_LIGHT_3_H
#define KGS_LIGHT_3_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/light.hpp"

namespace kgs
{
	class Light3 : public Light<SpaceType::SPACE_3>
	{
	public:
		Light3();
	};

} //namespace kgs

#endif // !KGS_LIGHT_3_H
