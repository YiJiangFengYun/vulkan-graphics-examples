#ifndef VG_PASS_PUSH_CONSTANT_HPP
#define VG_PASS_PUSH_CONSTANT_HPP

#include "graphics/global.hpp"

namespace vg
{
    struct PassPushConstantData 
    {
    public:
        struct ConstantRange {
            uint32_t size;
            // uint32_t offset;
            ConstantRange(uint32_t size = 0u
                // , uint32_t offset = 0u
                );
            ConstantRange(const ConstantRange &);
            ConstantRange& operator=(const ConstantRange &);
        };

        struct ConstantUpdate {
        public:
            ConstantUpdate(uint32_t offset = 0u
                , std::vector<Byte> data = std::vector<Byte>()
                , uint32_t size = 0u
                );
            
            ConstantUpdate(const ConstantUpdate &);
            ConstantUpdate& operator=(const ConstantUpdate &);
            
            void set(uint32_t offset
                , const void *pData
                , uint32_t size
                );
            
            template<typename T>
            void set(uint32_t offset
                , const T &data
                );

            void setData(const void *pData
                , uint32_t size
                );

            template <typename T>
            void setData(const T &data);

            uint32_t getOffset() const;
            const void *getData() const;
            uint32_t getSize() const;

            template<typename T>
            T getData() const;
        private:
            uint32_t m_offset;
            uint32_t m_size;
            std::vector<Byte> m_data;
        };

        struct ConstantItem {
        public:
            ConstantItem(uint32_t priority = 0u
                , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
                , uint32_t size = 0u
                );

            ConstantItem(const ConstantItem &);
            ConstantItem &operator=(const ConstantItem &);
            
            uint32_t getPriority() const;
            vk::ShaderStageFlags getStageFlags() const;
            const ConstantRange &getRange() const;
            ConstantRange &getRange();
            const ConstantUpdate &getUpdate() const;
            ConstantUpdate &getUpdate();
        private:
            uint32_t m_priority;
            vk::ShaderStageFlags m_stageFlags;
            ConstantRange m_range;
            ConstantUpdate m_update;
        };

        PassPushConstantData();

        Bool32 hasPushConstant(std::string name) const;
        void addPushConstant(std::string name
            , uint32_t priority
            , vk::ShaderStageFlags stageFlags
            , uint32_t size
            );
        void removePushConstant(std::string name);
        const ConstantItem &getPushConstant(std::string name) const;
        void setPushConstant(std::string name
            , uint32_t priority
            , vk::ShaderStageFlags stageFlags
            , uint32_t size
            );

        void setPushConstantUpdate(std::string name
            , uint32_t offset
            , const void *pData
            , uint32_t size
            );
            
        template<typename T>
        void setPushConstantUpdate(std::string name
            , uint32_t offset
            , const T &data
            );

        const std::vector<std::string> &getPushConstantNames() const;

    private:
        std::unordered_map<std::string, ConstantItem> m_mapItems;
        std::vector<std::string> m_arrItemNames;
    };
} //vg

#include "graphics/pass/pass_push_constant.inl"

#endif //VG_PASS_PUSH_CONSTANT_HPP