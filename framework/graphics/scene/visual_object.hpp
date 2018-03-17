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

		Material *getMaterial() const;

		void setMaterial(Material *pMaterial);

		BaseMesh *getMesh() const;

		uint32_t getSubMeshOffset() const;
		uint32_t getSubMeshCount() const;

		void updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount);

		Bool32 getIsVisibilityCheck() const;
		void setIsVisibilityCheck(Bool32 value);

	protected:
		Material *m_pMaterial;
		BaseMesh *m_pMesh;
		int32_t m_subMeshOffset;
		int32_t m_subMeshCount;
		Bool32 m_isVisibilityCheck;
	};

	template <SpaceType SPACE_TYPE>
	class VisualObject : public BaseVisualObject, public Object<SPACE_TYPE>
	{
	public:
		using MeshDimType = Mesh<SpaceTypeInfo<SPACE_TYPE>::MESH_DIM_TYPE>;

		VisualObject()
			: BaseVisualObject()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::VISUAL_OBJECT;

		}

		void setMesh(MeshDimType *pMesh)
		{
			m_pMesh = pMesh;
			m_subMeshOffset = -1;
			m_subMeshCount = -1;
		}

		void setMesh(MeshDimType *pMesh
		    , uint32_t subMeshOffset
			, uint32_t subMeshCount)
		{
			m_pMesh = pMesh;
			m_subMeshOffset = subMeshOffset;
			m_subMeshCount = subMeshCount;
		}
	protected:
		//aggregations
		
	};
} //namespace kgs

#endif // !VG_VISUAL_OBJECT_H
