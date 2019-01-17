#include "graphics/renderer/cmd_parser.hpp"

namespace vg
{
    CMDParser::ResultInfo::ResultInfo(uint32_t drawCount)
        : drawCount(drawCount)
    {
    }

    void CMDParser::record(CmdBuffer *pCmdBuffer
        , vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , RendererPassCache *pRendererPassCache
        , ResultInfo *pResult
        )
    {
        uint32_t drawCount = 0u;
        auto cmdInfoCount = pCmdBuffer->getCmdCount();
        uint32_t lastSubPassIndex = 0u;
        const vk::RenderPass *pRenderPass;
        const vk::Framebuffer *pFramebuffer;
        for (uint32_t cmdInfoIndex = 0u; cmdInfoIndex < cmdInfoCount; ++cmdInfoIndex)
        {
            const auto &cmdInfo = *(pCmdBuffer->getCmdInfos() + cmdInfoIndex);
            const auto &pRenderPassBeginInfo = cmdInfo.pRenderPassBeginInfo;
            if (pRenderPassBeginInfo != nullptr)
            {
                recordItemRenderPassBegin(pRenderPassBeginInfo, pCommandBuffer);
                lastSubPassIndex = 0u;
                pRenderPass = pRenderPassBeginInfo->pRenderPass;
                pFramebuffer = pRenderPassBeginInfo->pFramebuffer;
            }

            const auto &pRenderPassInfo = cmdInfo.pRenderPassInfo;
            if (pRenderPassInfo != nullptr)
            {
                if (pRenderPassInfo->subPassIndex - lastSubPassIndex == 1u) {
                    recordItemNextSubpass(pCommandBuffer);
                } else if (pRenderPassInfo->subPassIndex - lastSubPassIndex != 0u) {
                    throw std::runtime_error("Error of increasing of subpass index of render pass for cmd info.");
                } //else it is inner sub pass.

                if (pRenderPassInfo->pRenderPass == nullptr || pRenderPassInfo->pFramebuffer == nullptr) {
                    RenderPassInfo tempRenderPassInfo = *pRenderPassInfo;
                    if (tempRenderPassInfo.pRenderPass == nullptr)tempRenderPassInfo.pRenderPass = pRenderPass;
                    if (tempRenderPassInfo.pFramebuffer == nullptr)tempRenderPassInfo.pFramebuffer = pFramebuffer;
                    recordItem(&tempRenderPassInfo, pCommandBuffer, pPipelineCache, pRendererPassCache, pResult);
                } else {
                    recordItem(pRenderPassInfo, pCommandBuffer, pPipelineCache, pRendererPassCache, pResult);
                }
            
                lastSubPassIndex = pRenderPassInfo->subPassIndex;
                ++drawCount;
            }

            const auto &pRenderPassEndInfo = cmdInfo.pRenderPassEndInfo;
            if (pRenderPassEndInfo != nullptr)
            {
                pRenderPass = nullptr;
                pFramebuffer = nullptr;
                recordItemRenderPassEnd(pRenderPassEndInfo, pCommandBuffer);
            }

            auto pBarrierInfo = cmdInfo.pBarrierInfo;
            if (pBarrierInfo != nullptr)
            {
                pCommandBuffer->pipelineBarrier(pBarrierInfo->srcStageMask
                    , pBarrierInfo->dstStageMask
                    , pBarrierInfo->dependencyFlags
                    , pBarrierInfo->memoryBarrierCount
                    , pBarrierInfo->pMemoryBarriers
                    , pBarrierInfo->bufferMemoryBarrierCount
                    , pBarrierInfo->pBufferMemoryBarriers
                    , pBarrierInfo->imageMemoryBarrierCount
                    , pBarrierInfo->pImageMemoryBarriers
                    );
            }
        }

        if (pResult != nullptr)pResult->drawCount = drawCount;
    }

