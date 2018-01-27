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

	typedef uint32_t InstanceID;
	typedef uint32_t StateID;
#   define  VG_BASE_INIT_STATE_ID 1u

	class Base
	{
	public:
		Base(BaseType baseType);
		BaseType getBaseType();
		InstanceID getID();
		StateID getStateID();
		void updateStateID();
	protected:
		BaseType m_baseType;
		InstanceID m_id;
		StateID m_stateID;
	private:
		Base() = delete;
		static std::unordered_map<BaseType, InstanceID> m_idCreator;
	};
} //namespace kgs

#endif // !VG_BASE_H
