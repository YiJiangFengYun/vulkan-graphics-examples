#include "graphics/material/material.hpp"

namespace vg
{
	Material::BindInfo::BindInfo(const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
		, uint32_t trunkFramebufferWidth
		, uint32_t trunkFramebufferHeight
        , const Matrix4x4 *pModelMatrix
        , const BaseMesh *pMesh
        , uint32_t subMeshIndex
        , Bool32 hasClipRect
        , const fd::Rect2D *pClipRect
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer *pPreparingBuildInDataCostTimer
        , fd::CostTimer *pPreparingPipelineCostTimer
        , fd::CostTimer *pPreparingCommandBufferCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        )
        : pProjMatrix(pProjMatrix)
        , pViewMatrix(pViewMatrix)
		, trunkFramebufferWidth(trunkFramebufferWidth)
		, trunkFramebufferHeight(trunkFramebufferHeight)
		, pModelMatrix(pModelMatrix)
        , pMesh(pMesh)
        , subMeshIndex(subMeshIndex)
        , hasClipRect(hasClipRect)
        , pClipRect(pClipRect)
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , pPreparingBuildInDataCostTimer(pPreparingBuildInDataCostTimer)
        , pPreparingPipelineCostTimer(pPreparingPipelineCostTimer)
        , pPreparingCommandBufferCostTimer(pPreparingCommandBufferCostTimer)
#endif //DEBUG and VG_ENABLE_COST_TIMER
    {
    }

    Material::BindResult::BindResult(CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        )
	    : pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
    {

    }

	Material::Material()
		: Base(BaseType::MATERIAL)
		, m_renderQueueType()
		, m_renderPriority()
		, m_arrPasses()
		, m_mapPasses()
		, m_pMainShader()
		, m_pMainPass()
	{
		m_pMainShader = std::shared_ptr<vg::Shader>{new vg::Shader()};
		m_pMainPass = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pMainShader.get())};
		_addPass(m_pMainPass.get());
	}

	Material::~Material()
	{
	}

	uint32_t Material::getPassCount() const
	{
		return static_cast<uint32_t>(m_arrPasses.size());
	}

	Pass *Material::getPassWithIndex(uint32_t index) const
	{
		return m_arrPasses[index];
	}

	Pass * const *Material::getPasses() const
	{
		return m_arrPasses.data();
	}

	Bool32 Material::isHas(const Pass *pass) const
	{
		return m_mapPasses.find(pass->getID()) != m_mapPasses.cend();
	}


	Shader *Material::getMainShader() const 
	{
		return m_pMainShader.get();
	}

	Pass *Material::getMainPass() const
	{
		return m_pMainPass.get();
	}

	void Material::apply()
	{
		for (const auto& item : m_mapPasses)
		{
			item.second->apply();
		}
	}

	MaterialShowType Material::getShowType()
	{
		return m_renderQueueType;
	}

	void Material::setRenderQueueType(MaterialShowType type)
	{
		m_renderQueueType = type;
	}

	uint32_t Material::getRenderPriority()
	{
		return m_renderPriority;
	}

	void Material::setRenderPriority(uint32_t priority)
	{
		m_renderPriority = priority;
	}

	void Material::bindToRender(const BindInfo info, BindResult *pResult)
    {
		auto &result = *pResult;

        if (result.pTrunkRenderPassCmdBuffer != nullptr)
        {
            RenderPassInfo trunkRenderPassInfo;
            trunkRenderPassInfo.pRenderPass = nullptr;
            trunkRenderPassInfo.framebufferWidth = info.trunkFramebufferWidth;
            trunkRenderPassInfo.framebufferHeight = info.trunkFramebufferHeight;
            trunkRenderPassInfo.projMatrix = *(info.pProjMatrix);
            trunkRenderPassInfo.viewMatrix = *(info.pViewMatrix);
            trunkRenderPassInfo.pPass = *m_arrPasses.data();
            trunkRenderPassInfo.modelMatrix = *(info.pModelMatrix);
            trunkRenderPassInfo.pMesh = info.pMesh;
            trunkRenderPassInfo.subMeshIndex = info.subMeshIndex;
            trunkRenderPassInfo.hasClipRect = info.hasClipRect;
            trunkRenderPassInfo.pClipRect = info.pClipRect;

            CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;

            result.pTrunkRenderPassCmdBuffer->addCmd(cmdInfo);
        }

    }

	void Material::_addPass(Pass *pPass)
	{
		if (isHas(pPass)) return;
		m_arrPasses.push_back(pPass);
		m_mapPasses[pPass->getID()] = pPass;
	}

	void Material::_removePass(Pass *pPass)
	{
		if (isHas(pPass) == VG_FALSE) return;
		m_arrPasses.erase(std::remove(m_arrPasses.begin(), m_arrPasses.end(), pPass));
		m_mapPasses.erase(pPass->getID());
	}
}