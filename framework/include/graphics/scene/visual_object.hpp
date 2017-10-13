#ifndef KGS_VISUAL_OBJECT_H
#define KGS_VISUAL_OBJECT_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/material/material.hpp"

namespace kgs
{
	class BaseVisualObject
	{
	public:
		std::shared_ptr<Material> getMaterial();

		void setPMaterial(std::shared_ptr<Material> pMaterial);

		std::shared_ptr<BaseMesh> getMesh();

		uint32_t getSubMeshIndex();

	protected:
		std::shared_ptr<Material> m_pMaterial;
		std::shared_ptr<BaseMesh> m_pMesh;
		uint32_t m_subMeshIndex;
	};

	template <SpaceType SPACE_TYPE>
	class VisualObject : public BaseVisualObject, public Object<SPACE_TYPE>
	{
	public:
		typedef Mesh<SpaceConstInfo<SPACE_TYPE>::MESH_TYPE> MeshType;

		VisualObject()
			: BaseVisualObject()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::VISUAL_OBJECT;

		}

		void setMesh(std::shared_ptr<MeshType> pMesh, uint32_t subMeshIndex)
		{
			m_pMesh = pMesh;
			m_subMeshIndex = subMeshIndex;
		}
	protected:
		//aggregations
		
	};
} //namespace kgs

#endif // !KGS_VISUAL_OBJECT_H
