#ifndef KGS_UTIL_H
#define KGS_UTIL_H
namespace kgs
{
	template <typename T>
	inline T getValue(std::string key, std::unordered_map<std::string, T>& map, std::vector<T>& arr)
	{
		auto iterator = map.find(name);
		if (iterator == map.end())
		{
			return {};
		}
		else
		{
			return iterator->second;
		}
	}

	template <typename T>
	inline void setValue(std::string key, T value, std::unordered_map<std::string, T>& map, std::vector<T>& arr)
	{
		auto iterator = map.find(name);
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
		}
	}
} //namespace kgs
#endif // !KGS_UTIL_H