    void CMDParser::recordTrunkWaitBarrier(CmdBuffer *pTrunkWaitBarrierCmdBuffer
            , vk::CommandBuffer *pCommandBuffer
            )
    {
        if (pTrunkWaitBarrierCmdBuffer->getCmdCount() == 0) return;
        vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlags();
        vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlags();
        vk::DependencyFlags dependencyFlags = vk::DependencyFlags();
        uint32_t cmdCount = pTrunkWaitBarrierCmdBuffer->getCmdCount();
        for (uint32_t i = 0; i < cmdCount; ++i)
        {
            const auto &cmdInfo = *(pTrunkWaitBarrierCmdBuffer->getCmdInfos() + i);
            if (cmdInfo.pRenderPassInfo != nullptr) 
                VG_LOG(plog::warning) << "The render pass info in trunk wait barrier cmd buffer is invalid." << std::endl;
            const auto &trunkWaitBarrierInfo = *(cmdInfo.pBarrierInfo);
            pCommandBuffer->pipelineBarrier(trunkWaitBarrierInfo.srcStageMask, 
                trunkWaitBarrierInfo.dstStageMask, 
                trunkWaitBarrierInfo.dependencyFlags,
                trunkWaitBarrierInfo.memoryBarrierCount,
                trunkWaitBarrierInfo.pMemoryBarriers,
                trunkWaitBarrierInfo.bufferMemoryBarrierCount,
                trunkWaitBarrierInfo.pBufferMemoryBarriers,
                trunkWaitBarrierInfo.imageMemoryBarrierCount,
                trunkWaitBarrierInfo.pImageMemoryBarriers
                );
        }
    }

    void CMDParser::recordItemRenderPassBegin(const RenderPassBeginInfo *pRenderPassBeginInfo
            ,  vk::CommandBuffer *pCommandBuffer)
    {
        uint32_t framebufferWidth = pRenderPassBeginInfo->framebufferWidth;
        uint32_t framebufferHeight = pRenderPassBeginInfo->framebufferHeight;
        auto renderArea = pRenderPassBeginInfo->renderArea;
        vk::RenderPassBeginInfo renderPassBeginInfo = {
            * (pRenderPassBeginInfo->pRenderPass), 
            * (pRenderPassBeginInfo->pFramebuffer),
            vk::Rect2D(           
                vk::Offset2D(static_cast<int32_t>(std::round(framebufferWidth * renderArea.x))
                    , static_cast<int32_t>(std::round(framebufferHeight * renderArea.y))
                ),
                vk::Extent2D(static_cast<uint32_t>(std::round(framebufferWidth * renderArea.width)),
                    static_cast<uint32_t>(std::round(framebufferHeight * renderArea.height))
                )
            ),
            pRenderPassBeginInfo->clearValueCount,
            pRenderPassBeginInfo->pClearValues
        };

        pCommandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    }

    void CMDParser::recordItemRenderPassEnd(const RenderPassEndInfo *pRenderPassEndInfo
            ,  vk::CommandBuffer *pCommandBuffer)
    {
        pCommandBuffer->endRenderPass();
    }

    void CMDParser::recordItemRenderPassEnd(vk::CommandBuffer *pCommandBuffer)
    {
        pCommandBuffer->endRenderPass();
    }

    void CMDParser::recordItemNextSubpass(vk::CommandBuffer *pCommandBuffer)
    {
        pCommandBuffer->nextSubpass(vk::SubpassContents::eInline);
    }

    void CMDParser::recordItem(const RenderPassInfo *pRenderPassInfo
        ,  vk::CommandBuffer *pCommandBuffer
        , PipelineCache *pPipelineCache
        , RendererPassCache *pRendererPassCache
        , ResultInfo *pResult)
    {
        const auto &renderPassInfo = *pRenderPassInfo;
        auto pMesh = renderPassInfo.pMesh;

        if (pMesh != nullptr) pMesh->beginRecord();

        auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
        auto subMeshIndex = renderPassInfo.subMeshIndex;

        auto pPass = renderPassInfo.pPass;
        if (pPass != nullptr) pPass->beginRecord();

        auto pRendererPass = pRendererPassCache->get(pPass);
        if (pRendererPass) pRendererPass->beginRecord();

        auto pShader = pPass->getShader();
        auto stageInfos = pShader->getShaderStageInfos();
        if (stageInfos.size() != 0)
        {
            std::shared_ptr<vk::Pipeline> pPipeline;
            _createPipeline(renderPassInfo.pRenderPass,
                renderPassInfo.pMesh,
                subMeshIndex, 
                pPass, 
                pRendererPass,
                pPipelineCache,
                pPipeline);
            _recordCommandBuffer(pPipeline.get(),
                pCommandBuffer,
                renderPassInfo.framebufferWidth,
                renderPassInfo.framebufferHeight,
                renderPassInfo.pMesh,
                subMeshIndex, 
                pPass,
                pRendererPass,
                renderPassInfo.viewport,
                renderPassInfo.scissor,
                renderPassInfo.pCmdDraw,
                renderPassInfo.pCmdDrawIndexed
            );
        }
        else
        {
            VG_LOG(plog::warning) << "No one valid shader module for pass. Pass ID: " << pPass->getID() << std::endl;
        }

        if (pMesh != nullptr) pMesh->endRecord();
        if (pPass != nullptr) pPass->endRecord();
        if (pRendererPass) pRendererPass->endRecord();
    }

