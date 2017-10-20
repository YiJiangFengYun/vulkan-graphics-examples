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
		/*auto iterator = map.find(name);
		return iterator != map.end();*/

	}


	template <typename T>
	inline const T& getValue(std::string name, std::unordered_map<std::string, T>& map)
	{
		return map[name];
		/*auto iterator = map.find(name);
		if (iterator == map.end())
		{
			throw std::runtime_error("The Content of the specify key don't exit in map");
		}
		else
		{
			return iterator->second;
		}*/
	}

	template <typename T>
	inline void setValue(std::string name, const T& value, std::unordered_map<std::string, T>& map)
	{
		map[name] = value;
		/*auto iterator = map.find(name);
		if (iterator == map.end())
		{
			map.insert(std::make_pair(name, value));
		}
		else
		{
			iterator->second = value;
		}*/
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
		/*auto iterator = map.find(name);
		if (iterator == map.end())
		{
			map.insert(std::make_pair(name, value));
			arr.push_back(value);
		}
		else
		{
			iterator->second = value;
			auto iterator2 = std::find(arr.begin(), arr.end(), iterator->second);
			*iterator2 = value;
		}*/
	}
} //namespace kgs
#endif // !KGS_UTIL_H
