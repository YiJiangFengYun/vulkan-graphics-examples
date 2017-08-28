#ifndef KGS_TEXTURE_TYPE_H
#define KGS_TEXTURE_TYPE_H

namespace kgs
{
	enum class TextureType
	{
		UNDEFINED,
		TEX_1D,
		TEX_2D,
		TEX_3D,
		CUBE,
		TEX_2D_ARRAY,
		BEGIN_RANGE = UNDEFINED,
		END_RANGE = TEX_2D_ARRAY,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	enum class FilterMode
	{
		NEAREST,
		LINEAR,
		TRILINEAR,
		BEGIN_RANGE = NEAREST,
		END_RANGE = TRILINEAR,
		RANGE_SIZE = (BEGIN_RANGE - END_RANGE + 1)
	};

	enum class SamplerAddressMode
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRROR_CLAMP_TO_EDGE,
		BEGIN_RANGE = REPEAT,
		END_RANGE = MIRROR_CLAMP_TO_EDGE,
		RANGE_SIZE = (BEGIN_RANGE - END_RANGE + 1)
	};
}


#endif // !KGS_TEXTURE_TYPE_H
