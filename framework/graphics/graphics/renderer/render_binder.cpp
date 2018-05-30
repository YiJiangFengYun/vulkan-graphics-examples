#include "graphics/renderer/render_binder.hpp"

#include "graphics/util/gemo_util.hpp"

namespace vg
{
    void fillValidVisualObjects(std::vector<VisualObject<SpaceType::SPACE_2> *> &arrPVObjs
        , uint32_t &PVObjIndex
        , const Transform<SpaceType::SPACE_2> *pTransform
        , Scene<SpaceType::SPACE_2> *pScene
        , Camera<SpaceType::SPACE_2> *pCamera
#ifdef USE_WORLD_BOUNDS
        , const fd::Bounds<SpaceTypeInfo<SpaceType::SPACE_2>::PointType> *pBounds
#endif
        );

    RenderBinder::RenderBinder(uint32_t framebufferWidth
        , uint32_t framebufferHeight
        )
        : m_framebufferWidth(framebufferWidth)
        , m_framebufferHeight(framebufferHeight)
        , m_bindedObjects()
        , m_bindedObjectCount(0u)
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

    void RenderBinder::bind(BaseScene *pScene
        , BaseCamera *pCamera
        , const PreZTarget *pPreZTarget
        , CmdBuffer *pPreZCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer        
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , PostRender *pPostRender
        , const PostRenderTarget *pPostRenderTarget
        , CmdBuffer *pPostRenderCmdBuffer
        )
    {
        _beginBind();
        _bind(pScene, 
            pCamera,
            pPreZTarget,
            pPreZCmdBuffer, 
            pBranchCmdBuffer, 
            pTrunkWaitBarrierCmdBuffer, 
            pTrunkRenderPassCmdBuffer,
            pPostRender,
            pPostRenderTarget,
            pPostRenderCmdBuffer
            );
        _endBind();
    }

    void RenderBinder::_beginBind()
    {
        m_bindedObjectCount = 0u;
    }

    void RenderBinder::_bind(BaseScene *pScene
        , BaseCamera *pCamera
        , const PreZTarget *pPreZTarget
        , CmdBuffer *pPreZCmdBuffer
        , CmdBuffer *pBranchCmdBuffer
        , CmdBuffer *pTrunkWaitBarrierCmdBuffer        
        , CmdBuffer *pTrunkRenderPassCmdBuffer
        , PostRender *pPostRender
        , const PostRenderTarget *pPostRenderTarget
        , CmdBuffer *pPostRenderCmdBuffer
        )
    {
        _beginBind();
            
        //scene make cmds.
        if (pScene->getSpaceType() == SpaceType::SPACE_2)
        {
            _bindScene2(dynamic_cast<Scene<SpaceType::SPACE_2> *>(pScene), 
                dynamic_cast<Camera<SpaceType::SPACE_2> *>(pCamera),
                pPreZTarget,
                pPreZCmdBuffer,
                pBranchCmdBuffer,                    
                pTrunkWaitBarrierCmdBuffer,
                pTrunkRenderPassCmdBuffer
                );
        } else if (pScene->getSpaceType() == SpaceType::SPACE_3)
        {
            _bindScene3(dynamic_cast<Scene<SpaceType::SPACE_3> *>(pScene), 
                dynamic_cast<Camera<SpaceType::SPACE_3> *>(pCamera), 
                pPreZTarget,
                pPreZCmdBuffer,
                pBranchCmdBuffer,                    
                pTrunkWaitBarrierCmdBuffer,
                pTrunkRenderPassCmdBuffer
                );
        } else {
            //todo
        }
        //post render make cmds.
        if (pPostRender != nullptr && pPostRenderTarget != nullptr && pPostRenderCmdBuffer != nullptr &&
            pPostRender->isValidBindToRender() == VG_TRUE
            )
        {
            auto pMaterial = pPostRender->getMaterial();
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
                        auto pColorTex = pPostRenderTarget->getColorAttachment();
                        PassTextureInfo info = {
                            pColorTex,
                            nullptr,
                            nullptr,
                            vk::ImageLayout::eUndefined,
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
                pPostRenderCmdBuffer,
            };
            
            pPostRender->beginBind(bindInfo, &result);
        }
        //post render end bind
        if (pPostRender != nullptr && pPostRenderTarget != nullptr && pPostRenderCmdBuffer != nullptr &&
            pPostRender->isValidBindToRender() == VG_TRUE)
        {
            pPostRender->endBind();
        }
        _endBind();    
    }