    void CMDParser::_createPipeline(const vk::RenderPass *pRenderPass,
        const BaseMesh *pMesh,
        uint32_t subMeshIndex,
        const Pass *pPass,
        const RendererPass *pRendererPass,
        PipelineCache *pPipelineCache,
        std::shared_ptr<vk::Pipeline> &pPipeline)
    {
        const vg::ContentMesh *pContentMesh = nullptr;
        if (pMesh != nullptr) {
            pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
        } 
        PipelineCache::Info info(
            *pRenderPass,
            pPass,
            pContentMesh != nullptr ? pContentMesh->getVertexData() : nullptr,
            pContentMesh != nullptr ? pContentMesh->getIndexData() : nullptr,
            subMeshIndex,
            pRendererPass
        );
        pPipeline = pPipelineCache->get(info);
    }

    void CMDParser::_recordCommandBuffer(vk::Pipeline *pPipeline,
        vk::CommandBuffer *pCommandBuffer,
        uint32_t framebufferWidth,
        uint32_t framebufferHeight,
        const BaseMesh *pMesh,
        uint32_t subMeshIndex,
        const Pass *pPass,
        const RendererPass *pRendererPass,
        const fd::Viewport viewport,
        const fd::Rect2D scissor,
        const CmdDraw * pCmdDraw,
        const CmdDrawIndexed * pCmdDrawIndexed
        )
    {   
        const auto& viewportOfPass = pPass->getViewport();
        const auto& scissorOfPass = pPass->getScissor();

        fd::Viewport finalViewport;
        //The mesh viewport is base on the viewport of pass.
        finalViewport.x = viewport.x * viewportOfPass.width + viewportOfPass.x;
        finalViewport.y = viewport.y * viewportOfPass.height + viewportOfPass.y;
        finalViewport.width = viewport.width * viewportOfPass.width;
        finalViewport.height = viewport.height * viewportOfPass.height;
        finalViewport.minDepth = viewport.minDepth * viewportOfPass.minDepth;
        finalViewport.maxDepth = viewport.maxDepth * viewportOfPass.maxDepth;

        //View port info.
        vk::Viewport vkViewport = {
            (float)framebufferWidth * finalViewport.x,                      //x
            (float)framebufferHeight * finalViewport.y,                     //y
            (float)framebufferWidth * finalViewport.width,                  //width
            (float)framebufferHeight * finalViewport.height,                 //height
            1.0f * finalViewport.minDepth,                                     //minDepth
            1.0f * finalViewport.maxDepth                                      //maxDepth
        };

        pCommandBuffer->setViewport(0, vkViewport);

        fd::Rect2D finalScissor = scissorOfPass;
        glm::vec2 minOfClipRect(scissor.x, scissor.y);
        glm::vec2 maxOfclipRect(scissor.x + scissor.width, scissor.y + scissor.height);
        fd::Bounds<glm::vec2> boundsOfClipRect(minOfClipRect, maxOfclipRect);
        glm::vec2 minOfScissorOfPass(scissorOfPass.x, scissorOfPass.y);
        glm::vec2 maxOfScissorOfPass(scissorOfPass.x + scissorOfPass.width, scissorOfPass.y + scissorOfPass.height);
        fd::Bounds<glm::vec2> boundsOfScissorOfPass(minOfScissorOfPass, maxOfScissorOfPass);
        fd::Bounds<glm::vec2> intersection;
        if (boundsOfScissorOfPass.intersects(boundsOfClipRect, &intersection))
        {
            auto min = intersection.getMin();
            auto size = intersection.getSize();
            finalScissor.x = min.x;
            finalScissor.y = min.y;
            finalScissor.width = size.x;
            finalScissor.height = size.y;
        }
        
        vk::Rect2D vkScissor = {
            {                               //offset
                static_cast<int32_t>(std::floor((float)framebufferWidth * viewportOfPass.x + 
                    (float)framebufferWidth * viewportOfPass.width * finalScissor.x)),    //x
                static_cast<int32_t>(std::floor((float)framebufferHeight * viewportOfPass.y +
                    (float)framebufferHeight * viewportOfPass.height * finalScissor.y))    //y
            },
            {                               //extent
                static_cast<uint32_t>(std::ceil((float)framebufferWidth * viewportOfPass.width * finalScissor.width)),   //width
                static_cast<uint32_t>(std::ceil((float)framebufferHeight * viewportOfPass.height * finalScissor.height))  //height
            }
        };

        pCommandBuffer->setScissor(0, vkScissor);

        auto pPipelineLayout = pRendererPass->getPipelineLayout();    

        //push constants
        auto pushConstantUpdates = pPass->getPushconstantUpdates();
        for(const auto& pushConstantUpdate : pushConstantUpdates)
        {
            pCommandBuffer->pushConstants(*pPipelineLayout, 
                pushConstantUpdate.stageFlags, 
                pushConstantUpdate.offset,
                pushConstantUpdate.size,
                pushConstantUpdate.pData
            );
        }

        //depth bias
        auto &depthBiasInfo = pPass->getDepthBiasInfo();
        auto &depthBiasUpdateInfo = pPass->getDepthBiasUpdateInfo();
        if (depthBiasInfo.enable == VG_TRUE && 
            depthBiasInfo.dynamic == VG_TRUE
            ) 
        {
            pCommandBuffer->setDepthBias(depthBiasUpdateInfo.constantFactor,
                depthBiasUpdateInfo.clamp,
                depthBiasUpdateInfo.slopeFactor
            );
        }


        pCommandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pPipeline);

