#ifndef VG_MESH_DATA_H
#define VG_MESH_DATA_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/util/slot_map.hpp"

#define VG_VERTEX_SLOT_POSITION 0u
#define VG_VERTEX_SLOT_COLOR 1u
#define VG_VERTEX_SLOT_NORMAL 2u
#define VG_VERTEX_SLOT_TANGENT 3u
#define VG_VERTEX_SlOT_TextureCoordinate0 4u
#define VG_VERTEX_SLOT_TextureCoordinate1 5u
#define VG_VERTEX_SLOT_TextureCoordinate2 6u
#define VG_VERTEX_SLOT_TextureCoordinate3 7u
#define VG_VERTEX_SLOT_OTHER_MIN 8u

namespace vg
{
    struct MeshData
    {
        enum class DataType
        {
            FLOAT_ARRAY,
            INT_ARRAY,
            VECTOR_2_ARRAY,
            VECTOR_3_ARRAY,
            VECTOR_4_ARRAY,
            COLOR_32_ARRAY,
            COLOR_ARRAY,
            BEGIN_RANGE = FLOAT_ARRAY,
            END_RANGE = COLOR_ARRAY,
            RANGE_SIZE = (END_RANGE - BEGIN_RANGE + 1),
        };

        template <DataType type>
        struct DataTypeInfo
        {
            using ValueType = void;
            using BaseType = void ;
            const vk::Format static BASE_FORMAT = vk::Format::eUndefined;
        };

        template<>
        struct DataTypeInfo<DataType::FLOAT_ARRAY>
        {
            using ValueType = std::vector<float>;
            using BaseType = float;
            const vk::Format static BASE_FORMAT = vk::Format::eR32Sfloat;
        };

        template<>
        struct DataTypeInfo<DataType::INT_ARRAY>
        {
            using ValueType = std::vector<int32_t>;
            using BaseType = int32_t;
            const vk::Format static BASE_FORMAT = vk::Format::eR32Sint;
        };

        template<>
        struct DataTypeInfo<DataType::VECTOR_2_ARRAY>
        {
            using ValueType = std::vector<Vector2>;
            using BaseType = Vector2;
            const vk::Format static BASE_FORMAT = vk::Format::eR32G32Sfloat;
        };

        template<>
        struct DataTypeInfo<DataType::VECTOR_3_ARRAY>
        {
            using ValueType = std::vector<Vector3>;
            using BaseType = Vector3;
            const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32Sfloat;
        };

        template<>
        struct DataTypeInfo<DataType::VECTOR_4_ARRAY>
        {
            using ValueType = std::vector<Vector4>;
            using BaseType = Vector4;
            const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32A32Sfloat;
        };

        template<>
        struct DataTypeInfo<DataType::COLOR_32_ARRAY>
        {
            using ValueType = std::vector<Color32>;
            using BaseType = Color32;
            const vk::Format static BASE_FORMAT = vk::Format::eR8G8B8A8Unorm;
        };

        template<>
        struct DataTypeInfo<DataType::COLOR_ARRAY>
        {
            using ValueType = std::vector<Color>;
            using BaseType = Color;
            const vk::Format static BASE_FORMAT = vk::Format::eR32G32B32A32Sfloat;
        };

        inline static vk::Format getBaseFormatWithDataType(DataType dataType)
        {
            switch (dataType)
            {
            case DataType::FLOAT_ARRAY:
            {
                return DataTypeInfo<DataType::FLOAT_ARRAY>::BASE_FORMAT;
            }
            case DataType::INT_ARRAY:
            {
                return DataTypeInfo<DataType::INT_ARRAY>::BASE_FORMAT;
            }
            case DataType::VECTOR_2_ARRAY:
            {
                return DataTypeInfo<DataType::VECTOR_2_ARRAY>::BASE_FORMAT;
            }
            case DataType::VECTOR_3_ARRAY:
            {
                return DataTypeInfo<DataType::VECTOR_3_ARRAY>::BASE_FORMAT;
            }
            case DataType::VECTOR_4_ARRAY:
            {
                return DataTypeInfo<DataType::VECTOR_4_ARRAY>::BASE_FORMAT;
            }
            case DataType::COLOR_32_ARRAY:
            {
                return DataTypeInfo<DataType::COLOR_32_ARRAY>::BASE_FORMAT;
            }
            case DataType::COLOR_ARRAY:
            {
                return DataTypeInfo<DataType::COLOR_ARRAY>::BASE_FORMAT;
            }
            default:
                throw std::runtime_error("Can't get base format with data type.");
            }
        }

