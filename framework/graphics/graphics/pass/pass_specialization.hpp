#ifndef VG_PASS_SPECIALIZATION_CONSTANT_HPP
#define VG_PASS_SPECIALIZATION_CONSTANT_HPP

#include "graphics/global.hpp"

namespace vg
{
    struct PassSpecializationData
    {
    public:
        struct SpecializationItem
        {
        public:
            SpecializationItem(uint32_t priority = 0u
                , std::vector<Byte> data = std::vector<Byte>()            
                , uint32_t size = 0u
                );
            
            SpecializationItem(uint32_t priority
                , const void *pData
                , uint32_t size
                );

            template <typename T>
            SpecializationItem(uint32_t priority
                , const T &data
                );

            SpecializationItem(const SpecializationItem &);
            SpecializationItem& operator=(const SpecializationItem &);

            void setData(const void *pData
                , uint32_t size);
            
            template <typename T>
            void setData(const T &data);

            uint32_t getPriority() const;
            const void *getData() const;
            uint32_t getSize() const;

            template <typename T>
            T getData() const;
            
        private:
            uint32_t m_priority;
            std::vector<Byte> m_data;        
            uint32_t m_size;
        };

        PassSpecializationData();
        Bool32 hasSpecialization(std::string name) const;
        void addSpecialization(std::string name
            , uint32_t priority
            , const void *pData
            , uint32_t size
            );
        template<typename T>
        void addSpecialization(std::string name
            , uint32_t priority
            , const T &data
            );
        void removeSpecialization(std::string name);
        const SpecializationItem &getSpecialization(std::string name) const;
        void setSpecialization(std::string name
            , uint32_t priority
            , const void *pData
            , uint32_t size
            );
        template <typename T>
        void setSpecialization(std::string name
            , uint32_t priority
            , const T &data
            );
        const std::vector<std::string> &getSpecializationNames() const;
    private:
        std::unordered_map<std::string, SpecializationItem> m_mapItems;
        std::vector<std::string> m_arrItemNames;
    };
} //vg

#include "graphics/pass/pass_specialization.inl"

#endif //VG_PASS_SPECIALIZATION_CONSTANT_HPP