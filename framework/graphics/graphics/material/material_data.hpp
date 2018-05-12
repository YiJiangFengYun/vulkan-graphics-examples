#ifndef VG_MATERIAL_DATA_H
#define VG_MATERIAL_DATA_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/texture/texture.hpp"

namespace vg
{
#ifdef OPAQUE
#define TEMP_OPAQUE OPAQUE
#undef OPAQUE
#endif //OPAQUE

#ifdef TRANSPARENT
#define TEMP_TRANSPARENT TRANSPARENT
#undef TRANSPARENT
#endif //TRANSPARENT
	enum class MaterialShowType
	{
		BACKGROUND,
		OPAQUE,
		TRANSPARENT,
		OVERLAY,
		BEGIN_RANGE = BACKGROUND,
		END_RANGE = OVERLAY,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};
#ifdef TEMP_OPAQUE
#define OPAQUE TEMP_OPAQUE
#endif //TEMP_OPAQUE

#ifdef TEMP_TRANSPARENT
#define TRANSPARENT TEMP_TRANSPARENT
#endif //TEMP_TRANSPARENT

	
} //namespace kgs

#endif // !VG_MATERIAL_DATA_H
