#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/buffer_data/uniform_buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_option.hpp"

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

#endif //VG_PASS_DATA_HPP