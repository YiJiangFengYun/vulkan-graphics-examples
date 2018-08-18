#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/buffer_data/uniform_buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_option.hpp"


#define VG_PASS_BUILDIN_DATA_NAME "_BuildIn"

#define VG_PASS_PRE_DEPTH_TEXTURE_NAME "_pre_depth_depth_tex"
#define VG_PASS_POST_RENDER_TEXTURE_NAME "_post_render_tex"
#define VG_PASS_LIGHT_DATA_BUFFER_NAME "_light_data_buffer"
#define VG_PASS_LIGHT_TEXTURE_NAME "_light_texture"
#define VG_PASS_LIGHT_RENDER_DATA_NAME "_light_render_data"

#define VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY 0
#define VG_PASS_LIGHT_RENDER_DATA_LAYOUT_PRIORITY 1
#define VG_PASS_OTHER_DATA_MIN_LAYOUT_PRIORITY 2

#define VG_PASS_PRE_DEPTH_TEXTURE_BINDING_PRIORITY 0
#define VG_PASS_POST_RENDER_TEXTURE_BINDING_PRIORITY 1
#define VG_PASS_LIGHT_DATA_BUFFER_BINDING_PRIORITY 2
#define VG_PASS_LIGHT_TEXTURE_MIN_BINDING_PRIORITY 3
#define VG_PASS_OTHER_MIN_BINDING_PRIORITY 100

namespace vg
{
    struct PassExtUniformBufferInfo 
    {
        UniformBufferData *pData;
        uint32_t subDataOffset;
        uint32_t subDataCount;
        uint32_t bindingPriority;
        PassExtUniformBufferInfo(UniformBufferData *pData = nullptr
            , uint32_t subDataOffset = 0u
            , uint32_t subDataCount = 0u
            , uint32_t bindingPriority = 0u
            );
        PassExtUniformBufferInfo(const PassExtUniformBufferInfo &);
        PassExtUniformBufferInfo& operator=(const PassExtUniformBufferInfo &);
    };


    struct PassExtUniformBuffers
    {    
        std::vector<std::string> arrExtUniformBufferNames;
        std::unordered_map<std::string, PassExtUniformBufferInfo> mapExtUniformBuffers;

        PassExtUniformBuffers();

        const std::vector<std::string> getExtUniformBufferNames() const;
        Bool32 hasExtUniformBuffer(std::string name) const;
        void addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo);
        void removeExtUniformBuffer(std::string name);
        const PassExtUniformBufferInfo &getExtUniformBuffer(std::string name) const;
        void setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &extUniformBufferInfo);
    };
} //vg

#include "graphics/pass/pass_data.inl"

#endif //VG_PASS_DATA_HPP