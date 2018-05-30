#ifndef VG_MESH_DATA_H
#define VG_MESH_DATA_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"

#define VG_VERTEX_POSITION_NAME "_Position"
#define VG_VERTEX_COLOR_NAME "_Color"
#define VG_VERTEX_NORMAL_NAME "_Normal"
#define VG_VERTEX_TANGENT_NAME "_Tangent"
#define VG_VERTEX_TextureCoordinate0_NAME "_TexCoord_0"
#define VG_VERTEX_TextureCoordinate1_NAME "_TexCoord_1"
#define VG_VERTEX_TextureCoordinate2_NAME "_TexCoord_2"
#define VG_VERTEX_TextureCoordinate3_NAME "_TexCoord_3"

#define VG_VERTEX_BINDING_PRIORITY_POSITION 0u
#define VG_VERTEX_BINDING_PRIORITY_COLOR 1u
#define VG_VERTEX_BINDING_PRIORITY_NORMAL 2u
#define VG_VERTEX_BINDING_PRIORITY_TANGENT 3u
#define VG_VERTEX_BINDING_PRIORITY_TextureCoordinate0 4u
#define VG_VERTEX_BINDING_PRIORITY_TextureCoordinate1 5u
#define VG_VERTEX_BINDING_PRIORITY_TextureCoordinate2 6u
#define VG_VERTEX_BINDING_PRIORITY_TextureCoordinate3 7u
#define VG_VERTEX_BINDING_PRIORITY_OTHER_MIN 8u

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
            std::string name;
            MeshData::DataType dataType;
            uint32_t bindingPriority;

            DataInfo();

            DataInfo(std::string name,
                MeshData::DataType dataType,
                uint32_t bindingPriority);

            DataInfo(const DataInfo &);

            DataInfo(const DataInfo &&);

            DataInfo &operator=(const DataInfo &);

            Bool32 operator ==(const DataInfo &target) const;

            Bool32 operator<(const DataInfo &target) const;
        };

        struct Data {
            std::vector<std::string> arrDataNames;
            std::unordered_map<std::string, std::vector<Byte>> mapDatas;
            std::unordered_map<std::string, uint32_t> mapDataCounts;
            std::unordered_map<std::string, DataInfo> mapDataInfos;

            const std::vector<std::string> getArrDataNames() const;
            Bool32 hasData(std::string name) const;
            void removeData(std::string name);
            uint32_t getDataCount(std::string name) const;
            const DataInfo &getDataInfo(std::string name) const;
    
            void addData(const std::string name, const DataInfo &info, void *src, uint32_t size);
            void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
            void setData(const std::string name, void *src, uint32_t size, uint32_t offset);
    
            template<typename T>
            void addData(const std::string name, const DataInfo &info, const T &value);
            template<typename T>
            T getData(const std::string name) const;
            template<typename T>
            void setData(const std::string name, const T &value);
            
            template<typename T>
            void addData(const std::string name, const DataInfo &info, const std::vector<T> &values);
            template <typename T>
            std::vector<T> getData(const std::string name, const uint32_t count) const;
            template<typename T>
            void setData(const std::string name, const std::vector<T> &values);
    
            template<typename T>
            void addData(const std::string name, const DataInfo &info, const T * const pSrc, const uint32_t count);
            template<typename T>
            void getData(const std::string name, const T * const pDst, const uint32_t count);
            template<typename T>
            void setData(const std::string name, const T * const pSrc, const uint32_t count);

            uint32_t getDataBaseSize(const std::string name) const;
            uint32_t getDataSize(const std::string name) const;

            void memoryCopyData(const std::string name
                , void* dst
                , uint32_t offset
                , uint32_t elementStart
                , uint32_t maxElementCount) const;
        };

        std::array<Data, static_cast<size_t>(DataType::RANGE_SIZE)> datas;

        template <DataType type>
        Bool32 hasData(std::string name) const;

        template <DataType type>
        void addData(std::string name, const DataInfo &info, const typename DataTypeInfo<type>::ValueType& value);

        template <DataType type>
        void removeData(std::string name);

        template <DataType type>
        const typename DataTypeInfo<type>::ValueType getData(std::string name) const;

        template <DataType type>
        void setData(std::string name, const typename DataTypeInfo<type>::ValueType& value);

        uint32_t static getDataBaseSize(DataType dataType);

        template <DataType type>
        uint32_t static getDataBaseSize();

        template <DataType type>
        uint32_t getDataSize(const std::string name) const;

        void memoryCopyData(DataType type
            , const std::string name
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;

        template <DataType type>
        void memoryCopyData(const std::string name
            , void* dst
            , uint32_t offset
            , uint32_t elementStart
            , uint32_t maxElementCount) const;

    };
}

#include "graphics/mesh/mesh_data.inl"

#endif // !VG_MESH_DATA_H
