#include "graphics/material/material.hpp"

namespace vg
{
	Material::BindInfo::BindInfo(const Matrix4x4 *pProjMatrix
        , const Matrix4x4 *pViewMatrix
		, uint32_t trunkFramebufferWidth
		, uint32_t trunkFramebufferHeight
		, InstanceID objectID
        , const Matrix4x4 *pModelMatrix
        , const BaseMesh *pMesh
        , uint32_t subMeshIndex
        , Bool32 hasClipRect
        , const fd::Rect2D clipRect
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer *pPreparingPipelineCostTimer
        , fd::CostTimer *pPreparingCommandBufferCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        )
        : pProjMatrix(pProjMatrix)
        , pViewMatrix(pViewMatrix)
		, trunkFramebufferWidth(trunkFramebufferWidth)
		, trunkFramebufferHeight(trunkFramebufferHeight)
		, objectID(objectID)
		, pModelMatrix(pModelMatrix)
        , pMesh(pMesh)
        , subMeshIndex(subMeshIndex)
        , hasClipRect(hasClipRect)
        , clipRect(clipRect)
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , pPreparingPipelineCostTimer(pPreparingPipelineCostTimer)
        , pPreparingCommandBufferCostTimer(pPreparingCommandBufferCostTimer)
#endif //DEBUG and VG_ENABLE_COST_TIMER
    {
    }

    Material::BindResult::BindResult(CmdBuffer *pPreZCmdBuffer
	    , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        )
	    : pPreZCmdBuffer(pPreZCmdBuffer)
		, pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
    {

    }

	Material::EndBindInfo::EndBindInfo(InstanceID objectID
	    , uint32_t subMeshIndex
		)
	    : objectID(objectID)
		, subMeshIndex(subMeshIndex)
	{

	}

	Material::MaterialCreateInfo::MaterialCreateInfo(Bool32 onlyOnce
	    , Bool32 createPreZPass
	    )
		: onlyOnce(onlyOnce)
		, createPreZPass(createPreZPass) 

	{

	}

	Material::Material(Bool32 onlyOnce)
		: Base(BaseType::MATERIAL)
		, name()
		, m_onlyOnce(onlyOnce)
		, m_bindTargetID(0)
		, m_renderQueueType()
		, m_renderPriority()
		, m_arrPasses()
		, m_mapPasses()
		, m_pMainShader()
		, m_pMainPass()
		, m_pPreZPass()
	{
		m_pMainShader = std::shared_ptr<vg::Shader>{new vg::Shader()};
		m_pMainPass = std::shared_ptr<vg::Pass>{ new vg::Pass(m_pMainShader.get())};
		_addPass(m_pMainPass.get());
	}

	Material::Material(MaterialCreateInfo createInfo)
	    : Material(createInfo.onlyOnce)
	{
		if (createInfo.createPreZPass) {
			m_pPreZPass = std::shared_ptr<PreZPass>{new PreZPass()};
		}
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

	PreZPass * Material::getPreZPass() const
	{
		return m_pPreZPass.get();
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

	void Material::beginBindToRender(const BindInfo info, BindResult *pResult)
    {
		if (m_onlyOnce == VG_TRUE) {
			if (m_bindTargetID != 0) 
			    throw std::runtime_error("The material binding is only once, but it was used to repeatedly bind.");
			m_bindTargetID = info.objectID;
		}

		auto &result = *pResult;
		if (m_pPreZPass != nullptr && result.pPreZCmdBuffer != nullptr)
		{
            RenderPassInfo trunkRenderPassInfo;
            trunkRenderPassInfo.pRenderPass = nullptr;
            trunkRenderPassInfo.pFrameBuffer = nullptr;			
            trunkRenderPassInfo.framebufferWidth = info.trunkFramebufferWidth;
            trunkRenderPassInfo.framebufferHeight = info.trunkFramebufferHeight;
            trunkRenderPassInfo.projMatrix = *(info.pProjMatrix);
            trunkRenderPassInfo.viewMatrix = *(info.pViewMatrix);
            trunkRenderPassInfo.pPass = m_pPreZPass->getPass();
            trunkRenderPassInfo.modelMatrix = *(info.pModelMatrix);
            trunkRenderPassInfo.pMesh = info.pMesh;
            trunkRenderPassInfo.subMeshIndex = info.subMeshIndex;
            trunkRenderPassInfo.viewport = fd::Viewport();
            trunkRenderPassInfo.scissor = info.hasClipRect ? info.clipRect : fd::Rect2D();
            CmdInfo cmdInfo;
            cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;
            result.pPreZCmdBuffer->addCmd(cmdInfo);
		}

		_beginBindToRender(info, pResult);

    }

	void Material::endBindToRender(const EndBindInfo info)
	{
		if (m_onlyOnce == VG_TRUE) {
			m_bindTargetID = 0;
		}

		_endBindToRender(info);
		
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

    void Material::_beginBindToRender(const BindInfo info, BindResult *pResult)
    {
		auto &result = *pResult;
        RenderPassInfo trunkRenderPassInfo;
        trunkRenderPassInfo.pRenderPass = nullptr;
        trunkRenderPassInfo.pFrameBuffer = nullptr;			
        trunkRenderPassInfo.framebufferWidth = info.trunkFramebufferWidth;
        trunkRenderPassInfo.framebufferHeight = info.trunkFramebufferHeight;
        trunkRenderPassInfo.projMatrix = *(info.pProjMatrix);
        trunkRenderPassInfo.viewMatrix = *(info.pViewMatrix);
        trunkRenderPassInfo.pPass = *m_arrPasses.data();
        trunkRenderPassInfo.modelMatrix = *(info.pModelMatrix);
        trunkRenderPassInfo.pMesh = info.pMesh;
        trunkRenderPassInfo.subMeshIndex = info.subMeshIndex;
        trunkRenderPassInfo.viewport = fd::Viewport();
        trunkRenderPassInfo.scissor = info.hasClipRect ? info.clipRect : fd::Rect2D();
        CmdInfo cmdInfo;
        cmdInfo.pRenderPassInfo = &trunkRenderPassInfo;
        result.pTrunkRenderPassCmdBuffer->addCmd(cmdInfo);
        
	}
	
	void Material::_endBindToRender(const EndBindInfo info)
	{

	}
}