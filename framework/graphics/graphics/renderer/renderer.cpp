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
        , Bool32 preDepth
        , PostRender * pPostRender
        )
        : pScene(pScene)
        , pCamera(pCamera)
        , preDepth(preDepth)
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

    Renderer::Renderer(const RendererTarget * pRendererTarget)
        : Base(BaseType::RENDERER)
        , m_pRendererTarget()
        , m_pipelineCache()
        , m_trunkRenderPassCmdBuffer()
        , m_trunkWaitBarrierCmdBuffer()
        , m_branchCmdBuffer()
        , m_framebufferWidth(0u)
        , m_framebufferHeight(0u)
        , m_renderBinder()
        //pre z
        , m_preDepthEnable(VG_FALSE)
        , m_pPreDepthTarget()
        , m_pPreDepthCmdBuffer()
        //post render
        , m_postRenderEnable(VG_FALSE)
        , m_pPostRenderTarget()
        , m_pPostRenderCmdbuffer()

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , m_preparingRenderCostTimer(fd::CostTimer::TimerType::AVERAGE)
#endif //DEBUG and VG_ENABLE_COST_TIMER
    {
        setRendererTarget(pRendererTarget);
        _createCommandPool();
        _createCommandBuffer();
        //_createFence();
    }

    Renderer::~Renderer()
    {
    }

    const RendererTarget * Renderer::getRendererTarget() const
    {
        return m_pRendererTarget;
    }

    void Renderer::setRendererTarget(const RendererTarget * pRendererTarget)
    {
        if (m_pRendererTarget != pRendererTarget) {
            m_pRendererTarget = pRendererTarget;
            uint32_t framebufferWidth = 0u;
            uint32_t framebufferHeight = 0u;
            if (pRendererTarget != nullptr) {
                framebufferWidth = pRendererTarget->getFramebufferWidth();
                framebufferHeight = pRendererTarget->getFramebufferHeight();
            }
            if (m_framebufferWidth != framebufferWidth ||
                    m_framebufferHeight != framebufferHeight)
            {
                _resetFramebufferSize(framebufferWidth, framebufferHeight);
            }
        }
        if (m_pPostRenderTarget != nullptr)
            m_pPostRenderTarget->setClearValues(pRendererTarget->getClearValues()
                , pRendererTarget->getClearValueCount());
    }

    void Renderer::enablePreDepth()
    {
        if (m_preDepthEnable == VG_FALSE)
        {
            m_preDepthEnable = VG_TRUE;
            if (m_framebufferWidth != 0u && m_framebufferHeight != 0u)
            {
                _createPreDepthObjs();
            }
        }
    }

    void Renderer::disablePreDepth()
    {
        if (m_preDepthEnable == VG_TRUE)
        {
            m_preDepthEnable = VG_FALSE;
            _destroyPreDepthObjs();
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
        if (m_preDepthEnable) 
        {
            if (width != 0u && height != 0u)
            {
                _createPreDepthObjs();
            }
            else
            {
                _destroyPreDepthObjs();
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
    }

    void Renderer::_renderBegin(const RenderInfo & info, RenderResultInfo & resultInfo)
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        m_preparingRenderCostTimer.begin();

        fd::CostTimer renderBeginCostTimer(fd::CostTimer::TimerType::ONCE);
        renderBeginCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        m_pipelineCache.begin();
        m_renderBinder.begin();
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
            _renderScene(sceneInfo, i == 0, resultInfo);
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

        m_pipelineCache.end();
        m_renderBinder.end();
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
    }

    void Renderer::_renderScene(const SceneInfo &sceneInfo, Bool32 isFirstScene, RenderResultInfo &resultInfo)
    {
        _renderSceneLights(sceneInfo, resultInfo);
        _renderSceneVisualization(sceneInfo, isFirstScene, resultInfo);
    }

    //Render scene light depth
    void Renderer::_renderSceneLights(const SceneInfo &sceneInfo, RenderResultInfo &resultInfo)
    {
        const auto pScene = sceneInfo.pScene;
        
    }

    void Renderer::_renderSceneVisualization(const SceneInfo &sceneInfo, Bool32 isFirstScene, RenderResultInfo &resultInfo)
    {
        auto pScene = sceneInfo.pScene;
        auto pCamera = sceneInfo.pCamera;
        auto pPostRender = sceneInfo.pPostRender;
        Bool32 preDepthEnable = m_preDepthEnable == VG_TRUE && sceneInfo.preDepth == VG_TRUE;
        Bool32 postRenderEnable = m_postRenderEnable == VG_TRUE && 
            sceneInfo.pPostRender != nullptr &&
            sceneInfo.pPostRender->isValidBindToRender() == VG_TRUE;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer preparingSceneCostTimer(fd::CostTimer::TimerType::ONCE);
        preparingSceneCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        if (preDepthEnable)
        {
            m_pPreDepthCmdBuffer->begin();
        }
        m_branchCmdBuffer.begin();
        m_trunkWaitBarrierCmdBuffer.begin();                        
        m_trunkRenderPassCmdBuffer.begin();
        if (postRenderEnable)
        {
            m_pPostRenderCmdbuffer->begin();
        }

        RenderBinderInfo bindInfo = {
            isFirstScene,
            preDepthEnable,
            postRenderEnable,
            pScene,
            pCamera->getProjectorBase(),
            postRenderEnable ? pPostRender : nullptr,
            preDepthEnable ? m_pPreDepthTarget.get() : nullptr,
            postRenderEnable ? m_pPostRenderTarget.get() : nullptr,
            m_pRendererTarget,
            preDepthEnable ? m_pPreDepthTarget->getDepthTargetTexture() : nullptr,
            postRenderEnable ? m_pPostRenderTarget->getColorTargetTexture() : nullptr,
            preDepthEnable ? m_pPreDepthCmdBuffer.get() : nullptr,
            &m_branchCmdBuffer,
            &m_trunkWaitBarrierCmdBuffer,
            &m_trunkRenderPassCmdBuffer,
            postRenderEnable ? m_pPostRenderCmdbuffer.get() : nullptr,
        };

        m_renderBinder.bind(bindInfo);

        CMDParser::ResultInfo cmdParseResult;
         
        //record ...
        // pre z
        if (preDepthEnable)
        {
            CMDParser::record(m_pPreDepthCmdBuffer.get()
                , m_pCommandBuffer.get()
                , &m_pipelineCache
                , &cmdParseResult
                );
            resultInfo.drawCount += cmdParseResult.drawCount;
        }
        //branch render pass.
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
        CMDParser::record(&m_trunkRenderPassCmdBuffer
            , m_pCommandBuffer.get()
            , &m_pipelineCache
            , &cmdParseResult
            );
        resultInfo.drawCount += cmdParseResult.drawCount;
        //post render record
        if (postRenderEnable)
        {
            CMDParser::record(m_pPostRenderCmdbuffer.get()
                , m_pCommandBuffer.get()
                , &m_pipelineCache
                , &cmdParseResult
                );
            resultInfo.drawCount += cmdParseResult.drawCount;
        }

        if (preDepthEnable)
        {
            m_pPreDepthCmdBuffer->end();
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

    void Renderer::_createPreDepthObjs()
    {
        m_pPreDepthTarget = std::shared_ptr<RendererPreDepthTarget>{
            new RendererPreDepthTarget{
                m_framebufferWidth,
                m_framebufferHeight,
            }
        };

        m_pPreDepthCmdBuffer = std::shared_ptr<CmdBuffer>{
            new CmdBuffer{}
        };
    }

    void Renderer::_destroyPreDepthObjs()
    {
        m_pPreDepthTarget = nullptr;
        m_pPreDepthCmdBuffer = nullptr;
    }

    void Renderer::_createPostRenderObjs()
    {
        m_pPostRenderTarget = std::shared_ptr<RendererPostRenderTarget>{
            new RendererPostRenderTarget{
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