#include "graphics/renderer/renderer.hpp"

#include "graphics/util/util.hpp"
#include "graphics/buffer_data/util.hpp"
#include "graphics/util/gemo_util.hpp"
#include "graphics/renderer/cmd_parser.hpp"

#define USE_WORLD_BOUNDS

namespace vg
{
    Renderer::SceneInfo::SceneInfo(BaseScene *pScene
        , BaseCamera *pCamera
        , Bool32 preZ
        , PostRender * pPostRender
        )
        : pScene(pScene)
        , pCamera(pCamera)
        , preZ(preZ)
        , pPostRender(pPostRender)
    {

    }

    Renderer::RenderInfo::RenderInfo(uint32_t sceneInfoCount
        , const SceneInfo *pSceneInfos
        , uint32_t waitSemaphoreCount
        , const vk::Semaphore* pWaitSemaphores
        , const vk::PipelineStageFlags* pWaitDstStageMask
        , uint32_t signalSemaphoreCount
        , const vk::Semaphore* pSignalSemaphores
        )
        : sceneInfoCount(sceneInfoCount)
        , pSceneInfos(pSceneInfos)
        , waitSemaphoreCount(waitSemaphoreCount)
        , pWaitSemaphores(pWaitSemaphores)
        , pWaitDstStageMask(pWaitDstStageMask)
        , signalSemaphoreCount(signalSemaphoreCount)
        , pSignalSemaphores(pSignalSemaphores)
    {
    }

    Renderer::RenderResultInfo::RenderResultInfo(Bool32 isRendered
        , uint32_t drawCount
        )
        : isRendered(isRendered)
        , drawCount(drawCount)
    {

    }

    Renderer::Renderer(const RenderTarget * pRenderTarget)
        : Base(BaseType::RENDERER)
        , m_pRenderTarget()
        , m_pipelineCache()
        , m_trunkRenderPassCmdBuffer()
        , m_trunkWaitBarrierCmdBuffer()
        , m_branchCmdBuffer()
        , m_framebufferWidth(0u)
        , m_framebufferHeight(0u)
        , m_renderBinder()
        //pre z
        , m_preZEnable(VG_FALSE)
        , m_pPreZTarget()
        , m_pPreZCmdBuffer()
        //post render
        , m_postRenderEnable(VG_FALSE)
        , m_pPostRenderTarget()
        , m_pPostRenderCmdbuffer()

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , m_preparingRenderCostTimer(fd::CostTimer::TimerType::AVERAGE)
#endif //DEBUG and VG_ENABLE_COST_TIMER
    {
        setRenderTarget(pRenderTarget);
        _createCommandPool();
        _createCommandBuffer();
        //_createFence();
    }

    Renderer::~Renderer()
    {
    }

    const RenderTarget * Renderer::getRenderTarget() const
    {
        return m_pRenderTarget;
    }

    void Renderer::setRenderTarget(const RenderTarget * pRenderTarget)
    {
        if (m_pRenderTarget != pRenderTarget) {
            m_pRenderTarget = pRenderTarget;
            uint32_t framebufferWidth = 0u;
            uint32_t framebufferHeight = 0u;
            if (pRenderTarget != nullptr) {
                framebufferWidth = pRenderTarget->getFramebufferWidth();
                framebufferHeight = pRenderTarget->getFramebufferHeight();
            }
            if (m_framebufferWidth != framebufferWidth ||
                    m_framebufferHeight != framebufferHeight)
            {
                _resetFramebufferSize(framebufferWidth, framebufferHeight);
            }
        }
        if (m_pPostRenderTarget != nullptr)
            m_pPostRenderTarget->setClearValues(pRenderTarget->getClearValues()
                , pRenderTarget->getClearValueCount());
    }

    void Renderer::enablePreZ()
    {
        if (m_preZEnable == VG_FALSE)
        {
            m_preZEnable = VG_TRUE;
            if (m_framebufferWidth != 0u && m_framebufferHeight != 0u)
            {
                _createPreZObjs();
            }
        }
    }

