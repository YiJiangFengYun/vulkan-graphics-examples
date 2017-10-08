#ifndef KGS_RENDERER_OPTION_H
#define KGS_RENDERER_OPTION_H


#include "graphics/material/material_data.hpp"

namespace kgs
{
	enum class RenderQueueType
	{
		BACKGROUND,
		OPAQUE,
		TRANSPARENT,
		OVERLAY,
		BEGIN_RANGE = BACKGROUND,
		END_RANGE = OVERLAY,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	inline RenderQueueType tranMaterialShowTypeToRenderQueueType(MaterialShowType showType)
	{
		return static_cast<RenderQueueType>(showType);
	}

} //namespace kgs

#endif // !KGS_RENDERER_OPTION_H
