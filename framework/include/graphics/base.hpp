#ifndef KGS_BASE_H
#define KGS_BASE_H

#include <unordered_map>

namespace kgs
{
	enum class BaseType
	{
		UNDEFINED,
		APP,
		TEXTURE,
		MATERIAL,
		PASS,
		SHADER,
		MESH,
		RENDERER,
		SCENE,
		SCENE_OBJECT,
		TRANSFORM
	};
	class Base
	{
	public:
		Base(BaseType baseType);
		BaseType getBaseType();
		uint32_t getID();
	protected:
		BaseType m_baseType;
		uint32_t m_id;
	private:
		Base() = delete;
		static std::unordered_map<BaseType, std::uint32_t> m_idCreateor;
	};
} //namespace kgs

#endif // !KGS_BASE_H
