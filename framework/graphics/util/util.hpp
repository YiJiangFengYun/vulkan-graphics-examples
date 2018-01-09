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
} //namespace kgs
#endif // !KGS_UTIL_H
