#ifndef VG_BASE_H
#define VG_BASE_H

#include <unordered_map>

namespace vg
{
	enum class BaseType
	{
		UNDEFINED,
		APP,
		TEXTURE,
		MATERIAL,
		PASS,
		SHADER,
		VERTEX_DATA,
		INDEX_DATA,
		MESH,
		RENDERER,
		SCENE,
		SCENE_OBJECT,
		TRANSFORM
	};

	using InstanceID = uint32_t;
	class Base
	{
	public:
		Base(BaseType baseType);
		virtual ~Base();
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

#endif // !VG_BASE_H
