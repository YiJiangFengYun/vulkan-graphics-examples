#include "graphics/renderer/render_binder.hpp"

#include "graphics/util/gemo_util.hpp"
#include "graphics/scene/light_3.hpp"

namespace vg
{
    void fillValidVisualObjects(std::vector<VisualObject<SpaceType::SPACE_2> *> &arrPVObjs
        , uint32_t &PVObjIndex
        , const Transform<SpaceType::SPACE_2> *pTransform
        , Scene<SpaceType::SPACE_2> *pScene
        , const Projector<SpaceType::SPACE_2> *pProjector
#ifdef USE_WORLD_BOUNDS
        , const fd::Bounds<SpaceTypeInfo<SpaceType::SPACE_2>::PointType> *pBounds
#endif
        );

    RenderBinderInfo::RenderBinderInfo(Bool32 lightingEnable
        , Bool32 shadowEnable
        , Bool32 preDepthEnable
        , Bool32 postRenderEnable

        , Bool32 firstScene
        , BaseScene *pScene
        , const BaseProjector *pProjector
        , PostRender *pPostRender

        , const PreDepthTarget *pPreDepthTarget
        , const PostRenderTarget *pPostRenderTarget
        , const MultiRenderTarget *pRendererTarget

        , const Texture *pPreDepthResultTex
        , const Texture *pPostRenderTex

        , CmdBuffer *pLightDepthCmdBuffer
        , CmdBuffer *pPreDepthCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pPostRenderCmdBuffer
        )
        : lightingEnable(lightingEnable)
        , shadowEnable(shadowEnable) 
        , preDepthEnable(preDepthEnable)
        , postRenderEnable(postRenderEnable)

        , firstScene(firstScene)
        , pScene(pScene)
        , pProjector(pProjector)
        , pPostRender(pPostRender)

        , pPreDepthTarget(pPreDepthTarget)
        , pPostRenderTarget(pPostRenderTarget)
        , pRendererTarget(pRendererTarget)

        , pPreDepthResultTex(pPreDepthResultTex)
        , pPostRenderTex(pPostRenderTex)

        , pLightDepthCmdBuffer(pLightDepthCmdBuffer)
        , pPreDepthCmdBuffer(pPreDepthCmdBuffer)
        , pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pPostRenderCmdBuffer(pPostRenderCmdBuffer)
    {

    }

    RenderBinder::RenderBinder()
        : m_bindedObjectsForLighting()
        , m_bindedObjectCountForLighting(0u)
        , m_bindedObjectsForPreDepth()
        , m_bindedObjectCountForPreDepth(0u)
        , m_bindedObjects()
        , m_bindedObjectCount(0u)
        //light data buffer
        , m_lightDataBufferCache([](const vg::InstanceID &sceneID) {
            return std::shared_ptr<BufferData>{new BufferData(vk::BufferUsageFlagBits::eUniformBuffer
                , vk::MemoryPropertyFlagBits::eHostVisible)
            };
        })
        , m_lightTypeCount()
        , m_pCurrLightDataBuffer()
        , m_lightPassTextureInfos()
        , m_lightTextureInfos()
    {}

    void RenderBinder::begin()
    {
        m_lightDataBufferCache.begin();
    }

    void RenderBinder::bind(RenderBinderInfo info)
    {
        _beginBind();
        _bind(info);
        _endBind();
    }

    void RenderBinder::end()
    {
        m_lightDataBufferCache.end();
    }

    void RenderBinder::_beginBind()
    {
        m_bindedObjectCount = 0u;
    }

    void RenderBinder::_endBind()
    {
        for (uint32_t i = 0; i < m_bindedObjectCountForLighting; ++i)
        {
            m_bindedObjectsForLighting[i]->endBind();
        }
        m_bindedObjectCountForLighting = 0u;

        for (uint32_t i = 0; i < m_bindedObjectCountForPreDepth; ++i)
        {
            m_bindedObjectsForPreDepth[i]->endBind();
        }
        m_bindedObjectCountForPreDepth = 0u;

        for (uint32_t i = 0; i < m_bindedObjectCount; ++i) 
        {
            m_bindedObjects[i]->endBind();
        }
        m_bindedObjectCount = 0u;
    }