    void Renderer::disablePreZ()
    {
        if (m_preZEnable == VG_TRUE)
        {
            m_preZEnable = VG_FALSE;
            _destroyPreZObjs();
        }
    }

    void Renderer::enablePostRender()
    {
        if (m_postRenderEnable == VG_FALSE)
        {
            m_postRenderEnable = VG_TRUE;
            if (m_framebufferWidth != 0u && m_framebufferHeight != 0u)
            {
                _createPostRenderObjs();
            }
        }
    }

    void Renderer::disablePostRender()
    {
        if (m_postRenderEnable == VG_TRUE)
        {
            m_postRenderEnable = VG_FALSE;
            _destroyPostRenderObjs();
        }
    }

    Bool32 Renderer::isValidForRender() const
    {
        return _isValidForRender();
    }

    void Renderer::render(const RenderInfo &info, 
        RenderResultInfo &resultInfo)
    {
        _preRender();
        _renderBegin(info, resultInfo);
        _render(info, resultInfo);
        _renderEnd(info, resultInfo);
        _postRender();
    }

    // Renderer::PreObjectRecordingFun Renderer::setPreObjectRecordingCallBack(PreObjectRecordingFun cbFun)
    // {
    //     auto oldFun = m_preObjectRecordingFun;
    //     m_preObjectRecordingFun = cbFun;
    //     return oldFun;
    // }
        
    // Renderer::PostObjectRecordingFun Renderer::setPostObjectRecordingCallBack(PostObjectRecordingFun cbFun)
    // {
    //     auto oldFun = m_postObjectRecordingFun;
    //     m_postObjectRecordingFun = cbFun;
    //     return oldFun;
    // }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
    const fd::CostTimer &Renderer::getPreparingRenderCostTimer() const
    {
        return m_preparingRenderCostTimer;
    }
#endif //DEBUG and VG_ENABLE_COST_TIMER

    Bool32 Renderer::_isValidForRender() const
    {
        return VG_TRUE;
    }

    void Renderer::_resetFramebufferSize(uint32_t width, uint32_t height)
    {
        m_framebufferWidth = width;
        m_framebufferHeight = height;
        m_renderBinder.setFramebufferWidth(width);
        m_renderBinder.setFramebufferHeight(height);
        if (m_preZEnable) 
        {
            if (width != 0u && height != 0u)
            {
                _createPreZObjs();
            }
            else
            {
                _destroyPreZObjs();
            }
        }
		if (m_postRenderEnable)
		{
			if (width != 0u && height != 0u)
			{
				_createPostRenderObjs();
			}
			else
			{
				_destroyPostRenderObjs();
			}
		}
    }

    void Renderer::_preRender()
    {
        m_pipelineCache.start();
    }

    void Renderer::_renderBegin(const RenderInfo & info, RenderResultInfo & resultInfo)
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        m_preparingRenderCostTimer.begin();

