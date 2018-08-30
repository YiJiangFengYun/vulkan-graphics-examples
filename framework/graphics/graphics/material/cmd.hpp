#ifndef VG_RENDER_PASS_INFO_HPP
#define VG_RENDER_PASS_INFO_HPP

#include "graphics/global.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/mesh/mesh.hpp"

namespace vg
{
    struct BarrierInfo
    {
        vk::PipelineStageFlags  srcStageMask; 
        vk::PipelineStageFlags  dstStageMask;
        vk::DependencyFlags dependencyFlags;
        uint32_t memoryBarrierCount;
        const vk::MemoryBarrier * pMemoryBarriers;
        uint32_t bufferMemoryBarrierCount;
        const vk::BufferMemoryBarrier * pBufferMemoryBarriers;
        uint32_t imageMemoryBarrierCount;
        const vk::ImageMemoryBarrier * pImageMemoryBarriers; 

        BarrierInfo(vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlags()
            , vk::PipelineStageFlags  dstStageMask = vk::PipelineStageFlags()
            , vk::DependencyFlags dependencyFlags = vk::DependencyFlags()
            , uint32_t memoryBarrierCount = 0u
            , const vk::MemoryBarrier * pMemoryBarriers = nullptr
            , uint32_t bufferMemoryBarrierCount = 0u
            , const vk::BufferMemoryBarrier * pBufferMemoryBarriers = nullptr
            , uint32_t imageMemoryBarrierCount = 0u
            , const vk::ImageMemoryBarrier * pImageMemoryBarriers = nullptr
            );
    };

    struct CmdDraw {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;

        CmdDraw(uint32_t vertexCount = 0u
            , uint32_t instanceCount = 0u
            , uint32_t firstVertex = 0u
            , uint32_t firstInstance = 0u
        );
    };

    struct CmdDrawIndexed 
    {
        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        uint32_t vertexOffset;
        uint32_t firstInstance;

        CmdDrawIndexed(uint32_t indexCount = 0u
            , uint32_t instanceCount = 0u
            , uint32_t firstIndex = 0u
            , uint32_t vertexOffset = 0u
            , uint32_t firstInstance = 0u
        );
    };

    struct RenderPassBeginInfo
    {
        const vk::RenderPass *pRenderPass;
        const vk::Framebuffer *pFramebuffer;
        uint32_t framebufferWidth;
        uint32_t framebufferHeight;
        fd::Rect2D renderArea;
        uint32_t clearValueCount;
        const vk::ClearValue *pClearValues;

        RenderPassBeginInfo(const vk::RenderPass *pRenderPass = nullptr
            , const vk::Framebuffer *pFramebuffer = nullptr
            , uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , fd::Rect2D renderArea = fd::Rect2D()
            , uint32_t clearValueCount = 0u
            , const vk::ClearValue *pClearValues = nullptr
            );
    };

    struct RenderPassInfo
    {
        const vk::RenderPass *pRenderPass;
        uint32_t subPassIndex;
        const vk::Framebuffer *pFramebuffer;
        uint32_t framebufferWidth;
        uint32_t framebufferHeight;

        Matrix4x4 projMatrix;
        Matrix4x4 viewMatrix;
        const Pass *pPass;
        Matrix4x4 modelMatrix;
        const BaseMesh *pMesh;
        uint32_t subMeshIndex;
        fd::Viewport viewport;
        fd::Rect2D scissor;
        const CmdDraw *pCmdDraw;
        const CmdDrawIndexed *pCmdDrawIndexed;
            
        RenderPassInfo(const vk::RenderPass *pRenderPass = nullptr
            , uint32_t subPassIndex = 0u
            , const vk::Framebuffer *pFramebuffer = nullptr
            , uint32_t framebufferWidth = 0u
            , uint32_t framebufferHeight = 0u
            , Matrix4x4 projMatrix = Matrix4x4(1.0f)
            , Matrix4x4 viewMatrix = Matrix4x4(1.0f)
            , const Pass *pPass = nullptr
            , Matrix4x4 modelMatrix = Matrix4x4(1.0f)
            , const BaseMesh *pMesh = nullptr
            , uint32_t subMeshIndex = 0u
            , fd::Viewport viewport = fd::Viewport()
            , fd::Rect2D scissor = fd::Rect2D()
            , const CmdDraw *pCmdDraw = nullptr
            , const CmdDrawIndexed *pCmdDrawIndexed = nullptr
            );
    };

    struct RenderPassEndInfo
    {
        RenderPassEndInfo();
    };

    struct CmdInfo
    {
        const RenderPassBeginInfo *pRenderPassBeginInfo;
        const RenderPassInfo *pRenderPassInfo;
        const RenderPassEndInfo *pRenderPassEndInfo;        
        const BarrierInfo *pBarrierInfo;
        CmdInfo();
    };

    class CmdBuffer 
    {
    public:
        CmdBuffer();
        uint32_t getCmdCount() const;
        const CmdInfo *getCmdInfos() const;
        void empty();
        void begin();
        void addCmd(CmdInfo cmdInfo);
        void end();
        void clear();
    
    private:
        uint32_t m_cmdInfoCount;
        uint32_t m_cmdInfoCapacity;
        std::vector<CmdInfo> m_cmdInfos;

        uint32_t m_renderPassBeginInfoCount;
        uint32_t m_renderPassBeginInfoCapacity;
        std::vector<RenderPassBeginInfo> m_renderPassBeginInfos;
        std::vector<uint32_t> m_renderPassBeginInfoToCmdInfoIndices;

        uint32_t m_renderPassInfoCount;
        uint32_t m_renderPassInfoCapacity;
        std::vector<RenderPassInfo> m_renderPassInfos;
        std::vector<uint32_t> m_renderPassInfoToCmdInfoIndices;

        uint32_t m_renderPassEndInfoCount;
        uint32_t m_renderPassEndInfoCapacity;
        std::vector<RenderPassEndInfo> m_renderPassEndInfos;
        std::vector<uint32_t> m_renderPassEndInfoToCmdInfoIndices;

        uint32_t m_clearValuesCount;
        uint32_t m_clearValuesCapacity;
        std::vector<std::vector<vk::ClearValue>> m_clearValues;

        uint32_t m_cmdDrawCount;
        uint32_t m_cmdDrawCapacity;
        std::vector<CmdDraw> m_cmdDraws;
        std::vector<uint32_t> m_cmdDrawToRenderPassInfoIndices;

        uint32_t m_cmdDrawIndexedCount;
        uint32_t m_cmdDrawIndexedCapacity;
        std::vector<CmdDrawIndexed> m_cmdDrawIndexeds;
        std::vector<uint32_t> m_cmdDrawIndexedToRenderPassInfoIndices;

        uint32_t m_barrierInfoCount;
        uint32_t m_barrierInfosCapacity;
        std::vector<BarrierInfo> m_barrierInfos;
        std::vector<uint32_t> m_barrierInfoToCmdInfoIndices;

        uint32_t m_memoryBarrierCount;
        uint32_t m_memoryBarrierCapacity;
        std::vector<std::vector<vk::MemoryBarrier>> m_memoryBarriers;

        uint32_t m_bufferMemoryBarrierCount;
        uint32_t m_bufferMemoryBarrierCapacity;
        std::vector<std::vector<vk::BufferMemoryBarrier>> m_bufferMemoryBarriers;

        uint32_t m_imageMemoryBarrierCount;
        uint32_t m_imageMemoryBarrierCapacity;
        std::vector<std::vector<vk::ImageMemoryBarrier>> m_imageMemoryBarriers;
    };
} //vg

#endif //VG_RENDER_PASS_INFO_HPP