    void RenderBinder::_bind(RenderBinderInfo info)
    {
        m_lightingEnable = info.lightingEnable;
        m_shadowEnable = info.shadowEnable;
        if (info.lightingEnable == VG_TRUE)
        {
            _syncLightData(info.pScene);
        }
        _bindForRenderPassBegin(info);
        //scene make cmds.
        if (info.pScene->getSpaceType() == SpaceType::SPACE_2)
        {
            _bindScene2(nullptr,
                dynamic_cast<Scene<SpaceType::SPACE_2> *>(info.pScene), 
                dynamic_cast<const Projector<SpaceType::SPACE_2> *>(info.pProjector),
                info.preDepthEnable ? info.pPreDepthTarget : nullptr,
                info.postRenderEnable ? 
                    dynamic_cast<const BaseRenderTarget *>(info.pPostRenderTarget) : 
                    dynamic_cast<const BaseRenderTarget *>(info.pRendererTarget),
                info.preDepthEnable ? info.pPreDepthResultTex : nullptr,
                info.preDepthEnable ? info.pPreDepthCmdBuffer : nullptr,
                info.pBranchCmdBuffer,                    
                info.pTrunkWaitBarrierCmdBuffer,
                info.pTrunkRenderPassCmdBuffer
                );
        } 
        else if (info.pScene->getSpaceType() == SpaceType::SPACE_3)
        {
            if (info.lightingEnable == VG_TRUE && info.shadowEnable == VG_TRUE)
            {
                _bindForLightDepth(dynamic_cast<Scene<SpaceType::SPACE_3> *>(info.pScene),
                    dynamic_cast<const Projector<SpaceType::SPACE_3> *>(info.pProjector),
                    info.pLightDepthCmdBuffer
                );
            }
            _bindScene3(nullptr,
                dynamic_cast<Scene<SpaceType::SPACE_3> *>(info.pScene), 
                dynamic_cast<const Projector<SpaceType::SPACE_3> *>(info.pProjector),
                info.preDepthEnable ? info.pPreDepthTarget : nullptr,
                info.postRenderEnable ? 
                    dynamic_cast<const BaseRenderTarget *>(info.pPostRenderTarget) : 
                    dynamic_cast<const BaseRenderTarget *>(info.pRendererTarget),
                info.preDepthEnable ? info.pPreDepthResultTex : nullptr,
                info.preDepthEnable ? info.pPreDepthCmdBuffer : nullptr,
                info.pBranchCmdBuffer,                    
                info.pTrunkWaitBarrierCmdBuffer,
                info.pTrunkRenderPassCmdBuffer
                );
        } 
        else 
        {
            //todo
        }
        //post render make cmds.
        if (info.postRenderEnable)
        {
            auto pMaterial = info.pPostRender->getMaterial();
            auto passCount = pMaterial->getPassCount();
            for (uint32_t i = 0; i < passCount; ++i)
            {
                auto pPass = pMaterial->getPassWithIndex(i);
                auto pColorTex = info.pPostRenderTex;
                vg::PassTextureInfo::TextureInfo itemInfo = {
                    pColorTex,
                    nullptr,
                    nullptr,
                    vk::ImageLayout::eUndefined,
                };
                PassTextureInfo info = {
                    vg::SamplerTextureType::TEX_2D,
                    1u,
                    &itemInfo,
                    VG_PASS_POST_RENDER_TEXTURE_BINDING_PRIORITY,
                    vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
                    vk::ShaderStageFlagBits::eFragment,
                };
                if (pPass->hasTexture(VG_PASS_POST_RENDER_TEXTURE_NAME) == VG_FALSE)
                {
                    pPass->addTexture(VG_PASS_POST_RENDER_TEXTURE_NAME, info);
                }
                else
                {
                    pPass->setTexture(VG_PASS_POST_RENDER_TEXTURE_NAME, info);
                }
                pPass->apply();
            }

            ;
            PostRender::BindInfo bindInfo = {
                info.pRendererTarget->getFramebufferWidth(),
                info.pRendererTarget->getFramebufferHeight(),
            };
            PostRender::BindResult result = {
                info.pPostRenderCmdBuffer,
            };
            
            info.pPostRender->beginBind(bindInfo, &result);
        }
        //post render end bind
        if (info.postRenderEnable)
        {
            info.pPostRender->endBind();
        }

        _bindForRenderPassEnd(info);
    }