        fd::CostTimer renderBeginCostTimer(fd::CostTimer::TimerType::ONCE);
        renderBeginCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        uint32_t count = info.sceneInfoCount;
        for (uint32_t i = 0; i < count; ++i)
        {
            const auto & sceneInfo = *(info.pSceneInfos + i);
            const auto pScene = sceneInfo.pScene;
            pScene->beginRender();
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        renderBeginCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Render begin cost time: "
            << renderBeginCostTimer.costTimer
            << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void Renderer::_render(const RenderInfo &info
            , RenderResultInfo &resultInfo)
    {
        resultInfo.drawCount = 0u;

        //command buffer begin
        _recordCommandBufferForBegin();

        uint32_t count = info.sceneInfoCount;
        for (uint32_t i = 0; i < count; ++i)
        {
            if (i == 0) resultInfo.isRendered = VG_TRUE;
            const auto & sceneInfo = *(info.pSceneInfos + i);
            const auto pScene = sceneInfo.pScene;
            const auto pCamera = sceneInfo.pCamera;
            const auto pPostRender = sceneInfo.pPostRender;
            Bool32 preZEnable = m_preZEnable == VG_TRUE && sceneInfo.preZ == VG_TRUE;
            Bool32 postRenderEnable = m_postRenderEnable == VG_TRUE && 
                sceneInfo.pPostRender != nullptr &&
                sceneInfo.pPostRender->isValidBindToRender() == VG_TRUE;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer preparingSceneCostTimer(fd::CostTimer::TimerType::ONCE);
            preparingSceneCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            if (preZEnable)
            {
                m_pPreZCmdBuffer->begin();
            }
            m_branchCmdBuffer.begin();
            m_trunkWaitBarrierCmdBuffer.begin();                        
            m_trunkRenderPassCmdBuffer.begin();
            if (postRenderEnable)
            {
                m_pPostRenderCmdbuffer->begin();
            }

            //bind render pass begin.
			if (preZEnable)
            {
                //pre z cmd buffer
                const BaseRenderTarget *pRenderTarget;
                const vk::RenderPass *pRenderPass;
                const vk::Framebuffer *pFramebuffer;
                pRenderTarget = m_pPreZTarget.get();
                pRenderPass = m_pPreZTarget->getRenderPass();
                pFramebuffer = m_pPreZTarget->getFramebuffer();
                m_renderBinder.bindForRenderPassBegin(pRenderTarget
                    , pRenderPass
                    , pFramebuffer
                    , m_pPreZCmdBuffer.get()
                    );
            }

            if (postRenderEnable)
            {
                const BaseRenderTarget *pRenderTarget;
                const vk::RenderPass *pRenderPass;
                const vk::Framebuffer *pFramebuffer;

                //trunk cmd buffer
                pRenderTarget = m_pPostRenderTarget.get();
                pRenderPass = m_pPostRenderTarget->getRenderPass();
                pFramebuffer = m_pPostRenderTarget->getFramebuffer();
                m_renderBinder.bindForRenderPassBegin(pRenderTarget
                    , pRenderPass
                    , pFramebuffer
                    , &m_trunkRenderPassCmdBuffer
                    );
                
                //post render cmd buffer
                pRenderTarget = m_pRenderTarget;
                pRenderPass = i == 0 ? m_pRenderTarget->getFirstRenderPass() : m_pRenderTarget->getSecondRenderPass();
                pFramebuffer = i == 0 ? m_pRenderTarget->getFirstFramebuffer() : m_pRenderTarget->getSecondFramebuffer();
                m_renderBinder.bindForRenderPassBegin(pRenderTarget
                    , pRenderPass
                    , pFramebuffer
                    , m_pPostRenderCmdbuffer.get()
                    );
            }
            else
            {
                const BaseRenderTarget *pRenderTarget;
                const vk::RenderPass *pRenderPass;
                const vk::Framebuffer *pFramebuffer;

                //trunk cmd buffer
                pRenderTarget = m_pRenderTarget;
                pRenderPass = i == 0 ? m_pRenderTarget->getFirstRenderPass() : m_pRenderTarget->getSecondRenderPass();
                pFramebuffer = i == 0 ? m_pRenderTarget->getFirstFramebuffer() : m_pRenderTarget->getSecondFramebuffer();
                m_renderBinder.bindForRenderPassBegin(pRenderTarget
                    , pRenderPass
                    , pFramebuffer
                    , &m_trunkRenderPassCmdBuffer
                    );
            }

            m_renderBinder.bind(pScene
                , pCamera
                , preZEnable ? m_pPreZTarget.get() : nullptr
                , preZEnable ? m_pPreZCmdBuffer.get() : nullptr
                , & m_branchCmdBuffer
                , & m_trunkWaitBarrierCmdBuffer
                , & m_trunkRenderPassCmdBuffer
                , postRenderEnable ? pPostRender : nullptr
                , postRenderEnable ? m_pPostRenderTarget.get() : nullptr
                , postRenderEnable ? m_pPostRenderCmdbuffer.get() : nullptr
                );

            
            //bind render pass end.
            if (preZEnable) m_renderBinder.bindForRenderPassEnd(m_pPreZCmdBuffer.get());
            if (postRenderEnable)
            {
                //trunk cmd buffer
                m_renderBinder.bindForRenderPassEnd(&m_trunkRenderPassCmdBuffer);
                
                //post render cmd buffer
                m_renderBinder.bindForRenderPassEnd(m_pPostRenderCmdbuffer.get());
            }
            else
            {
                //trunk cmd buffer
                m_renderBinder.bindForRenderPassEnd(&m_trunkRenderPassCmdBuffer);
            }
             

            //record ...
            // pre z
            if (preZEnable)
            {
                resultInfo.drawCount += m_pPreZCmdBuffer->getCmdCount();
                CMDParser::record(m_pPreZCmdBuffer.get()
                    , m_pCommandBuffer.get()
                    , &m_pipelineCache
                    );
            }

            //branch render pass.
            CMDParser::ResultInfo cmdParseResult;
            CMDParser::record(&m_branchCmdBuffer,
                m_pCommandBuffer.get(),
                &m_pipelineCache,
                &cmdParseResult
                );
            resultInfo.drawCount += cmdParseResult.drawCount;

            //trunk wait barrier
            CMDParser::recordTrunkWaitBarrier(&m_trunkWaitBarrierCmdBuffer,
                m_pCommandBuffer.get());

            //trunk render pass.
            resultInfo.drawCount += m_trunkRenderPassCmdBuffer.getCmdCount();
            CMDParser::record(&m_trunkRenderPassCmdBuffer
                , m_pCommandBuffer.get()
                , &m_pipelineCache
                );
            //post render record
            if (postRenderEnable)
            {
                CMDParser::record(m_pPostRenderCmdbuffer.get()
                    , m_pCommandBuffer.get()
                    , &m_pipelineCache
                    );
            }      

            if (preZEnable)
            {
                m_pPreZCmdBuffer->end();
            }
            m_trunkRenderPassCmdBuffer.end();
            m_trunkWaitBarrierCmdBuffer.end();
            m_branchCmdBuffer.end();
            if (postRenderEnable)
            {
                m_pPostRenderCmdbuffer->end();
            }
    
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            preparingSceneCostTimer.end();
            VG_COST_TIME_LOG(plog::debug) << "Preparing scene cost time: " 
                << preparingSceneCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
        }

        //command buffer end
        _recordCommandBufferForEnd();

    }

    void Renderer::_renderEnd(const RenderInfo & info, RenderResultInfo & resultInfo)
    {

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer renderEndCostTimer(fd::CostTimer::TimerType::ONCE);
        renderEndCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER    

        /*auto pDevice = pApp->getDevice();
        pDevice->waitForFences(*m_waitFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
        pDevice->resetFences(*m_waitFence);*/
        

        vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };        
        //submit        
        vk::SubmitInfo submitInfo = {
            info.waitSemaphoreCount,              //waitSemaphoreCount
            info.pWaitSemaphores,                 //pWaitSemaphores
            waitStages,                           //pWaitDstStageMask
            1u,                                   //commandBufferCount
            m_pCommandBuffer.get(),               //pCommandBuffers
            info.signalSemaphoreCount,            //signalSemaphoreCount
            info.pSignalSemaphores,               //pSignalSemaphores
        };

        VG_LOG(plog::debug) << "Pre submit to grahics queue." << std::endl;
        vk::Queue queue;
        uint32_t queueIndex;
        pApp->allocateGaphicsQueue(queueIndex, queue);
        queue.submit(submitInfo, nullptr);
        //queue.submit(submitInfo, *m_waitFence);
        pApp->freeGraphicsQueue(queueIndex);
        VG_LOG(plog::debug) << "Post submit to grahics queue." << std::endl;

        uint32_t count = info.sceneInfoCount;
        for (uint32_t i = 0; i < count; ++i)
        {
            const auto & sceneInfo = *(info.pSceneInfos + i);
            const auto pScene = sceneInfo.pScene;
            pScene->endRender();
        }
        
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        renderEndCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Render end cost time: " 
                << renderEndCostTimer.costTimer 
                << std::endl;

        m_preparingRenderCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Average cost time of preparing render: " << m_preparingRenderCostTimer.costTimer << "ms." << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void Renderer::_postRender()
    {
        m_pipelineCache.end();
    }

    void Renderer::_recordCommandBufferForBegin()
    {
        vk::CommandBufferBeginInfo beginInfo = {
            vk::CommandBufferUsageFlagBits::eSimultaneousUse,  //flags
            nullptr                                            //pInheritanceInfo
        };

        m_pCommandBuffer->begin(beginInfo);
        VG_LOG(plog::debug) << "Post begin command buffer for render." << std::endl;

        
    }

    void Renderer::_recordCommandBufferForEnd()
    {
        
        VG_LOG(plog::debug) << "Pre end command buffer." << std::endl;
        m_pCommandBuffer->end();
        VG_LOG(plog::debug) << "Post end command buffer." << std::endl;
    }

    /*void Renderer::_createFence()
    {
        if (m_waitFence != nullptr) return;
        auto pDevice = pApp->getDevice();
        vk::FenceCreateInfo createInfo;
        createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
        m_waitFence = fd::createFence(pDevice, createInfo);
    }*/

    void Renderer::_createCommandPool()
    {
        if (m_pCommandPool != nullptr) return;
        auto pDevice = pApp->getDevice();
        auto graphicsFamily = pApp->getGraphicsFamily();
        vk::CommandPoolCreateInfo createInfo = {
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            graphicsFamily
        };
        m_pCommandPool = fd::createCommandPool(pDevice, createInfo);
    }

    void Renderer::_createCommandBuffer()
    {
        if (m_pCommandBuffer != nullptr) return;
        auto pCommandPool = m_pCommandPool;
        vk::CommandBufferAllocateInfo allocateInfo = {
            *pCommandPool,                             //commandPool
            vk::CommandBufferLevel::ePrimary,          //level
            1u                                         //commandBufferCount
        };

        auto pDevice = pApp->getDevice();

        VG_LOG(plog::debug) << "Pre allocate command buffer from pool." << std::endl;
        m_pCommandBuffer = fd::allocateCommandBuffer(pDevice, pCommandPool.get(), allocateInfo);
        VG_LOG(plog::debug) << "Post allocate command buffer from pool." << std::endl;
    }

    void Renderer::_createPreZObjs()
    {
        m_pPreZTarget = std::shared_ptr<PreZTarget>{
            new PreZTarget{
                m_framebufferWidth,
                m_framebufferHeight,
            }
        };

        m_pPreZCmdBuffer = std::shared_ptr<CmdBuffer>{
            new CmdBuffer{}
        };
    }

    void Renderer::_destroyPreZObjs()
    {
        m_pPreZTarget = nullptr;
        m_pPreZCmdBuffer = nullptr;
    }

    void Renderer::_createPostRenderObjs()
    {
        m_pPostRenderTarget = std::shared_ptr<PostRenderTarget>{
            new PostRenderTarget{
                m_framebufferWidth,
                m_framebufferHeight,
            }
        };
        m_pPostRenderCmdbuffer = std::shared_ptr<CmdBuffer>{
            new CmdBuffer{}
        };
    }

    void Renderer::_destroyPostRenderObjs()
    {
        m_pPostRenderTarget = nullptr;
        m_pPostRenderCmdbuffer = nullptr;
    }

}