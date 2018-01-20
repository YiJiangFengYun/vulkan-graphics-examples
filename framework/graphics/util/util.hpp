#ifndef KGS_UTIL_H
#define KGS_UTIL_H

#include <string>
#include <unordered_map>
#include <vector>
#include "graphics/global.hpp"

namespace kgs
{
	template <typename T>
	inline Bool32 isHas(std::string name, std::unordered_map<std::string, T>& map)
	{
		return map.count(name) != 0;
	}

	template <typename T>
	inline const T& getValue(std::string name, const std::unordered_map<std::string, T>& map)
	{
		const auto& iterator = map.find(name);
		if (iterator == map.cend())
		{
			throw std::runtime_error("Map don't has item whose key: " + name);
		}
		else
		{
			return iterator->second;
		}
	}

	template <typename T>
	inline void setValue(std::string name, const T& value, std::unordered_map<std::string, T>& map)
	{
		map[name] = value;
	}

	template <typename T>
	inline void setValue(std::string name, const T& value, std::unordered_map<std::string, T>& map, std::vector<std::string>& arr)
	{
		map[name] = value;

		auto iterator = std::find(arr.begin(), arr.end(), name);
		if (iterator == arr.end())
		{
			arr.push_back(name);
		}
	}

	template <typename T>
	inline Bool32 isContainStruct(T parent, T child)
	{
		size_t size = sizeof(T);
		size_t index = 0;
		typedef uint8_t sep_type;
		size_t size2 = sizeof(sep_type);
		size_t sep = size / size2;
		Bool32 isContain = KGS_TRUE;
		for (size_t i = 0; i < sep; ++i)
		{
			sep_type copy1 = 0;
			sep_type copy2 = 0;
			uint8_t *src1 = static_cast<uint8_t *>(static_cast<void *>(&parent)) + index;
			uint8_t *src2 = static_cast<uint8_t *>(static_cast<void *>(&child)) + index;
			memcpy(&copy1, src1, sizeof(sep_type));
			memcpy(&copy2, src2, sizeof(sep_type));
			if ((copy1 & copy2) != copy2)
			{
				isContain = KGS_FALSE;
				break;
			}
			index += size2;
		}

		return isContain;
	}
} //namespace kgs
#endif // !KGS_UTIL_H
