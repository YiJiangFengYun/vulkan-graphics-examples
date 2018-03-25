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

		virtual ~BaseVisualObject();

		Material *getMaterial() const;

		void setMaterial(Material *pMaterial);

		BaseMesh *getMesh() const;

		uint32_t getSubMeshOffset() const;
		uint32_t getSubMeshCount() const;

		void updateSubMeshInfo(uint32_t subMeshOffset, uint32_t subMeshCount);

		Bool32 getIsVisibilityCheck() const;
		void setIsVisibilityCheck(Bool32 value);

		Bool32 getHasClipRect() const;
		void setHasClipRect(Bool32 value);
		uint32_t getClipRectCount() const;
		const fd::Rect2D *getClipRects() const;
		void updateClipRects(fd::ArrayProxy<fd::Rect2D> rects, uint32_t count, uint32_t offset = 0u);
		void updateClipRects(fd::Rect2D rect, uint32_t count, uint32_t offset = 0u);

	protected:
		Material *m_pMaterial;
		BaseMesh *m_pMesh;
		int32_t m_subMeshOffset;
		int32_t m_subMeshCount;
		Bool32 m_isVisibilityCheck;
		Bool32 m_hasClipRect;
		//Valid range of ClipRect is [(0, 0), (1, 1)]
		std::vector<fd::Rect2D> m_clipRects;

		void _asyncMeshData();
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
			m_clipRects.resize(dynamic_cast<const ContentMesh *>(m_pMesh)->getSubMeshOffset());
		}

		void setMesh(MeshDimType *pMesh
		    , uint32_t subMeshOffset
			, uint32_t subMeshCount)
		{
			m_pMesh = pMesh;
			m_subMeshOffset = subMeshOffset;
			m_subMeshCount = subMeshCount;
			m_clipRects.resize(subMeshCount);
		}
	protected:
		//aggregations
		
	};
} //namespace kgs

#endif // !VG_VISUAL_OBJECT_H
