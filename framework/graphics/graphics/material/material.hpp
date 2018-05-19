#ifndef VG_MATERIAL_H
#define VG_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/pre_z_pass/pre_z_pass.hpp"
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
			InstanceID objectID;
            const Matrix4x4 *pModelMatrix;
            const BaseMesh *pMesh;
            uint32_t subMeshIndex;
            Bool32 hasClipRect;
            const fd::Rect2D clipRect;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

            BindInfo(const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
			    , uint32_t trunkFramebufferWidth = 0u
			    , uint32_t trunkFramebufferHeight = 0u
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                , InstanceID objectID = 0
                , const Matrix4x4 *pModelMatrix = nullptr
                , const BaseMesh *pMesh = nullptr
                , uint32_t subMeshIndex = 0u
                , Bool32 hasClipRect = VG_FALSE
                , const fd::Rect2D clipRect = fd::Rect2D()
                );
        };
    
        struct BindResult
        {
			CmdBuffer *pPreZCmdBuffer;
            CmdBuffer *pBranchCmdBuffer;
            CmdBuffer *pTrunkRenderPassCmdBuffer;
            CmdBuffer *pTrunkWaitBarrierCmdBuffer;
            BindResult(CmdBuffer *pPreZCmdBuffer = nullptr
			    , CmdBuffer *pBranchCmdBuffer = nullptr
                , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
                , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
                );
        };


		struct EndBindInfo 
		{
			InstanceID objectID;
			uint32_t subMeshIndex;
			EndBindInfo(InstanceID objectID = 0
			    , uint32_t subMeshIndex = 0u);
		};

		struct MaterialCreateInfo
		{
			Bool32 onlyOnce;
			Bool32 createPreZPass;
			MaterialCreateInfo(Bool32 onlyOnce = VG_FALSE, Bool32 createPreZPass = VG_FALSE);
		};

		Material(Bool32 onlyOnce = VG_FALSE);
		Material(MaterialCreateInfo createInfo);
		~Material();

		uint32_t getPassCount() const;
		Pass *getPassWithIndex(uint32_t index) const;
		Pass * const *getPasses() const;
		Bool32 isHas(const Pass *pPass) const;

		Shader *getMainShader() const;
		Pass *getMainPass() const;

		PreZPass * getPreZPass() const;

		MaterialShowType getShowType();
		void setRenderQueueType(MaterialShowType type);
		uint32_t getRenderPriority();
		void setRenderPriority(uint32_t priority);
		/*Call the apply methods of all passes in the material.*/
		virtual void apply();

		void beginBind(const BindInfo info, BindResult *pResult) const;
		void endBind(const EndBindInfo info) const;

		std::string name;
	protected:
	    Bool32 m_onlyOnce;
		std::shared_ptr<InstanceID> m_pBindTargetID; 
		MaterialShowType m_renderQueueType;
		uint32_t m_renderPriority;
		std::shared_ptr<Shader> m_pMainShader; 
		std::shared_ptr<Pass> m_pMainPass;

		std::shared_ptr<PreZPass> m_pPreZPass;

		std::vector<Pass *> m_arrPasses;
		std::unordered_map<InstanceID, Pass *> m_mapPasses;

		virtual void _addPass(Pass *pPass);
		virtual void _removePass(Pass *pPass);

		virtual void _beginBind(const BindInfo info, BindResult *pResult) const;
		virtual void _endBind(const EndBindInfo info) const;
	};
}

#endif // !VG_MATERIAL_H
