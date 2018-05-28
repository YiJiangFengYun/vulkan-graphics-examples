#ifndef VG_UTIL_H
#define VG_UTIL_H

#include <string>
#include <unordered_map>
#include <vector>
#include "graphics/global.hpp"

namespace vg
{
//only map
template <typename T>
inline Bool32 hasValue(std::string name, const std::unordered_map<std::string, T> &map)
{
	return map.count(name) != 0;
}

template <typename T>
inline const T &getValue(std::string name, const std::unordered_map<std::string, T> &map)
{
	const auto &iterator = map.find(name);
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
inline T &getValue(std::string name, std::unordered_map<std::string, T> &map)
{
	const auto &iterator = map.find(name);
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
inline void setValue(std::string name, const T &value, std::unordered_map<std::string, T> &map)
{
	const auto &iterator = map.find(name);
	if (iterator == map.cend())
	{
		throw std::runtime_error("Map don't has item whose key: " + name);
	}
	else
	{
		iterator->second = value;
	}
}

template <typename T>
inline void addValue(std::string name, const T &value, std::unordered_map<std::string, T> &map)
{
	const auto &iterator = map.find(name);
	if (iterator != map.cend())
	{
		throw std::runtime_error("Map has exist the item whose key: " + name);
	}
	map.insert({name, value});
}

template <typename T>
inline void removeValue(std::string name, std::unordered_map<std::string, T> &map)
{
	map.erase(name);
}

//array and map
template <typename T>
inline Bool32 hasValue(std::string name, const std::unordered_map<std::string, T> &map, const std::vector<std::string> &arr)
{
	return map.count(name) != 0;
}

template <typename T>
inline const T &getValue(std::string name, const std::unordered_map<std::string, T> &map, const std::vector<std::string> &arr)
{
	const auto &iterator = map.find(name);
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
inline T &getValue(std::string name, std::unordered_map<std::string, T> &map, std::vector<std::string> &arr)
{
	const auto &iterator = map.find(name);
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
inline void setValue(std::string name, const T &value, std::unordered_map<std::string, T> &map,
					 std::vector<std::string> &arr)
{
	{
		const auto &iterator = map.find(name);
		if (iterator == map.cend())
		{
			throw std::runtime_error("Map don't has item whose key: " + name);
		}
		else
		{
			iterator->second = value;
		}
	}
}

template <typename T>
inline void addValue(std::string name, const T &value, std::unordered_map<std::string, T> &map, std::vector<std::string> &arr)
{
	{
		const auto &iterator = map.find(name);
		if (iterator != map.cend())
		{
			throw std::runtime_error("Map has exist the item whose key: " + name);
		}
		map.insert({name, value});
	}
	{
		auto iterator = std::find(arr.begin(), arr.end(), name);
		if (iterator != arr.end())
		{
			throw std::runtime_error("Array has exist the item whose value: " + name);
		}
		arr.push_back(name);
	}
}

template <typename T>
inline void removeValue(std::string name, std::unordered_map<std::string, T> &map, std::vector<std::string> &arr)
{
	map.erase(name);
	auto iterator = std::find(arr.begin(), arr.end(), name);
	arr.erase(iterator);
}

template <typename T>
inline Bool32 isContainStruct(T parent, T child)
{
	size_t size = sizeof(T);
	size_t index = 0;
	using SepType = uint8_t;
	size_t size2 = sizeof(SepType);
	size_t sep = size / size2;
	Bool32 isContain = VG_TRUE;
	for (size_t i = 0; i < sep; ++i)
	{
		SepType copy1 = 0;
		SepType copy2 = 0;
		uint8_t *src1 = static_cast<uint8_t *>(static_cast<void *>(&parent)) + index;
		uint8_t *src2 = static_cast<uint8_t *>(static_cast<void *>(&child)) + index;
		memcpy(&copy1, src1, sizeof(SepType));
		memcpy(&copy2, src2, sizeof(SepType));
		if ((copy1 & copy2) != copy2)
		{
			isContain = VG_FALSE;
			break;
		}
		index += size2;
	}

	return isContain;
}

uint32_t getNextCapacity(uint32_t current);
} //namespace vg
#endif // !VG_UTIL_H
