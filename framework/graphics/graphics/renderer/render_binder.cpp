#include "graphics/renderer/render_binder.hpp"

#include "graphics/util/gemo_util.hpp"

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

    RenderBinderInfo::RenderBinderInfo(Bool32 firstScene
        , Bool32 preZEnable
        , Bool32 postRenderEnable
        , BaseScene *pScene
        , const BaseProjector *pProjector
        , PostRender *pPostRender

        , const PreZTarget *pPreZTarget
        , const PostRenderTarget *pPostRenderTarget
        , const MultiRenderTarget *pRendererTarget

        , CmdBuffer *pPreZCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , CmdBuffer *pPostRenderCmdBuffer
        )
        : firstScene(firstScene)
        , preZEnable(preZEnable)
        , postRenderEnable(postRenderEnable)

        , pScene(pScene)
        , pProjector(pProjector)
        , pPostRender(pPostRender)

        , pPreZTarget(pPreZTarget)
        , pPostRenderTarget(pPostRenderTarget)
        , pRendererTarget(pRendererTarget)

        , pPreZCmdBuffer(pPreZCmdBuffer)
        , pBranchCmdBuffer(pBranchCmdBuffer)
        , pTrunkWaitBarrierCmdBuffer(pTrunkWaitBarrierCmdBuffer)
        , pTrunkRenderPassCmdBuffer(pTrunkRenderPassCmdBuffer)
        , pPostRenderCmdBuffer(pPostRenderCmdBuffer)
    {

    }

    RenderBinder::RenderBinder(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        )
        : m_framebufferWidth(framebufferWidth)
        , m_framebufferHeight(framebufferHeight)
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

     uint32_t RenderBinder::getFramebufferWidth() const
     {
         return m_framebufferWidth;
     }

    void RenderBinder::setFramebufferWidth(uint32_t value)
    {
        m_framebufferWidth = value;
    }

    uint32_t RenderBinder::getFramebufferHeight() const
    {
        return m_framebufferHeight;
    }

    void RenderBinder::setFramebufferHeight(uint32_t value)
    {
        m_framebufferHeight = value;
    }

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
        for (uint32_t i = 0; i < m_bindedObjectCount; ++i) {
            m_bindedObjects[i]->endBind();
        }
        m_bindedObjectCount = 0u;
    }

    void RenderBinder::_bind(RenderBinderInfo info)
    {
        _syncLightData(info.pScene);
        _bindForRenderPassBegin(info);
        //scene make cmds.
        if (info.pScene->getSpaceType() == SpaceType::SPACE_2)
        {
            _bindScene2(dynamic_cast<Scene<SpaceType::SPACE_2> *>(info.pScene), 
                dynamic_cast<const Projector<SpaceType::SPACE_2> *>(info.pProjector),
                info.preZEnable ? info.pPreZTarget : nullptr,
                info.preZEnable ? info.pPreZCmdBuffer : nullptr,
                info.pBranchCmdBuffer,                    
                info.pTrunkWaitBarrierCmdBuffer,
                info.pTrunkRenderPassCmdBuffer
                );
        } 
        else if (info.pScene->getSpaceType() == SpaceType::SPACE_3)
        {
            _bindScene3(dynamic_cast<Scene<SpaceType::SPACE_3> *>(info.pScene), 
                dynamic_cast<const Projector<SpaceType::SPACE_3> *>(info.pProjector), 
                info.preZEnable ? info.pPreZTarget : nullptr,
                info.preZEnable ? info.pPreZCmdBuffer : nullptr,
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
                auto buildInDataInfo = pPass->getBuildInDataInfo();
                auto buildInComponentCount = buildInDataInfo.componentCount;
                for (uint32_t j = 0; j < buildInComponentCount; ++j)
                {
                    if ((*(buildInDataInfo.pComponent + j)).type == Pass::BuildInDataType::POST_RENDER_RESULT)
                    {
                        auto pColorTex = dynamic_cast<const Texture *>(info.pPostRenderTarget->getColorAttachment());
                        vg::PassTextureInfo::TextureInfo itemInfo = {
                            pColorTex,
                            nullptr,
                            nullptr,
                            vk::ImageLayout::eUndefined,
                        };
                        PassTextureInfo info = {
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
                        break;
                    }
                }
            }
            PostRender::BindInfo bindInfo = {
                m_framebufferWidth,
                m_framebufferHeight,
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

    void RenderBinder::_syncLightData(BaseScene *pScene)
    {
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
            totalSize += static_cast<uint32_t>(sizeof(uint32_t));
            //space for data of this type lights.
            totalSize += lightInfo.dataSize * maxLightCount;
            totalTextureBindingCount += textureCount;
        }

        if (static_cast<uint32_t>(m_lightTextureInfos.size()) < totalTextureBindingCount) 
        {
            m_lightTextureInfos.resize(static_cast<uint32_t>(totalTextureBindingCount));
        }
        
        //Allocate memory and copy ligth data to it.
        //Create light pass texture infos.
        std::vector<Byte> memory(totalSize);
        uint32_t offset = 0u;
        uint32_t lightTypeOffset = 0u;
        uint32_t lightBindingOffset = 0u;
        for (const auto pLightTypeInfo : arrRegisteredLights)
        {
            const auto &lightInfo = mapReigsteredLights[std::type_index(*pLightTypeInfo)];
            if (pScene->getSpaceType() == SpaceType::SPACE_2) 
            {
                const auto &lightGroup = dynamic_cast<Scene<SpaceType::SPACE_2> *>(pScene)->getLightGroup(*pLightTypeInfo);
                uint32_t lightCount = static_cast<uint32_t>(lightGroup.size());

                //copy data to buffer
                memcpy(memory.data() + offset, &lightCount, sizeof(uint32_t));
                offset += static_cast<uint32_t>(sizeof(uint32_t));
                for (const auto &pLight : lightGroup) {
                    pLight->memcpyLightData(memory.data() + offset);
                    offset += lightInfo.dataSize;
                }
                
                //create pass texture infos.
                for (uint32_t textureIndex = 0; textureIndex < lightInfo.textureCount; ++textureIndex)
                {
                    auto lightTextureInfos = m_lightTextureInfos[lightBindingOffset];
                    auto lightPassTextureInfo = m_lightPassTextureInfos[lightBindingOffset];
                    lightPassTextureInfo.textureCount = lightCount;
                    lightPassTextureInfo.descriptorType = vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER;
                    lightPassTextureInfo.stageFlags = vk::ShaderStageFlagBits::eFragment;
                    lightPassTextureInfo.bindingPriority = VG_PASS_LIGHT_TEXTURE_MIN_BINDING_PRIORITY + lightBindingOffset;
                    if (static_cast<uint32_t>(lightTextureInfos.size()) < lightCount)
                        lightTextureInfos.resize(static_cast<uint32_t>(lightCount));
                    for (uint32_t lightIndex = 0; lightIndex < lightCount; ++lightIndex) {
                        auto pLight = lightGroup[lightIndex];
                        lightTextureInfos[lightIndex].pTexture = pLight->getTexture(textureIndex);
                    }
                    lightPassTextureInfo.pTextures = lightTextureInfos.data();
                    ++lightBindingOffset;
                }
                
            }
            else
            {
                const auto &lightGroup = dynamic_cast<Scene<SpaceType::SPACE_3> *>(pScene)->getLightGroup(*pLightTypeInfo);
                uint32_t lightCount = static_cast<uint32_t>(lightGroup.size());
                //copy data to buffer
                memcpy(memory.data() + offset, &lightCount, sizeof(uint32_t));
                offset += static_cast<uint32_t>(sizeof(uint32_t));
                for (const auto &pLight : lightGroup) {
                    pLight->memcpyLightData(memory.data() + offset);
                    offset += lightInfo.dataSize;
                }

                //create pass texture infos.
                for (uint32_t textureIndex = 0; textureIndex < lightInfo.textureCount; ++textureIndex)
                {
                    auto lightTextureInfos = m_lightTextureInfos[lightBindingOffset];
                    auto lightPassTextureInfo = m_lightPassTextureInfos[lightBindingOffset];
                    lightPassTextureInfo.textureCount = lightCount;
                    lightPassTextureInfo.descriptorType = vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER;
                    lightPassTextureInfo.stageFlags = vk::ShaderStageFlagBits::eFragment;
                    lightPassTextureInfo.bindingPriority = VG_PASS_LIGHT_TEXTURE_MIN_BINDING_PRIORITY + lightBindingOffset;
                    if (static_cast<uint32_t>(lightTextureInfos.size()) < lightCount)
                        lightTextureInfos.resize(static_cast<uint32_t>(lightCount));
                    for (uint32_t lightIndex = 0; lightIndex < lightCount; ++lightIndex) {
                        auto pLight = lightGroup[lightIndex];
                        lightTextureInfos[lightIndex].pTexture = pLight->getTexture(textureIndex);
                    }
                    lightPassTextureInfo.pTextures = lightTextureInfos.data();
                    ++lightBindingOffset;
                }
            }
            lightTypeOffset += static_cast<uint32_t>(sizeof(uint32_t));
            lightTypeOffset += lightInfo.dataSize * lightInfo.maxCount;
            //offet should be entire block memory for last light types because light count may be is less than max light count.
            offset = lightTypeOffset;
        }

        pLightDataBuffer->updateBuffer(memory.data(), totalSize);


    }

    void RenderBinder::_bindForRenderPassBegin(RenderBinderInfo info)
    {
        //bind render pass begin.
        if (info.preZEnable)
        {
            //pre z cmd buffer
            const BaseRenderTarget *pRenderTarget;
            const vk::RenderPass *pRenderPass;
            const vk::Framebuffer *pFramebuffer;
            pRenderTarget = info.pPreZTarget;
            pRenderPass = info.pPreZTarget->getRenderPass();
            pFramebuffer = info.pPreZTarget->getFramebuffer();
            _renderPassBegin(pRenderTarget
                , pRenderPass
                , pFramebuffer
                , info.pPreZCmdBuffer
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
        if (info.preZEnable) _renderPassEnd(info.pPreZCmdBuffer);
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

    void RenderBinder::_bindScene2(Scene<SpaceType::SPACE_2> *pScene
        , const Projector<SpaceType::SPACE_2> *pProjector
        , const PreZTarget *pPreZTarget
        , CmdBuffer *pPreZCmdBuffer
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
                << preparingCommonMatrixsCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
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
                << visibilityCheckCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
        
        fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer preparingPipelineCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer preparingCommandBufferCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
#endif //DEBUG and VG_ENABLE_COST_TIMER

        //------Doing render.
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;    
        for (uint32_t i = 0u; i < validVisualObjectCount; ++i)
        {
            auto pVisualObject = validVisualObjects[i];
            auto modelMatrix = tranMat3ToMat4(pVisualObject->getTransform()->getMatrixLocalToWorld());
            if (pPreZCmdBuffer != nullptr) 
            {
                _setPreZBuildInData(pVisualObject
                    , modelMatrix
                    , viewMatrix
                    , projMatrix
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    , preparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                );    
            }
            if (pTrunkRenderPassCmdBuffer != nullptr)
            {
                _setBuildInData(pVisualObject
                    , modelMatrix
                    , viewMatrix
                    , projMatrix
                    , pScene
                    , pPreZTarget
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    , preparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                    );
            }
            BaseVisualObject::BindInfo info = {
                framebufferWidth,
                framebufferHeight,
                &projMatrix,
                &viewMatrix,
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                &preparingPipelineCostTimer,
                &preparingCommandBufferCostTimer,
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                };

            BaseVisualObject::BindResult result;
            result.pPreZCmdBuffer = pPreZCmdBuffer;
            result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
            result.pBranchCmdBuffer = pBranchCmdBuffer;
            result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
            _bindVisualObject(pVisualObject, info, &result);
        }
        
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        VG_COST_TIME_LOG(plog::debug) << "Pre object recording callback cost time: "
            << preObjectRecordingCallBackCostTimer.costTimer
            << "ms, scene id: " << pScene->getID()
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
            << std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
            << preparingBuildInDataCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing pipeline cost time: " 
            << preparingPipelineCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing command buffer cost time: " 
            << preparingCommandBufferCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Post object recording callback cost time: "
            << postObjectRecordingCallBackCostTimer.costTimer
            << "ms, scene id: " << pScene->getID()
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
            << std::endl;
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

    void RenderBinder::_bindScene3(Scene<SpaceType::SPACE_3> *pScene
        , const Projector<SpaceType::SPACE_3> *pProjector
        , const PreZTarget *pPreZTarget
        , CmdBuffer *pPreZCmdBuffer
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
                << preparingCommonMatrixsCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
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
                << visibilityCheckCostTimer.costTimer 
                << "ms, scene id: " << pScene->getID() 
                << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
                <<  std::endl;
        
        fd::CostTimer preparingBuildInDataCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer preparingPipelineCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer preparingCommandBufferCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer preObjectRecordingCallBackCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
        fd::CostTimer postObjectRecordingCallBackCostTimer(fd::CostTimer::TimerType::ACCUMULATION);
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

        //-----Doing render
        const auto framebufferWidth = m_framebufferWidth;
        const auto framebufferHeight = m_framebufferHeight;
        for (uint32_t typeIndex = 0u; typeIndex < queueTypeCount; ++typeIndex)
        {
            auto queueLength = queueLengths[typeIndex];
            for (uint32_t objectIndex = 0u; objectIndex < queueLength; ++objectIndex)
            {
                auto pVisualObject = queues[typeIndex][objectIndex];
                auto modelMatrix = pVisualObject->getTransform()->getMatrixLocalToWorld();
                if (pPreZCmdBuffer != nullptr) 
                {
                    _setPreZBuildInData(pVisualObject
                        , modelMatrix
                        , viewMatrix
                        , projMatrix
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                        , preparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                    );    
                }
                if (pTrunkRenderPassCmdBuffer != nullptr)
                {
                    _setBuildInData(pVisualObject
                        , modelMatrix
                        , viewMatrix
                        , projMatrix
                        , pScene
                        , pPreZTarget
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                        , preparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                    );
                }

                BaseVisualObject::BindInfo info = {
                    framebufferWidth,
                    framebufferHeight,
                    &projMatrix,
                    &viewMatrix,
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                    &preparingPipelineCostTimer,
                    &preparingCommandBufferCostTimer,
#endif //DEBUG and VG_ENABLE_COST_TIMER    
                    };

                BaseVisualObject::BindResult result;
                result.pPreZCmdBuffer = pPreZCmdBuffer;
                result.pTrunkRenderPassCmdBuffer = pTrunkRenderPassCmdBuffer;
                result.pBranchCmdBuffer = pBranchCmdBuffer;
                result.pTrunkWaitBarrierCmdBuffer = pTrunkWaitBarrierCmdBuffer;
                _bindVisualObject(pVisualObject, info, &result);
            }
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        VG_COST_TIME_LOG(plog::debug) << "Pre object recording callback cost time: "
            << preObjectRecordingCallBackCostTimer.costTimer
            << "ms, scene id: " << pScene->getID()
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
            << std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing buildin data cost time: " 
            << preparingBuildInDataCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing pipeline cost time: " 
            << preparingPipelineCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Preparing command buffer cost time: " 
            << preparingCommandBufferCostTimer.costTimer 
            << "ms, scene id: " << pScene->getID() 
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2") 
            <<  std::endl;
        VG_COST_TIME_LOG(plog::debug) << "Post object recording callback cost time: "
            << postObjectRecordingCallBackCostTimer.costTimer
            << "ms, scene id: " << pScene->getID()
            << ", scene type: " << (pScene->getSpaceType() == SpaceType::SPACE_3 ? "space3" : "space2")
            << std::endl;
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void RenderBinder::_setPreZBuildInData(BaseVisualObject * pVisualObject
        , Matrix4x4 modelMatrix
        , Matrix4x4 viewMatrix
        , Matrix4x4 projMatrix
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
        )
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                pPreparingBuildInDataCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        uint32_t materialCount = pVisualObject->getMaterialCount();
        for (uint32_t materialIndex = 0u; materialIndex < materialCount; ++materialIndex)
        {
            auto pMaterial = pVisualObject->getMaterial(materialIndex);
            //pre z pass.
            if (pMaterial->getPreZPass() != nullptr)
            {
                auto pPreZPass = pMaterial->getPreZPass();
                auto pPassOfPreZPass = pPreZPass->getPass();
                Bool32 hasMatrixObjectToNDC = VG_FALSE;
                Bool32 hasMatrixObjectToWorld = VG_FALSE;
                Bool32 hasMatrixObjectToView = VG_FALSE;
                Bool32 hasMatrixView = VG_FALSE;
                Bool32 hasMatrixProjection = VG_FALSE;
                //update building in matrix variable.
                auto info = pPassOfPreZPass->getBuildInDataInfo();
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
                        pPreZPass->_setBuildInMatrixData(type, mvpMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD)
                    {
                        pPreZPass->_setBuildInMatrixData(type, modelMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW)
                    {
                        pPreZPass->_setBuildInMatrixData(type, mvMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_VIEW)
                    {
                        pPreZPass->_setBuildInMatrixData(type, viewMatrix);
                    }
                    else if (type == Pass::BuildInDataType::MATRIX_PROJECTION)
                    {
                        pPreZPass->_setBuildInMatrixData(type, projMatrix);
                    }
                }
                pPreZPass->apply();
            }
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                pPreparingBuildInDataCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void RenderBinder::_setBuildInData(BaseVisualObject * pVisualObject
        , Matrix4x4 modelMatrix
        , Matrix4x4 viewMatrix
        , Matrix4x4 projMatrix
        , BaseScene *pScene
        , const PreZTarget *pPreZTarget
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
        , fd::CostTimer * pPreparingBuildInDataCostTimer
#endif //DEBUG and VG_ENABLE_COST_TIMER
    )
    {
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                pPreparingBuildInDataCostTimer->begin();
#endif //DEBUG and VG_ENABLE_COST_TIMER
        uint32_t materialCount = pVisualObject->getMaterialCount();
        for (uint32_t materialIndex = 0u; materialIndex < materialCount; ++materialIndex)
        {
            auto pMaterial = pVisualObject->getMaterial(materialIndex);
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
                    else if (type == Pass::BuildInDataType::LIGHTS_DATA)
                    {
                        //light data buffer.
                        if (m_lightTypeCount > 0)
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
                        }
                        //light textures.
                        auto lightPassTextureInfos = m_lightPassTextureInfos;
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
                    else if (type == Pass::BuildInDataType::PRE_Z_DEPTH_RESULT)
                    {
                        if (pPreZTarget != nullptr)
                        {
                            vg::PassTextureInfo::TextureInfo itemInfo = {
                                dynamic_cast<const Texture *>(pPreZTarget->getDepthAttachment()),
                                nullptr,
                                nullptr,
                                vk::ImageLayout::eUndefined,
                            };
                            PassTextureInfo info = {
                                1u,
                                &itemInfo,
                                VG_PASS_PRE_Z_TEXTURE_BINDING_PRIORITY,
                                vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
                                vk::ShaderStageFlagBits::eFragment,
                            };
                            if (pPass->hasTexture(VG_PASS_PRE_Z_TEXTURE_NAME) == VG_FALSE)
                            {
                                pPass->addTexture(VG_PASS_PRE_Z_TEXTURE_NAME, info);
                            }
                            else
                            {
                                pPass->setTexture(VG_PASS_PRE_Z_TEXTURE_NAME, info);
                            }
                        }
                    }
                }

                pPass->apply();
            }
        }

#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                pPreparingBuildInDataCostTimer->end();
#endif //DEBUG and VG_ENABLE_COST_TIMER
    }

    void RenderBinder::_bindVisualObject(BaseVisualObject *pVisublObject
        , BaseVisualObject::BindInfo & bindInfo
        , BaseVisualObject::BindResult *pResult
        )
    {
        pVisublObject->beginBind(bindInfo, pResult);
        if (static_cast<uint32_t>(m_bindedObjects.size()) == m_bindedObjectCount) {
            auto newSize = getNextCapacity(static_cast<uint32_t>(m_bindedObjects.size()));
            m_bindedObjects.resize(newSize);
        }

        m_bindedObjects[m_bindedObjectCount] = pVisublObject;

        ++m_bindedObjectCount;
    }

    void RenderBinder::_renderPassBegin(const BaseRenderTarget *pRenderTarget
        , const vk::RenderPass *pRenderPass
        , const vk::Framebuffer *pFramebuffer
        , CmdBuffer *pCmdBuffer
        )
    {
        const auto framebufferWidth = pRenderTarget->getFramebufferWidth();
        const auto framebufferHeight = pRenderTarget->getFramebufferHeight();
        const auto renderArea = pRenderTarget->getRenderArea();
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
    }

    void RenderBinder::_renderPassEnd(CmdBuffer *pCmdBuffer)
    {
        RenderPassEndInfo endInfo;
        CmdInfo cmdInfo;
        cmdInfo.pRenderPassEndInfo = &endInfo;
        pCmdBuffer->addCmd(cmdInfo);
    }
} //vg