    void RenderBinder::_bindForLightDepth(Scene<SpaceType::SPACE_3> *pScene
        , const Projector<SpaceType::SPACE_3> *pProjector
        , CmdBuffer *pPreDepthCmdBuffer
        )
    {

    VG_LOG(plog::debug) << "Begin to bind for light depth." << std::endl;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer bindForLightDepthCostTimer(fd::CostTimer::TimerType::ONCE);
        fd::CostTimer exportDepthRenderInfoCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        bindForLightDepthCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        uint32_t lightCount = pScene->getLightCount();
        for (uint32_t i = 0u; i < lightCount; ++i) {
           auto *pLight = pScene->getLightWithIndex(i);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        exportDepthRenderInfoCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

           auto depthRenderInfo = pLight->getDepthRenderInfo();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        exportDepthRenderInfoCostTimer.end();
        fd::CostTimer oneLightBindCosttimer(fd::CostTimer::TimerType::ONCE);
        oneLightBindCosttimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        
           for (uint32_t j = 0; j < depthRenderInfo.renderCount; ++j) {
                const PreDepthTarget *pRenderTarget = dynamic_cast<const PreDepthTarget *>(*(depthRenderInfo.pDepthTargets + j));
                _renderPassBegin(pRenderTarget
                 , pRenderTarget->getRenderPass()
                 , pRenderTarget->getFramebuffer()
                 , pPreDepthCmdBuffer
                 );
                _bindScene3(pLight
                    , pScene
                    , dynamic_cast<const Projector<SpaceType::SPACE_3> *>(*(depthRenderInfo.pProjectors + j))
                    , nullptr
                    , pRenderTarget
                    , nullptr
                    , nullptr
                    , nullptr
                    , nullptr
                    , pPreDepthCmdBuffer
                    );
                _renderPassEnd(pPreDepthCmdBuffer);
           }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        oneLightBindCosttimer.end();
        VG_COST_TIME_LOG(plog::debug) << "One light Bind cost time: " 
                << oneLightBindCosttimer.costTimer << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        bindForLightDepthCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Export light depth render info cost time: " 
                << exportDepthRenderInfoCostTimer.costTimer << std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Bind for light depth cost time: " 
                << bindForLightDepthCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        VG_LOG(plog::debug) << "End to bind for light depth." << std::endl;
    }

    void RenderBinder::_syncLightData(BaseScene *pScene)
    {
    VG_LOG(plog::debug) << "Begin to sync light data." << std::endl;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer syncLightDataCostTimer(fd::CostTimer::TimerType::ONCE);
        syncLightDataCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        BufferData *pLightDataBuffer = m_lightDataBufferCache.caching(pScene->getID()).get();
        m_pCurrLightDataBuffer = pLightDataBuffer; 

        auto arrRegisteredLights = pScene->getArrRegisteredLights();
        auto mapReigsteredLights = pScene->getMapRegisteredLights();

        m_lightTypeCount = static_cast<uint32_t>(arrRegisteredLights.size());

        //copy and sort array.
        std::sort(arrRegisteredLights.begin(), arrRegisteredLights.end(), [&](const std::type_info *item1, const std::type_info *item2) {
            const auto &lightInfo1 = mapReigsteredLights[std::type_index(*item1)];
            const auto &lightInfo2 = mapReigsteredLights[std::type_index(*item2)];
            return lightInfo1.bindingPriority - lightInfo2.bindingPriority;
        });

        uint32_t vectorSize = static_cast<uint32_t>(sizeof(Vector4));
        //count total size of light data.        
        //count total texture binding count.
        uint32_t totalSize = 0u;
        uint32_t totalTextureBindingCount = 0u;
        for (const auto pLightTypeInfo : arrRegisteredLights)
        {
            const auto &lightInfo = mapReigsteredLights[std::type_index(*pLightTypeInfo)];
            uint32_t maxLightCount = lightInfo.maxCount;
            uint32_t textureCount = lightInfo.textureCount;
            //space for count variable.
            totalSize += vectorSize;
            //space for data of this type lights.
            totalSize += lightInfo.dataSize * maxLightCount;
            totalTextureBindingCount += textureCount;
        }

        if (static_cast<uint32_t>(m_lightTextureInfos.size()) < totalTextureBindingCount) 
        {
            m_lightTextureInfos.resize(static_cast<uint32_t>(totalTextureBindingCount));
        }
        if (static_cast<uint32_t>(m_lightPassTextureInfos.size()) < totalTextureBindingCount)
        {
            m_lightPassTextureInfos.resize(static_cast<uint32_t>(totalTextureBindingCount));
        }
        
        //Allocate memory and copy ligth data to it.
        std::vector<Byte> memory(totalSize);
        uint32_t offset = 0u;
        uint32_t lightTypeOffset = 0u;
        uint32_t lightBindingOffset = 0u;
        for (const auto pLightTypeInfo : arrRegisteredLights)
        {
            const auto &lightInfo = mapReigsteredLights[std::type_index(*pLightTypeInfo)];
            uint32_t lightGroupSize;
            BaseLight * const *lightGroup;
            if (pScene->getSpaceType() == SpaceType::SPACE_2) 
            {
                auto &lightGroup2 = dynamic_cast<Scene<SpaceType::SPACE_2> *>(pScene)->getLightGroup(*pLightTypeInfo);
                lightGroupSize = static_cast<uint32_t>(lightGroup2.size());
                lightGroup = reinterpret_cast<BaseLight * const *>(lightGroup2.data());
            }
            else
            {
                auto &lightGroup3 = dynamic_cast<Scene<SpaceType::SPACE_3> *>(pScene)->getLightGroup(*pLightTypeInfo);
                lightGroupSize = static_cast<uint32_t>(lightGroup3.size());
                lightGroup = reinterpret_cast<BaseLight * const *>(lightGroup3.data());
            }

            uint32_t maxLightCount = lightInfo.maxCount;
            uint32_t lightCount = static_cast<uint32_t>(lightGroupSize);
            //copy data to buffer
            memcpy(memory.data() + offset, &lightCount, sizeof(uint32_t));
            offset += vectorSize;
            for (uint32_t lightIndex = 0u; lightIndex < lightGroupSize; ++lightIndex)
            {
                auto pLight = *(lightGroup + lightIndex);
                auto lightExportInfo = pLight->getExportInfo();
                //check if data size is equal between light and light type registed.
                if (lightExportInfo.dataSize != lightInfo.dataSize)
                    throw std::runtime_error("The data size of the light is not equal to the data size of this light type registerd to scene. ");
                memcpy(memory.data() + offset, lightExportInfo.pData, lightExportInfo.dataSize);
                offset += lightInfo.dataSize;
            }
            
            //create pass texture infos.
            for (uint32_t textureIndex = 0; textureIndex < lightInfo.textureCount; ++textureIndex)
            {
                auto &lightTextureInfos = m_lightTextureInfos[lightBindingOffset];
                auto &lightPassTextureInfo = m_lightPassTextureInfos[lightBindingOffset];
                lightPassTextureInfo.textureCount = maxLightCount;
                lightPassTextureInfo.descriptorType = vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER;
                lightPassTextureInfo.stageFlags = vk::ShaderStageFlagBits::eFragment;
                lightPassTextureInfo.bindingPriority = VG_PASS_LIGHT_TEXTURE_MIN_BINDING_PRIORITY + lightBindingOffset;

                //set texture type for pass texture info.
                auto pFirstLight = *(lightGroup + 0);
                auto firstLightExportInfo = pFirstLight->getExportInfo();
                auto &firstLightTextureInfo = *(firstLightExportInfo.pTextureInfos + textureIndex);
                lightPassTextureInfo.textureType = firstLightTextureInfo.textureType;

                if (static_cast<uint32_t>(lightTextureInfos.size()) < maxLightCount)
                    lightTextureInfos.resize(static_cast<size_t>(maxLightCount));

                uint32_t lightIndex;
                for (lightIndex = 0u; lightIndex < lightCount; ++lightIndex) {
                    auto &pLight = *(lightGroup + lightIndex);
                    auto lightExportInfo = pLight->getExportInfo();
                    //chek if light count of the light is qual to light count of its type registed.
                    if (lightExportInfo.textureCount != lightInfo.textureCount)
                        throw std::runtime_error("The light count of the light is not equal to the light count of this light type registered to scene.");
                    auto &textureInfo = *(lightExportInfo.pTextureInfos + textureIndex);
                    lightTextureInfos[lightIndex].pTexture = textureInfo.pTexture;
                    lightTextureInfos[lightIndex].pImageView = textureInfo.pImageView;
                    lightTextureInfos[lightIndex].pSampler = textureInfo.pSampler;
                    lightTextureInfos[lightIndex].imageLayout = textureInfo.imageLayout;
                }
                for (; lightIndex < maxLightCount; ++lightIndex) {
                    lightTextureInfos[lightIndex].pTexture = nullptr;
                    lightTextureInfos[lightIndex].pImageView = nullptr;
                    lightTextureInfos[lightIndex].pSampler = nullptr;
                    lightTextureInfos[lightIndex].imageLayout = vk::ImageLayout::eUndefined;
                }

                lightPassTextureInfo.pTextures = lightTextureInfos.data();
                ++lightBindingOffset;
            }
            lightTypeOffset += vectorSize;
            lightTypeOffset += lightInfo.dataSize * maxLightCount;
            //offet should be entire block memory for last light types because light count may be is less than max light count.
            offset = lightTypeOffset;
        }

        pLightDataBuffer->updateBuffer(memory.data(), totalSize);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        syncLightDataCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Sync light data cost time: " 
                << syncLightDataCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
        VG_LOG(plog::debug) << "End to sync light data." << std::endl;
    }

    void RenderBinder::_bindForRenderPassBegin(RenderBinderInfo info)
    {
        //bind render pass begin.
        if (info.preDepthEnable)
        {
            //pre z cmd buffer
            const BaseRenderTarget *pRenderTarget;
            const vk::RenderPass *pRenderPass;
            const vk::Framebuffer *pFramebuffer;
            pRenderTarget = info.pPreDepthTarget;
            pRenderPass = info.pPreDepthTarget->getRenderPass();
            pFramebuffer = info.pPreDepthTarget->getFramebuffer();
            _renderPassBegin(pRenderTarget
                , pRenderPass
                , pFramebuffer
                , info.pPreDepthCmdBuffer
                );
        }
        if (info.postRenderEnable)
        {
            const BaseRenderTarget *pRenderTarget;
            const vk::RenderPass *pRenderPass;
            const vk::Framebuffer *pFramebuffer;
            //trunk cmd buffer
            pRenderTarget = info.pPostRenderTarget;
            pRenderPass = info.pPostRenderTarget->getRenderPass();
            pFramebuffer = info.pPostRenderTarget->getFramebuffer();
            _renderPassBegin(pRenderTarget
                , pRenderPass
                , pFramebuffer
                , info.pTrunkRenderPassCmdBuffer
                );
            
            //post render cmd buffer
            pRenderTarget = info.pRendererTarget;
            pRenderPass = info.firstScene ? info.pRendererTarget->getFirstRenderPass() : info.pRendererTarget->getSecondRenderPass();
            pFramebuffer = info.firstScene ? info.pRendererTarget->getFirstFramebuffer() : info.pRendererTarget->getSecondFramebuffer();
            _renderPassBegin(pRenderTarget
                , pRenderPass
                , pFramebuffer
                , info.pPostRenderCmdBuffer
                );
        }
        else
        {
            const BaseRenderTarget *pRenderTarget;
            const vk::RenderPass *pRenderPass;
            const vk::Framebuffer *pFramebuffer;
            //trunk cmd buffer
            pRenderTarget = info.pRendererTarget;
            pRenderPass = info.firstScene ? info.pRendererTarget->getFirstRenderPass() : info.pRendererTarget->getSecondRenderPass();
            pFramebuffer = info.firstScene ? info.pRendererTarget->getFirstFramebuffer() : info.pRendererTarget->getSecondFramebuffer();
            _renderPassBegin(pRenderTarget
                , pRenderPass
                , pFramebuffer
                , info.pTrunkRenderPassCmdBuffer
                );
        }
    }

    void RenderBinder::_bindForRenderPassEnd(RenderBinderInfo info)
    {
        //bind render pass end.
        if (info.preDepthEnable) _renderPassEnd(info.pPreDepthCmdBuffer);
        if (info.postRenderEnable)
        {
            //trunk cmd buffer
            _renderPassEnd(info.pTrunkRenderPassCmdBuffer);
            
            //post render cmd buffer
            _renderPassEnd(info.pPostRenderCmdBuffer);
        }
        else
        {
            //trunk cmd buffer
            _renderPassEnd(info.pTrunkRenderPassCmdBuffer);
        }
    }

    void RenderBinder::_bindScene2(BaseLight *pLight
        , Scene<SpaceType::SPACE_2> *pScene
        , const Projector<SpaceType::SPACE_2> *pProjector
        , const BaseRenderTarget *pPreDepthTarget
        , const BaseRenderTarget *pRenderTarget
        , const Texture *pPreDepthResultTex
        , CmdBuffer *pPreDepthCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer        
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        )
    {
        uint32_t drawCount = 0u;        

        using SceneType = Scene<SpaceType::SPACE_2>;
        auto pDevice = pApp->getDevice();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        
        fd::CostTimer preparingCommonMatrixsCostTimer(fd::CostTimer::TimerType::ONCE);
        preparingCommonMatrixsCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        auto projMatrix3x3 = pScene->getProjMatrix(pProjector);
        auto projMatrix = tranMat3ToMat4(projMatrix3x3);
#ifdef USE_WORLD_BOUNDS
        auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pProjector);
#endif
        
        auto viewMatrix3x3 = pProjector->getWorldToLocalMatrix();
        auto viewMatrix = tranMat3ToMat4(viewMatrix3x3);
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        preparingCommonMatrixsCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Preparing common matrixs cost time: " 
                << preparingCommonMatrixsCostTimer.costTimer << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        uint32_t visualObjectCount = pScene->getVisualObjectCount();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer visibilityCheckCostTimer(fd::CostTimer::TimerType::ONCE);
        visibilityCheckCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        //flat visual objects and filter them that is out of projection with its bounds.
        //allocate enough space for array to storage points.
        std::vector<SceneType::VisualObjectType *> validVisualObjects(visualObjectCount);
        uint32_t validVisualObjectCount(0u);
        auto pRoot = pScene->pRootTransform;
        fillValidVisualObjects(validVisualObjects
            , validVisualObjectCount
            , pRoot.get()
            , pScene
            , pProjector
#ifdef USE_WORLD_BOUNDS
            , &boundsOfViewInWorld
#endif //USE_WORLD_BOUNDS
        );

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        visibilityCheckCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Visibility check cost time: " 
                << visibilityCheckCostTimer.costTimer <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer bindObjectCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
#endif //DEBUG and VG_ENABLE_COST_TIMER
        //------Doing render.
        for (uint32_t i = 0u; i < validVisualObjectCount; ++i)
        {
            auto pVisualObject = validVisualObjects[i];
            auto modelMatrix = tranMat3ToMat4(pVisualObject->getTransform()->getMatrixLocalToWorld());
            if (pPreDepthCmdBuffer != nullptr) 
            {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                _setBuildInData(nullptr
                    , VG_TRUE
                    , pVisualObject
                    , modelMatrix
                    , viewMatrix
                    , projMatrix
                    , nullptr
                );
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER    
            }
            if (pTrunkRenderPassCmdBuffer != nullptr)
            {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                _setBuildInData(pLight
                    , VG_FALSE
                    , pVisualObject
                    , modelMatrix
                    , viewMatrix
                    , projMatrix
                    , pPreDepthResultTex
                    );
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            }
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                bindObjectCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            if (pPreDepthCmdBuffer != nullptr) 
            {
                BaseVisualObject::BindInfo info = {
                    pPreDepthTarget->getFramebufferWidth(),
                    pPreDepthTarget->getFramebufferHeight(),
                    &projMatrix,
                    &viewMatrix,
                    };
                
                BaseVisualObject::BindResult result;
                result.pTrunkRenderPassCmdBuffer = pPreDepthCmdBuffer;
                result.pBranchCmdBuffer = nullptr;
                result.pTrunkWaitBarrierCmdBuffer = nullptr;
                _bindVisualObject(VG_FALSE, VG_TRUE, pVisualObject, info, &result);
            }

            if (pTrunkRenderPassCmdBuffer != nullptr)
            {
                BaseVisualObject::BindInfo info = {
                    pRenderTarget != nullptr ? pRenderTarget->getFramebufferWidth() : 0u,
                    pRenderTarget != nullptr ? pRenderTarget->getFramebufferHeight() : 0u,
                    &projMatrix,
                    &viewMatrix,
                    };
    
                BaseVisualObject::BindResult result;
                result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
                result.pBranchCmdBuffer = pBranchCmdBuffer;
                result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
                _bindVisualObject(pLight, VG_FALSE, pVisualObject, info, &result);
            }
            
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                bindObjectCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        }
        
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
            << preparingBuildInDataCostTimer.costTimer <<  std::endl;
        
        VG_COST_TIME_LOG(plog::debug) << "Bind Visual Object cost time: " 
            << bindObjectCostTimer.costTimer <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void fillValidVisualObjects(std::vector<VisualObject<SpaceType::SPACE_2> *> &arrPVObjs
        , uint32_t &PVObjIndex
        , const Transform<SpaceType::SPACE_2> *pTransform
        , Scene<SpaceType::SPACE_2> *pScene
        , const Projector<SpaceType::SPACE_2> *pProjector
#ifdef USE_WORLD_BOUNDS
        , const fd::Bounds<SpaceTypeInfo<SpaceType::SPACE_2>::PointType> *pViewBoundsInWorld
#endif
        )
    {
        VisualObject<SpaceType::SPACE_2> *pVisualObjectOfChild;
        uint32_t childCount = pTransform->getChildCount();
        const Transform<SpaceType::SPACE_2> *pChild;
        for (uint32_t i = 0; i < childCount; ++i)
        {
            pChild = pTransform->getChildWithIndex(i);
            //Children's visual object is placed ahead of own visual objects
            fillValidVisualObjects(arrPVObjs
                , PVObjIndex
                , pChild
                , pScene
                , pProjector
#ifdef USE_WORLD_BOUNDS
                , pViewBoundsInWorld
#endif //USE_WORLD_BOUNDS
                );
            //Own visual object is placed behind children's visual object.
            pVisualObjectOfChild = pScene->getVisualObjectWithTransform(pChild);
            if (pVisualObjectOfChild == nullptr) continue;
            
            auto pMeshOfChild = pVisualObjectOfChild->getMesh();
            auto isHasBoundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild)->getIsHasBounds();
            if (isHasBoundsOfChild == VG_FALSE)
            {
                arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
            } 
            else if (pVisualObjectOfChild->getIsVisibilityCheck() == VG_FALSE)
            {
                arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
                pVisualObjectOfChild->setHasClipRect(VG_FALSE);
            }
            else {
                //Filter obj out of projection.
                auto boundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild)->getBounds();
#ifdef USE_WORLD_BOUNDS
                auto boundsOfChildInWorld = tranBoundsToNewSpace<Vector2>(boundsOfChild, pChild->getMatrixLocalToWorld(), VG_FALSE);
#endif //USE_WORLD_BOUNDS
                fd::Rect2D clipRect;
#ifdef USE_WORLD_BOUNDS
                if (pViewBoundsInWorld->isIntersects(boundsOfChildInWorld) && pScene->isInProjection(pProjector, boundsOfChildInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInProjection(pProjector, pChild, boundsOfChild, &clipRect) == VG_TRUE)
#endif //USE_WORLD_BOUNDS
                {
                    arrPVObjs[PVObjIndex++] = pVisualObjectOfChild;
                    //Transform range [-1, 1] to range [0, 1]
                    clipRect.x = (clipRect.x + 1.0f) / 2.0f;
                    clipRect.y = (clipRect.y + 1.0f) / 2.0f;
                    clipRect.width = clipRect.width / 2.0f;
                    clipRect.height = clipRect.height / 2.0f;
                    uint32_t subMeshCount = pVisualObjectOfChild->getSubMeshCount();
                    pVisualObjectOfChild->setHasClipRect(VG_TRUE);
                    pVisualObjectOfChild->updateClipRects(clipRect, subMeshCount);
                }
            }
        }
    }