        uint32_t descriptSetCount = pRendererPass->getDescriptorSetCount();
        auto pDescriptorSets = pRendererPass->getDescriptorSets();
        uint32_t dynamicOffsetCount = pRendererPass->getPass()->getDescriptorDynamicOffsetCount();
        auto pDynamicOffsets = pRendererPass->getPass()->getDescriptorDynamicOffsets();

        pCommandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pPipelineLayout, 
            0u, descriptSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);

        //dynamic line width
        pCommandBuffer->setLineWidth(pPass->getLineWidth());

        if (pMesh != nullptr) {
            auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
            const auto &pVertexData = pContentMesh->getVertexData();
            const auto &subVertexDatas = pVertexData->getSubVertexDatas();
            const auto &pIndexData = pContentMesh->getIndexData();
            const auto &subIndexDatas = pIndexData->getSubIndexDatas();
            const auto &subIndexData = subIndexDatas[subMeshIndex];
    
            vertexDataToCommandBuffer(*pCommandBuffer, pVertexData, subIndexData.vertexDataIndex);
            indexDataToCommandBuffer(*pCommandBuffer, pIndexData, subMeshIndex);
        }

        if (pCmdDraw != nullptr) {
            pCommandBuffer->draw(pCmdDraw->vertexCount, 
                pCmdDraw->instanceCount,
                pCmdDraw->firstVertex,
                pCmdDraw->firstInstance
                );
        } else if (pCmdDrawIndexed != nullptr) {
            pCommandBuffer->drawIndexed(pCmdDrawIndexed->indexCount,
                pCmdDrawIndexed->instanceCount,
                pCmdDrawIndexed->firstIndex,
                pCmdDrawIndexed->vertexOffset,
                pCmdDrawIndexed->firstInstance
                );
        } else if (pMesh != nullptr) {
            auto pContentMesh = dynamic_cast<const ContentMesh *>(pMesh);
            const auto &pIndexData = pContentMesh->getIndexData();
            const auto &subIndexDatas = pIndexData->getSubIndexDatas();
            const auto &subIndexData = subIndexDatas[subMeshIndex];

            uint32_t indexOffset = 0u;
            // for (uint32_t i = 0; i < subMeshIndex; ++i)
            // {
            //     indexOffset += subIndexDatas[i].indexCount;
            // }
    
            uint32_t vertexOffset = 0u;
            // for (uint32_t i = 0; i < subIndexData.vertexDataIndex; ++i)
            // {
            //     vertexOffset += subVertexDatas[i].vertexCount;
            // }
    
            uint32_t instanceOffset = 0u;
            uint32_t instanceCount = pPass->getInstanceCount();
    
            pCommandBuffer->drawIndexed(subIndexData.indexCount, 
                instanceCount, 
                indexOffset, 
                vertexOffset, 
                instanceOffset);
        } else {

        }
        //m_pCommandBuffer->draw(3, 1, 0, 0);
    }

} //vg