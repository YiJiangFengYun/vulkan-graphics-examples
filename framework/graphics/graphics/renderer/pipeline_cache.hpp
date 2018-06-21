#ifndef VG_PIPELINE_CACHE_H
#define VG_PIPELINE_CACHE_H

#include <boost/functional/hash.hpp>

#include "graphics/global.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/buffer_data/vertex_data.hpp"
#include "graphics/buffer_data/index_data.hpp"

namespace boost {
    template<> 
    struct hash<vk::RenderPass> {
        size_t operator()(const vk::RenderPass& renderPass) const {
            return hash<VkRenderPass>()((VkRenderPass)renderPass);
        }
    };
}

namespace vg {
    class PipelineCache
    {
    public:
        struct Info {
            vk::RenderPass renderPass;
            const Pass *pPass;
            const VertexData *pVertexData;
            const IndexData *pIndexData;
            uint32_t indexSubIndex;

            Info(vk::RenderPass renderPass = vk::RenderPass()
                , const Pass *pPass = nullptr
                , const VertexData *pVertexData = nullptr
                , const IndexData *pIndexData = nullptr
                , uint32_t indexSubIndex = 0u
                );
        };

        struct InfoFullKey {
            vk::RenderPass renderPass;
            const Pass *pPass;
            const VertexData *pVertexData;
            const IndexData *pIndexData;
            uint32_t indexSubIndex;

            Pass::PipelineStateID passPipelineStateID;
            uint32_t passSubPass;
            vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
            vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;
            std::vector<vk::VertexInputBindingDescription> vertexBindingDeses;
            std::vector<vk::VertexInputAttributeDescription> vertexAttributeDeses;

            InfoFullKey(Info info);
            InfoFullKey(const InfoFullKey &);
            InfoFullKey& operator=(const InfoFullKey &);
            InfoFullKey() = delete;
        };

        struct Hash {
             size_t operator()(const InfoFullKey & info) const;
        };

        struct EqualNoState {
            Bool32 operator() (const InfoFullKey & lhs, const InfoFullKey & rhs) const;
        };

        struct EqualFull {
            Bool32 operator() (const InfoFullKey & lhs, const InfoFullKey & rhs) const;
        };

        PipelineCache();
        ~PipelineCache();
        /**
         * When start of new frame calling this mehod to move cache to back and make cache
         * to empty. After rendering process, we will pick useful pipeline from back.
         **/
        void begin();
        std::shared_ptr<vk::Pipeline> caching(const Info &info);
        /**
         * At end of render of frame, we call this method to delete all unuseful pipeline in back.
         **/
        void end();

    private:
        std::unordered_map<InfoFullKey, std::shared_ptr<vk::Pipeline>, Hash, EqualNoState> m_mapPipelineNoState;
        std::unordered_map<InfoFullKey, std::shared_ptr<vk::Pipeline>, Hash, EqualFull> m_mapPipelineFull;
        std::unordered_map<InfoFullKey, std::shared_ptr<vk::Pipeline>, Hash, EqualFull> m_mapPipelineFullBack;
        std::shared_ptr<vk::PipelineCache> m_pPipelineCache;
        std::shared_ptr<vk::Pipeline> _createNewPipeline(const Info &info);
        void _createPipelineCache();
    };
} //!vg

#endif //!VG_PIPELINE_CACHE_H