    void RenderBinder::_bindScene3(BaseLight *pLight
        , Scene<SpaceType::SPACE_3> *pScene
        , const Projector<SpaceType::SPACE_3> *pProjector
        , const BaseRenderTarget *pPreDepthTarget
        , const BaseRenderTarget *pRenderTarget
        , const Texture *pPreDepthResultTex
        , CmdBuffer *pPreDepthCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer        
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        )
    {
        using SceneType = Scene<SpaceType::SPACE_3>;

        uint32_t drawCount = 0u;    

        auto queueTypeCount = static_cast<uint32_t>(RenderQueueType::RANGE_SIZE);
        auto pDevice = pApp->getDevice();

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        
        fd::CostTimer preparingCommonMatrixsCostTimer(fd::CostTimer::TimerType::ONCE);
        preparingCommonMatrixsCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        auto projMatrix = pScene->getProjMatrix(pProjector);

#ifdef USE_WORLD_BOUNDS
        auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pProjector);
#endif

        auto viewMatrix = pProjector->getWorldToLocalMatrix();
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        preparingCommonMatrixsCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Preparing common matrixs cost time: " 
                << preparingCommonMatrixsCostTimer.costTimer <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        uint32_t visualObjectCount = pScene->getVisualObjectCount();
        
        //----------Preparing render.