    void RenderBinder::_bindScene2(Scene<SpaceType::SPACE_2> *pScene
        , Camera<SpaceType::SPACE_2> *pCamera
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
        auto projMatrix3x3 = pScene->getProjMatrix(pCamera);
        auto projMatrix = tranMat3ToMat4(projMatrix3x3);
#ifdef USE_WORLD_BOUNDS
        auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pCamera);
#endif
        
        auto viewMatrix3x3 = pCamera->getTransform()->getMatrixWorldToLocal();
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

        //flat visual objects and filter them that is out of camera with its bounds.
        //allocate enough space for array to storage points.
        std::vector<SceneType::VisualObjectType *> validVisualObjects(visualObjectCount);
        uint32_t validVisualObjectCount(0u);
        auto pRoot = pScene->pRootTransformForVisualObject;
        fillValidVisualObjects(validVisualObjects
            , validVisualObjectCount
            , pRoot.get()
            , pScene
            , pCamera
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
            if (pPreZCmdBuffer != nullptr && pPreZTarget != nullptr) 
            {
                _setPreZBuildInData(pVisualObject
                    , modelMatrix
                    , viewMatrix
                    , projMatrix
                    , pPreZTarget
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
        , Camera<SpaceType::SPACE_2> *pCamera
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
                , pCamera
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
                //Filter obj out of camera view.
                auto boundsOfChild = dynamic_cast<Mesh<MeshDimType::SPACE_2> *>(pMeshOfChild)->getBounds();
#ifdef USE_WORLD_BOUNDS
                auto boundsOfChildInWorld = tranBoundsToNewSpace<Vector2>(boundsOfChild, pChild->getMatrixLocalToWorld(), VG_FALSE);
#endif //USE_WORLD_BOUNDS
                fd::Rect2D clipRect;    
#ifdef USE_WORLD_BOUNDS
                if (pViewBoundsInWorld->isIntersects(boundsOfChildInWorld) && pScene->isInView(pCamera, boundsOfChildInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInView(pCamera, pChild, boundsOfChild, &clipRect) == VG_TRUE)
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
        , Camera<SpaceType::SPACE_3> *pCamera
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

        auto projMatrix = pScene->getProjMatrix(pCamera);

#ifdef USE_WORLD_BOUNDS
        auto boundsOfViewInWorld = pScene->getViewBoundsInWorld(pCamera);
#endif

        auto viewMatrix = pCamera->getTransform()->getMatrixWorldToLocal();
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

        //Filter visualObject is out of camera with its bounds.
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
                    pScene->isInView(pCamera, boundsInWorld, &clipRect) == VG_TRUE)
#else 
                if (pScene->isInView(pCamera, pTransform, bounds, &clipRect) == VG_TRUE)
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
                //transform point from model coordinate system to camera coordinate system.
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
                if (pPreZCmdBuffer != nullptr && pPreZTarget != nullptr) 
                {
                    _setPreZBuildInData(pVisualObject
                        , modelMatrix
                        , viewMatrix
                        , projMatrix
                        , pPreZTarget
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
            //pre z pass.
            if (pPreZTarget != nullptr && 
                pMaterial->getPreZPass() != nullptr)
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
                    else if (type == Pass::BuildInDataType::PRE_Z_DEPTH_RESULT)
                    {
                        if (pPreZTarget != nullptr)
                        {
                            PassTextureInfo info = {
                                pPreZTarget->getDepthAttachment(),
                                nullptr,
                                nullptr,
                                vk::ImageLayout::eUndefined,
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

    void RenderBinder::_endBind()
    {
        for (uint32_t i = 0; i < m_bindedObjectCount; ++i) {
            m_bindedObjects[i]->endBind();
        }
        m_bindedObjectCount = 0u;
    }
} //vg