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

	typedef uint32_t InstanceID;

	class Base
	{
	public:
		Base(BaseType baseType);
		BaseType getBaseType();
		InstanceID getID();
	protected:
		BaseType m_baseType;
		InstanceID m_id;
	private:
		Base() = delete;
		static std::unordered_map<BaseType, InstanceID> m_idCreator;
	};
} //namespace kgs

#endif // !KGS_BASE_H
