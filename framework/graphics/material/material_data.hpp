#ifndef VG_MATERIAL_DATA_H
#define VG_MATERIAL_DATA_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/texture/texture.hpp"

#define VG_M_MAIN_TEXTURE_NAME "_MainTex"
#define VG_M_BUILDIN_NAME "_BuildIn"

#define VG_M_BUILDIN_BINDING 0
#define VG_M_MAIN_TEXTURE_BINDING 1
#define VG_M_OTHER_MIN_BINDING 2

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

	struct MaterialData
	{
		std::vector<std::string> arrDataNames;
		std::unordered_map<std::string, std::vector<Byte>> mapDatas;
		std::unordered_map<std::string, uint32_t> mapDataCounts;
		std::vector<std::string> arrTexNames;
		std::unordered_map<std::string, const Texture *> mapTextures;

		const Texture *getTexture(std::string name) const;
		void setTexture(std::string name, const Texture *pTex);

		void getDataValue(const std::string name, void *dst, uint32_t size, uint32_t offset) const;

		template<typename T>
		T getDataValue(const std::string name) const;

		template <typename T>
		std::vector<T> getDataValue(const std::string name, const uint32_t count) const;

		void setDataValue(const std::string name, void *src, uint32_t size, uint32_t offset);

		template<typename T>
		void setDataValue(const std::string name, const T &value);

		template<typename T>
		void setDataValue(const std::string name, const std::vector<T> &values);

		template<typename T>
		void setDataValue(const std::string name, const T * const pValue, const uint32_t count);

		uint32_t getDataBaseSize(const std::string name) const;

		uint32_t getDataSize(const std::string name) const;

		void memoryCopyData(const std::string name
			, void* dst
			, uint32_t offset
			, uint32_t elementStart
			, uint32_t maxElementCount) const;
	};
} //namespace kgs

#include "graphics/material/material_data.inl"

#endif // !VG_MATERIAL_DATA_H