        //Filter visualObject is out of projection with its bounds.
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer visibilityCheckCostTimer(fd::CostTimer::TimerType::ONCE);
        visibilityCheckCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        std::vector<SceneType::VisualObjectType *> validVisualObjects(visualObjectCount); //allocate enough space for array to storage points.
        uint32_t validVisualObjectCount(0u);
        for (uint32_t i = 0; i < visualObjectCount; ++i)
        {
            auto pVisualObject = pScene->getVisualObjectWithIndex(i);
            auto pMesh = pVisualObject->getMesh();
            auto isHasBounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh)->getIsHasBounds();
            if (isHasBounds == VG_FALSE)
            {
                validVisualObjects[validVisualObjectCount++] = pVisualObject;
            }
            else if (pVisualObject->getIsVisibilityCheck() == VG_FALSE)
            {
                validVisualObjects[validVisualObjectCount++] = pVisualObject;
                pVisualObject->setHasClipRect(VG_FALSE);
            }
            else 
            {
                auto bounds = dynamic_cast<SceneType::VisualObjectType::MeshDimType *>(pMesh)->getBounds();
                auto pTransform = pVisualObject->getTransform();
#ifdef USE_WORLD_BOUNDS
                auto boundsInWorld = tranBoundsToNewSpace<Vector3>(bounds, pTransform->getMatrixLocalToWorld(), VG_FALSE);      
#endif //USE_WORLD_BOUNDS
                fd::Rect2D clipRect;
#ifdef USE_WORLD_BOUNDS
                if (boundsOfViewInWorld.isIntersects(boundsInWorld) == FD_TRUE && 
                    pScene->isInProjection(pProjector, boundsInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInProjection(pProjector, pTransform, bounds, &clipRect) == VG_TRUE)
#endif //USE_WORLD_BOUNDS
                {
                    validVisualObjects[validVisualObjectCount++] = pVisualObject;
                    //Transform range [-1, 1] to range [0, 1]
                    clipRect.x = (clipRect.x + 1.0f) / 2.0f;
                    clipRect.y = (clipRect.y + 1.0f) / 2.0f;
                    clipRect.width = clipRect.width / 2.0f;
                    clipRect.height = clipRect.height / 2.0f;
    
                    uint32_t subMeshCount = pVisualObject->getSubMeshCount();
                    pVisualObject->setHasClipRect(VG_TRUE);
                    pVisualObject->updateClipRects(clipRect, subMeshCount);
                }
                
            }
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        visibilityCheckCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Visibility check cost time: " 
                << visibilityCheckCostTimer.costTimer <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER

        //Get queue count for each queue type.
        std::vector<uint32_t> queueLengths(queueTypeCount, 0u);
        for (uint32_t i = 0; i < validVisualObjectCount; ++i)
        {
            auto pVisualObject = validVisualObjects[i];
            auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
            ++queueLengths[static_cast<size_t>(renderQueueType)];
        }

        std::vector<std::vector<SceneType::VisualObjectType *>> queues(queueTypeCount);
        //Resize queues and reset quue counts to zero for preparing next use.
        for (uint32_t i = 0; i < queueTypeCount; ++i)
        {
            queues[i].resize(queueLengths[i], nullptr);
            queueLengths[i] = 0u;
        }

        //Update queues to point to visual object.
        for (uint32_t i = 0; i < validVisualObjectCount; ++i)
        {
            auto pVisualObject = validVisualObjects[i];
            auto renderQueueType = tranMaterialShowTypeToRenderQueueType(pVisualObject->getMaterial()->getShowType());
            queues[static_cast<size_t>(renderQueueType)][queueLengths[static_cast<size_t>(renderQueueType)]++] = pVisualObject;
        }

        //sort transparent queue.
        std::sort(queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].begin(),
            queues[static_cast<size_t>(RenderQueueType::TRANSPARENT)].end(),
            [&viewMatrix, &projMatrix](typename SceneType::ObjectType *pObject1, typename SceneType::ObjectType *pObject2)
            {
                auto modelMatrix1 = pObject1->getTransform()->getMatrixLocalToWorld();
                auto mvMatrix1 = viewMatrix * modelMatrix1;
                auto modelMatrix2 = pObject2->getTransform()->getMatrixLocalToWorld();
                auto mvMatrix2 = viewMatrix * modelMatrix2;
                //get position
                auto pos1 = pObject1->getTransform()->getLocalPosition();
                auto pos2 = pObject2->getTransform()->getLocalPosition();
        
                using PointType = SpaceTypeInfo<SpaceType::SPACE_3>::PointType;
                using MatrixVectorType = SpaceTypeInfo<SpaceType::SPACE_3>::MatrixVectorType;
                //transform point from model coordinate system to projector coordinate system.
                pos1 = mvMatrix1 * MatrixVectorType(pos1, 1.0f);
                pos2 = mvMatrix2 * MatrixVectorType(pos2, 1.0f);
        
                //it is smaller if its z is bigger than the other.
                return static_cast<Bool32>(pos1.z > pos2.z);
        
            });

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer bindObjectCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
#endif //DEBUG and VG_ENABLE_COST_TIMER
        //-----Doing render
        for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
        {
            auto queueLength = queueLengths[typeIndex];
            for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
            {
                auto pVisualObject = queues[typeIndex][objectIndex];
                auto modelMatrix = pVisualObject->getTransform()->getMatrixLocalToWorld();
                if (pPreDepthCmdBuffer != nullptr) 
                {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                    _setBuildInData(nullptr
                        , VG_TRUE
                        , pVisualObject
                        , modelMatrix
                        , viewMatrix
                        , projMatrix
                        , nullptr
                    );
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER   
                }
                if (pTrunkRenderPassCmdBuffer != nullptr)
                {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                    _setBuildInData(pLight
                        , VG_FALSE
                        , pVisualObject
                        , modelMatrix
                        , viewMatrix
                        , projMatrix
                        , pPreDepthResultTex
                    );
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                preparingBuildInDataCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                }
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                bindObjectCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
                if (pPreDepthCmdBuffer != nullptr) 
                {
                    BaseVisualObject::BindInfo info = {
                        pPreDepthTarget->getFramebufferWidth(),
                        pPreDepthTarget->getFramebufferHeight(),
                        &projMatrix,
                        &viewMatrix,
                        };
                    
                    BaseVisualObject::BindResult result;
                    result.pTrunkRenderPassCmdBuffer = pPreDepthCmdBuffer;
                    result.pBranchCmdBuffer = nullptr;
                    result.pTrunkWaitBarrierCmdBuffer = nullptr;
                    _bindVisualObject(VG_FALSE, VG_TRUE, pVisualObject, info, &result);
                }
    
                if (pTrunkRenderPassCmdBuffer != nullptr)
                {
                    BaseVisualObject::BindInfo info = {
                        pRenderTarget != nullptr ? pRenderTarget->getFramebufferWidth() : 0u,
                        pRenderTarget != nullptr ? pRenderTarget->getFramebufferHeight() : 0u,
                        &projMatrix,
                        &viewMatrix,
                        };
        
                    BaseVisualObject::BindResult result;
                    result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
                    result.pBranchCmdBuffer = pBranchCmdBuffer;
                    result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
                    _bindVisualObject(pLight, VG_FALSE, pVisualObject, info, &result);
                    
                }
                
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                bindObjectCostTimer.end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
            }
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
            << preparingBuildInDataCostTimer.costTimer <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Bind Visual Object cost time: " 
            << bindObjectCostTimer.costTimer <<  std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void RenderBinder::_setBuildInData(BaseLight *pLight
        , Bool32 isPreDepth
        , BaseVisualObject * pVisualObject
        , Matrix4x4 modelMatrix
        , Matrix4x4 viewMatrix
        , Matrix4x4 projMatrix
        , const Texture *pPreDepthResultTex
    )
    {
        uint32_t materialCount = pVisualObject->getMaterialCount();
        for (uint32_t materialIndex = 0u; materialIndex < materialCount; ++materialIndex)
        {
            Material *pMaterial;
            if (pLight != nullptr)
            {
                const auto &typeInfo = typeid(*pLight);
                pMaterial = pVisualObject->getLightingMaterial(typeInfo, materialIndex);
            }
            else if (isPreDepth)
            {
                pMaterial = pVisualObject->getPreDepthMaterial(materialIndex);
            }
            else
            {
                pMaterial = pVisualObject->getMaterial(materialIndex);
            }
            
            auto passCount = pMaterial->getPassCount();
            for (uint32_t passIndex = 0u; passIndex < passCount; ++passIndex)
            {
                auto pPass = pMaterial->getPassWithIndex(passIndex);

                Bool32 hasMatrixObjectToNDC = VG_FALSE;
                Bool32 hasMatrixObjectToWorld = VG_FALSE;
                Bool32 hasMatrixObjectToView = VG_FALSE;
                Bool32 hasMatrixView = VG_FALSE;
                Bool32 hasMatrixProjection = VG_FALSE;
                //update building in matrix variable.
                auto info = pPass->getBuildInDataInfo();
                uint32_t componentCount = info.componentCount;
                for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
                {
                    Pass::BuildInDataType type = (*(info.pComponent + componentIndex)).type;
                    if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC)
                    {
                        hasMatrixObjectToNDC = VG_TRUE;
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
                    {
                        hasMatrixObjectToWorld = VG_TRUE;
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
                    {
                        hasMatrixObjectToView = VG_TRUE;
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_VIEW)
                    {
                        hasMatrixView = VG_TRUE;
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
                    {
                        hasMatrixProjection = VG_TRUE;
                    }
                }
                
                Matrix4x4 mvMatrix;
                Matrix4x4 mvpMatrix;
                if (hasMatrixObjectToView || hasMatrixObjectToNDC)
                {
                    mvMatrix = viewMatrix * modelMatrix;
                }
                
                if (hasMatrixObjectToNDC)
                {
                    mvpMatrix = projMatrix * mvMatrix;
                }
                //update building in matrix variable.
                for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
                {
                    Pass::BuildInDataType type = (*(info.pComponent + componentIndex)).type;
                    if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC)
                    {
                        pPass->setBuildInMatrixData(type, mvpMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
                    {
                        pPass->setBuildInMatrixData(type, modelMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
                    {
                        pPass->setBuildInMatrixData(type, mvMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_VIEW)
                    {
                        pPass->setBuildInMatrixData(type, viewMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
                    {
                        pPass->setBuildInMatrixData(type, projMatrix);
                    }
                }

                if (isPreDepth == VG_FALSE && pLight == nullptr)
                {                
                    //light data buffer.
                    if (m_lightingEnable && m_lightTypeCount > 0)
                    {
                        vg::PassBufferInfo::BufferInfo itemInfo = {
                            m_pCurrLightDataBuffer,
                            0u,
                            m_pCurrLightDataBuffer->getBufferSize(),
                        };
                        PassBufferInfo info = {
                            1u,
                            &itemInfo,
                            VG_PASS_LIGHT_DATA_BUFFER_BINDING_PRIORITY,
                            vg::BufferDescriptorType::UNIFORM_BUFFER,
                            vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
                        };
                        if (pPass->hasBuffer(VG_PASS_LIGHT_DATA_BUFFER_NAME) == VG_FALSE)
                        {
                            pPass->addBuffer(VG_PASS_LIGHT_DATA_BUFFER_NAME, info);
                        }
                        else
                        {
                            pPass->setBuffer(VG_PASS_LIGHT_DATA_BUFFER_NAME, info);
                        }
                        //light textures.
                        auto &lightPassTextureInfos = m_lightPassTextureInfos;
                        uint32_t textureInfoCount = static_cast<uint32_t>(lightPassTextureInfos.size());
                        for (uint32_t textureInfoIndex = 0u; textureInfoIndex < textureInfoCount; ++textureInfoIndex)
                        {
                            std::string name = VG_PASS_LIGHT_TEXTURE_NAME + std::to_string(textureInfoIndex);
                            if (pPass->hasTexture(name) == VG_FALSE) 
                            {
                                pPass->addTexture(name, lightPassTextureInfos[textureInfoIndex]);
                            }
                            else
                            {
                                pPass->setTexture(name, lightPassTextureInfos[textureInfoIndex]);
                            }
                        }
                    }
    
                    if (pPreDepthResultTex != nullptr)
                    {
                        vg::PassTextureInfo::TextureInfo itemInfo = {
                            pPreDepthResultTex,
                            nullptr,
                            nullptr,
                            vk::ImageLayout::eUndefined,
                        };
                        PassTextureInfo info = {
                            vg::SamplerTextureType::TEX_2D,
                            1u,
                            &itemInfo,
                            VG_PASS_PRE_DEPTH_TEXTURE_BINDING_PRIORITY,
                            vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
                            vk::ShaderStageFlagBits::eFragment,
                        };
                        if (pPass->hasTexture(VG_PASS_PRE_DEPTH_TEXTURE_NAME) == VG_FALSE)
                        {
                            pPass->addTexture(VG_PASS_PRE_DEPTH_TEXTURE_NAME, info);
                        }
                        else
                        {
                            pPass->setTexture(VG_PASS_PRE_DEPTH_TEXTURE_NAME, info);
                        }
                    }
                }

                pPass->apply();
            }
        }
    }

    void RenderBinder::_bindVisualObject(BaseLight *pLight
        , Bool32 isPreDepth
        , BaseVisualObject *pVisublObject
        , BaseVisualObject::BindInfo & bindInfo
        , BaseVisualObject::BindResult *pResult
        )
    {
        if (pLight != nullptr)
        {
            const auto &typeInfo = typeid(*pLight);
            pVisublObject->beginBindForLighting(typeInfo, bindInfo, pResult);
            if (static_cast<uint32_t>(m_bindedObjectsForLighting.size()) == m_bindedObjectCountForLighting) {
                auto newSize = getNextCapacity(static_cast<uint32_t>(m_bindedObjectsForLighting.size()));
                m_bindedObjectsForLighting.resize(newSize);
            }
    
            m_bindedObjectsForLighting[m_bindedObjectCountForLighting] = pVisublObject;
            ++m_bindedObjectCountForLighting;

        } else if (isPreDepth) 
        {
            pVisublObject->beginBindForPreDepth(bindInfo, pResult);
            if (static_cast<uint32_t>(m_bindedObjectsForPreDepth.size()) == m_bindedObjectCountForPreDepth) {
                auto newSize = getNextCapacity(static_cast<uint32_t>(m_bindedObjectsForPreDepth.size()));
                m_bindedObjectsForPreDepth.resize(newSize);
            }
    
            m_bindedObjectsForPreDepth[m_bindedObjectCountForPreDepth] = pVisublObject;
            ++m_bindedObjectCountForPreDepth;

        } else {
            pVisublObject->beginBind(bindInfo, pResult);
            if (static_cast<uint32_t>(m_bindedObjects.size()) == m_bindedObjectCount) {
                auto newSize = getNextCapacity(static_cast<uint32_t>(m_bindedObjects.size()));
                m_bindedObjects.resize(newSize);
            }
    
            m_bindedObjects[m_bindedObjectCount] = pVisublObject;
    
            ++m_bindedObjectCount;
        }
    }

    void RenderBinder::_renderPassBegin(const BaseRenderTarget *pRenderTarget
        , const vk::RenderPass *pRenderPass
        , const vk::Framebuffer *pFramebuffer
        , CmdBuffer *pCmdBuffer
        )
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer renderPassBeginCostTimer(fd::CostTimer::TimerType::ONCE);
        renderPassBeginCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        const auto framebufferWidth = pRenderTarget->getFramebufferWidth();
        const auto framebufferHeight = pRenderTarget->getFramebufferHeight();
        RenderPassBeginInfo beginInfo;
        beginInfo.pRenderPass = pRenderPass;
        beginInfo.pFramebuffer = pFramebuffer;
        beginInfo.framebufferWidth = framebufferWidth;
        beginInfo.framebufferHeight = framebufferHeight;
        beginInfo.renderArea = pRenderTarget->getRenderArea();
        beginInfo.clearValueCount = pRenderTarget->getClearValueCount();
        beginInfo.pClearValues = pRenderTarget->getClearValues();

        CmdInfo cmdInfo;
        cmdInfo.pRenderPassBeginInfo = &beginInfo;
        pCmdBuffer->addCmd(cmdInfo);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        renderPassBeginCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Render pass begin cost time: " 
                << renderPassBeginCostTimer.costTimer << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void RenderBinder::_renderPassEnd(CmdBuffer *pCmdBuffer)
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        fd::CostTimer renderPassEndCostTimer(fd::CostTimer::TimerType::ONCE);
        renderPassEndCostTimer.begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER

        RenderPassEndInfo endInfo;
        CmdInfo cmdInfo;
        cmdInfo.pRenderPassEndInfo = &endInfo;
        pCmdBuffer->addCmd(cmdInfo);

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        renderPassEndCostTimer.end();
        VG_COST_TIME_LOG(plog::debug) << "Render pass end cost time: " 
                << renderPassEndCostTimer.costTimer << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }
} //vg