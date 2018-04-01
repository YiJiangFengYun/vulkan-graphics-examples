#include "graphics/material/cmd.hpp"

namespace vg
{
    CmdInfo::CmdInfo()
        : pRenderPassInfo(nullptr)
        , pBarrierInfo(nullptr)
    {

    }

    BarrierInfo::BarrierInfo(vk::PipelineStageFlags srcStageMask
        , vk::PipelineStageFlags  dstStageMask)
        : srcStageMask(srcStageMask)
        , dstStageMask(dstStageMask)
    {

    }

    RenderPassInfo::RenderPassInfo( vk::RenderPass *pRenderPass
        , vk::Framebuffer *pFrameBuffer
        , uint32_t framebufferWidth
        , uint32_t framebufferHeight
        , fd::Rect2D renderArea
        , uint32_t clearValueCount
        , vk::ClearValue *pClearValues
        , Matrix4x4 projMatrix
        , Matrix4x4 viewMatrix
        , Pass *pPass
        , Matrix4x4 modelMatrix
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
        : pRenderPass(pRenderPass)
        , pFrameBuffer(pFrameBuffer)
	, framebufferWidth(framebufferWidth)
	, framebufferHeight(framebufferHeight)
        , renderArea(renderArea)
        , clearValueCount(clearValueCount)
        , pClearValues(pClearValues)
        , projMatrix(projMatrix)
        , viewMatrix(viewMatrix)
        , pPass(pPass)
        , modelMatrix(modelMatrix)
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


    CmdBuffer::CmdBuffer()
        : m_cmdInfoCount(0u)
        , m_cmdInfoCapacity(0u)
        , m_cmdInfos()
        , m_renderPassInfoCount(0u)
        , m_renderPassInfoCapacity(0u)
        , m_renderPassInfos()
		, m_renderPassInfoToCmdInfoIndices()
        , m_barrierInfoCount(0u)
        , m_barrierInfosCapacity(0u)
        , m_barrierInfos()
		, m_barrierInfoToCmdInfoIndices()
    {    
    }    
    uint32_t CmdBuffer::getCmdCount() const
    {
        return m_cmdInfoCount;
    }    
    const CmdInfo *CmdBuffer::getCmdInfos() const
    {
        return m_cmdInfos.data();
    }    
    void CmdBuffer::begin()
    {
        m_cmdInfoCount =  0u;
        m_renderPassInfoCount = 0u;
        m_barrierInfoCount = 0u;
    }    
    void CmdBuffer::addCmd(CmdInfo cmdInfo)
    {
        if (m_cmdInfoCount == m_cmdInfoCapacity)
        {
            m_cmdInfoCapacity = _getNextCapacity(m_cmdInfoCapacity);
            m_cmdInfos.resize(m_cmdInfoCapacity);
        }

		uint32_t cmdInfoIndex = m_cmdInfoCount;
        auto &dstCmdInfo = m_cmdInfos[cmdInfoIndex];
        dstCmdInfo = cmdInfo;
        ++m_cmdInfoCount;

        //copy render pass info
        if (cmdInfo.pRenderPassInfo != nullptr)
        {
            if (m_renderPassInfoCount == m_renderPassInfoCapacity)
            {
                m_renderPassInfoCapacity = _getNextCapacity(m_renderPassInfoCapacity);
                m_renderPassInfos.resize(m_renderPassInfoCapacity);
				m_renderPassInfoToCmdInfoIndices.resize(m_renderPassInfoCapacity);

				uint32_t count = m_renderPassInfoCount;
				for (uint32_t i = 0; i < count; ++i)
				{
					auto &cmdInfo = m_cmdInfos[m_renderPassInfoToCmdInfoIndices[i]];
					cmdInfo.pRenderPassInfo = &(m_renderPassInfos[i]);
				}
            }
    
            
            auto &srcRenderPassInfo = *(cmdInfo.pRenderPassInfo);
            auto &dstRenderPassInfo = m_renderPassInfos[m_renderPassInfoCount];
            dstRenderPassInfo = srcRenderPassInfo;
           
            dstCmdInfo.pRenderPassInfo = &dstRenderPassInfo;
			m_renderPassInfoToCmdInfoIndices[m_renderPassInfoCount] = cmdInfoIndex;

			++m_renderPassInfoCount;
        }

        //copy barrier info
        if (cmdInfo.pBarrierInfo != nullptr)
        {
            if (m_barrierInfoCount == m_barrierInfosCapacity)
            {
                m_barrierInfosCapacity = _getNextCapacity(m_barrierInfosCapacity);
                m_barrierInfos.resize(m_barrierInfosCapacity);
				m_barrierInfoToCmdInfoIndices.resize(m_barrierInfosCapacity);

				uint32_t count = m_barrierInfoCount;
				for (uint32_t i = 0; i < count; ++i)
				{
					auto &cmdInfo = m_cmdInfos[m_barrierInfoToCmdInfoIndices[i]];
					cmdInfo.pBarrierInfo = &(m_barrierInfos[i]);
				}
            }

            auto &srcBarrierInfo = *(cmdInfo.pBarrierInfo);
            auto &dstBarrierInfo = m_barrierInfos[m_barrierInfoCount];
            dstBarrierInfo = srcBarrierInfo;

            dstCmdInfo.pBarrierInfo = &dstBarrierInfo;
			m_barrierInfoToCmdInfoIndices[m_barrierInfoCount] = cmdInfoIndex;

			++m_barrierInfoCount;

        }
    }
    
    void CmdBuffer::end()
    {    
    }

    uint32_t CmdBuffer::_getNextCapacity(uint32_t current)
    {
        if (current == 0u) return 1u;
        else return static_cast<uint32_t>(std::pow(2, std::log2(current) + 1));
    }
} //vg