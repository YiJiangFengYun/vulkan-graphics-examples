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

            Info();
            Info(const Info &);
			Info(const Info &&);
			Info& operator=(const Info &);
			Bool32 operator==(const Info &) const;
			Bool32 operator!=(const Info &) const;
    
            Info(vk::RenderPass renderPass
                , const Pass *pPass
                , const VertexData *pVertexData
                , const IndexData *pIndexData
                , uint32_t indexSubIndex
                );
            

        };

        struct VertexInputBindingDescriptionHash {
            size_t operator()(const vk::VertexInputBindingDescription & vertexInputBindingDes) const;
        };

        struct VertexInputAttributeDescriptionHash {
            size_t operator()(const vk::VertexInputAttributeDescription & vertexInputAttrDes) const;
        };

        struct PipelineVertexInputStateCreateInfoHash {
            size_t operator()(const vk::PipelineVertexInputStateCreateInfo & pipelineVertexInputStateCreateInfo) const;
        };

        struct PipelineInputAssemblyStateCreateInfoHash {
            size_t operator()(const vk::PipelineInputAssemblyStateCreateInfo & pipelineInputAssemblyStateCreateInfo) const;
        };

        struct HashFull {
            size_t operator()(const Info& info) const;
        };

        struct HashNoState {
            size_t operator()(const Info& info) const;
        };

        PipelineCache();
        ~PipelineCache();
        /**
         * When start of new frame calling this mehod to move cache to back and make cache
         * to empty. After rendering process, we will pick useful pipeline from back.
         **/
        void start();
        std::shared_ptr<vk::Pipeline> caching(const Info &info);
        /**
         * At end of render of frame, we call this method to delete all unuseful pipeline in back.
         **/
        void end();

    private:
        std::unordered_map<Info, std::shared_ptr<vk::Pipeline>, HashNoState> m_mapPipelineNoState;
        std::unordered_map<Info, std::shared_ptr<vk::Pipeline>, HashFull> m_mapPipelineFull;
        std::unordered_map<Info, std::shared_ptr<vk::Pipeline>, HashFull> m_mapPipelineFullBack;
        std::shared_ptr<vk::PipelineCache> m_pPipelineCache;
        std::shared_ptr<vk::Pipeline> _createNewPipeline(const Info &info);
        void _createPipelineCache();
    };
} //!vg

#endif //!VG_PIPELINE_CACHE_H