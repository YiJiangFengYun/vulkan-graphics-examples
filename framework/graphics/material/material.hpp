#ifndef VG_MATERIAL_H
#define VG_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/material/cmd.hpp"
namespace vg
{
	class Material : public Base
	{
	public:
	    struct BindInfo 
        {
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
			uint32_t trunkFramebufferWidth;
			uint32_t trunkFramebufferHeight;
            const Matrix4x4 *pModelMatrix;
            const BaseMesh *pMesh;
            uint32_t subMeshIndex;
            const fd::Viewport *pViewport;
            const fd::Rect2D *pScissor;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingBuildInDataCostTimer;
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

            BindInfo(const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
			    , uint32_t trunkFramebufferWidth = 0u
			    , uint32_t trunkFramebufferHeight = 0u
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingBuildInDataCostTimer = nullptr
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                , const Matrix4x4 *pModelMatrix = nullptr
                , const BaseMesh *pMesh = nullptr
                , uint32_t subMeshIndex = 0u
                , const fd::Viewport *pViewport = nullptr
                , const fd::Rect2D *pScissor = nullptr
                );
        };
    
        struct BindResult
        {
            CmdBuffer *pBranchCmdBuffer;
            CmdBuffer *pTrunkRenderPassCmdBuffer;
            CmdBuffer *pTrunkWaitBarrierCmdBuffer;
            BindResult(CmdBuffer *pBranchCmdBuffer = nullptr
                , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
                , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
                );
        };

		Material();
		~Material();

		uint32_t getPassCount() const;
		Pass *getPassWithIndex(uint32_t index) const;
		Pass * const *getPasses() const;
		Bool32 isHas(const Pass *pPass) const;

		Shader *getMainShader() const;
		Pass *getMainPass() const;

		MaterialShowType getShowType();
		void setRenderQueueType(MaterialShowType type);
		uint32_t getRenderPriority();
		void setRenderPriority(uint32_t priority);
		/*Call the apply methods of all passes in the material.*/
		virtual void apply();

		virtual void bindToRender(const BindInfo info, BindResult *pResult);
	protected:
		//--compositions
		MaterialShowType m_renderQueueType;
		uint32_t m_renderPriority;
		std::shared_ptr<Shader> m_pMainShader; 
		std::shared_ptr<Pass> m_pMainPass;
		//--compositions

		//--aggregations
		std::vector<Pass *> m_arrPasses;
		std::unordered_map<InstanceID, Pass *> m_mapPasses;
		//--aggregations

		virtual void _addPass(Pass *pPass);
		virtual void _removePass(Pass *pPass);
	};
}

#endif // !VG_MATERIAL_H
