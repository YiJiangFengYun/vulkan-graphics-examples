#ifndef VG_OBJECT_H
#define VG_OBJECT_H

#include "graphics/global.hpp"
#include "graphics/scene/space_info.hpp"
#include "graphics/scene/space_info_object.hpp"

namespace vg
{
	enum class ObjectType
	{
		UNDEFINED,
		VISUAL_OBJECT,
		CAMERA,
		LIGHT,
		BEGIN_RANGE = VISUAL_OBJECT,
		END_RANGE = LIGHT,
		RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1)
	};

	class BaseObject
	{
	public:
		BaseObject();
		ObjectType getObjectType();
	protected:
		ObjectType m_objectType;
	};

	template <SpaceType SPACE_TYPE>
	class Object : public BaseObject
	{
	public:
		using TransformType = typename SpaceTransformTypeInfo<SPACE_TYPE>::TransformType;

		Object()
			: BaseObject()
			, m_pTransform(new TransformType())
		{

		}

		TransformType *getTransform() const
		{
			return m_pTransform.get();
		}

	protected:
		std::shared_ptr<TransformType> m_pTransform;
	};

} //namespace kgs

#endif // !VG_OBJECT_H
