#ifndef VG_VISUAL_OBJECT_H
#define VG_VISUAL_OBJECT_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/object.hpp"
#include "graphics/mesh/mesh.hpp"
#include "graphics/material/material.hpp"
#include "graphics/visualizer/visualizer.hpp"
#include "graphics/pipeline/pipeline_cache.hpp"

namespace vg
{
	class BaseVisualObject
	{
	public:
	    struct BindInfo 
        {
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
            PipelineCache *pPipelineCache;
			vk::RenderPass *pRenderPass;
			vk::CommandBuffer *pCommandBuffer;
			uint32_t framebufferWidth;
			uint32_t framebufferHeight;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingBuildInDataCostTimer;
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER
            BindInfo(const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
                , PipelineCache *pPipelineCache = nullptr
				, vk::RenderPass *pRenderPass = nullptr
				, vk::CommandBuffer *pCommandBuffer = nullptr
			    , uint32_t framebufferWidth = 0u
			    , uint32_t framebufferHeight = 0u
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingBuildInDataCostTimer = nullptr
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                );
        };
    
        struct BindResult
        {
            uint32_t drawCount;
            BindResult(uint32_t drawCount = 0u);
        };

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

		void bindToRender(const BindInfo info, BindResult *pResult);

	protected:
		Material *m_pMaterial;
		BaseMesh *m_pMesh;
		int32_t m_subMeshOffset;
		int32_t m_subMeshCount;
		Bool32 m_isVisibilityCheck;
		Bool32 m_hasClipRect;
		//Valid range of ClipRect is [(0, 0), (1, 1)]
		std::vector<fd::Rect2D> m_clipRects;
		std::shared_ptr<Visualizer> m_pVisualizer;
		void _asyncMeshData();
		virtual Matrix4x4 _getModelMatrix() = 0;
	};

	class BaseVisualObject_Visualizer : public BaseVisualObject
	{
	public:
		BaseVisualObject_Visualizer();

		template <class VisualizerType>
		BaseVisualObject_Visualizer()
		    : BaseVisualObject()
		{
			m_pVisualizer = std::shared_ptr<Visualizer>{new VisualizerType()};
		}

		const Visualizer *getVisualizer() const
		{
			return m_pVisualizer.get();
		}

	protected:
	    
	};

	template <SpaceType SPACE_TYPE>
	class VisualObject : public BaseVisualObject_Visualizer, public Object<SPACE_TYPE>
	{
	public:
		using MeshDimType = Mesh<SpaceTypeInfo<SPACE_TYPE>::MESH_DIM_TYPE>;

		VisualObject()
			: BaseVisualObject_Visualizer()
			, Object<SPACE_TYPE>()
		{
			m_objectType = ObjectType::VISUAL_OBJECT;

		}

		template <class VisualizerType>
		VisualObject()
		    : BaseVisualObject_Visualizer<VisualizerType>()
			, Object<SPACE_TYPE>()
		{

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
