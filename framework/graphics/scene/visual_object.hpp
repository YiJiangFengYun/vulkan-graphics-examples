#ifndef VG_VISUAL_OBJECT_H
#define VG_VISUAL_OBJECT_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/material/material.hpp"

namespace vg
{
	class BaseVisualObject
	{
	public:
		BaseVisualObject();

		std::shared_ptr<Material> getMaterial();

		void setMaterial(std::shared_ptr<Material> pMaterial);

		std::shared_ptr<SepMesh> getMesh();

	protected:
		std::shared_ptr<Material> m_pMaterial;
		std::shared_ptr<SepMesh> m_pMesh;
	};

	template <SpaceType SPACE_TYPE>
	class VisualObject : public BaseVisualObject, public Object<SPACE_TYPE>
	{
	public:
		typedef Mesh<SpaceTypeInfo<SPACE_TYPE>::MESH_TYPE> MeshType;

		VisualObject()
			: BaseVisualObject()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::VISUAL_OBJECT;

		}

		void setMesh(std::shared_ptr<MeshType> pMesh)
		{
			m_pMesh = pMesh;
		}
	protected:
		//aggregations
		
	};
} //namespace kgs

#endif // !VG_VISUAL_OBJECT_H