        struct DataInfo
        {
            uint32_t slot;
            DataType dataType;


            DataInfo();

            DataInfo(DataType dataType
                , uint32_t slot
                );

            DataInfo(const DataInfo &);

            DataInfo &operator=(const DataInfo &);
            Bool32 operator==(const DataInfo &target) const;
            Bool32 operator<(const DataInfo &target) const;
        };

        struct Data {
            DataInfo info;
            std::vector<Byte> data;
            uint32_t count;

            Data(DataInfo info = DataInfo()
                , std::vector<Byte> data = std::vector<Byte>()
                , uint32_t count = 0u
            );
        };
        using SlotMapType = SlotMap<Data>;
        
        struct Datas {
            SlotMapType datas;

            const std::set<SlotType> &getSlots() const;
            Bool32 hasData(const SlotType &slot) const;
            void removeData(const SlotType &slot);
            uint32_t getDataCount(const SlotType &slot) const;
            const DataInfo &getDataInfo(const SlotType &slot) const;
    
            void addData(const SlotType &slot, const DataInfo &info, void *src, uint32_t size);
            void getData(const SlotType &slot, void *dst, uint32_t size, uint32_t offset) const;
            void setData(const SlotType &slot, void *src, uint32_t size, uint32_t offset);
    
            template<typename T>
            void addData(const SlotType &slot, const DataInfo &info, const T &value);
            template<typename T>
            T getData(const SlotType &slot) const;
            template<typename T>
            void setData(const SlotType &slot, const T &value);
            
            template<typename T>
            void addData(const SlotType &slot, const DataInfo &info, const std::vector<T> &values);
            template <typename T>
            std::vector<T> getData(const SlotType &slot, const uint32_t count) const;
            template<typename T>
            void setData(const SlotType &slot, const std::vector<T> &values);
    
            template<typename T>
            void addData(const SlotType &slot, const DataInfo &info, const T * const pSrc, const uint32_t count);
            template<typename T>
            void getData(const SlotType &slot, const T * const pDst, const uint32_t count);
            template<typename T>
            void setData(const SlotType &slot, const T * const pSrc, const uint32_t count);

            uint32_t getDataBaseSize(const SlotType &slot) const;
            uint32_t getDataSize(const SlotType &slot) const;

            void memoryCopyData(const SlotType &slot
                , void* dst
                , uint32_t offset
                , uint32_t elementStart
                , uint32_t maxElementCount) const;
        };

        MeshData();

        std::array<Datas, static_cast<size_t>(DataType::RANGE_SIZE)> datas;

        template <DataType type>
        Bool32 hasData(const SlotType &slot) const;

        template <DataType type>
        void addData(const SlotType &slot, const DataInfo &info, const typename DataTypeInfo<type>::ValueType& value);

        template <DataType type>
        void removeData(const SlotType &slot);

        template <DataType type>
        const typename DataTypeInfo<type>::ValueType getData(const SlotType &slot) const;

        template <DataType type>
        void setData(const SlotType &slot, const typename DataTypeInfo<type>::ValueType& value);

        uint32_t static getDataBaseSize(DataType dataType);

        template <DataType type>
        uint32_t static getDataBaseSize();

        template <DataType type>
        uint32_t getDataSize(const SlotType &slot) const;

        void memoryCopyData(DataType type
            , const SlotType &slot
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;

        template <DataType type>
        void memoryCopyData(const SlotType &slot
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;

    };
}

#include "graphics/mesh/mesh_data.inl"

#endif // !VG_MESH_DATA_H
