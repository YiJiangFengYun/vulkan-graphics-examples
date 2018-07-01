#ifndef VG_LIGHT_H
#define VG_LIGHT_H

#include "graphics/texture/texture.hpp"
#include "graphics/scene/object.hpp"

#define VG_LIGHT_DATA_TRANSFORM_NAME "_base"
#define VG_LIGHT_TEXTURE_DEPTH_NAME "_tex_depth"

#define VG_LIGHT_DATA_TRANSFORM_LAYOUT_PRIORITY 0

#define VG_LIGHT_TEXTURE_DEPTH_BINDING_PRIORITY 0

namespace vg
{
    struct LightDataInfo {
        uint32_t layoutPriority;
        LightDataInfo(uint32_t layoutPriority = 0u);
        LightDataInfo(const LightDataInfo &target);
        LightDataInfo &operator=(const LightDataInfo &target);
    };

    struct LightDataSizeInfo {
        uint32_t size;
        LightDataSizeInfo(uint32_t size = 0u);
        LightDataSizeInfo(const LightDataSizeInfo &target);
        LightDataSizeInfo &operator=(const LightDataSizeInfo &target);
        uint32_t getBufferSize() const;
    };

    struct LightTextureInfo {
        uint32_t bindingPriority;
        const Texture *pTexture;
        const Texture::ImageView *pImageView;
        const Texture::Sampler *pSampler;
        vk::ImageLayout imageLayout;

        LightTextureInfo(uint32_t bindingPriority = 0u
            , const Texture *pTexture = nullptr
            , const Texture::ImageView *pImageView = nullptr
            , const Texture::Sampler *pSampler = nullptr
            , vk::ImageLayout imageLayout = vk::ImageLayout()
            );
        LightTextureInfo(const LightTextureInfo &target);
        LightTextureInfo &operator=(const LightTextureInfo &target);
    };

    struct LightData {
        std::vector<std::string> arrDataNames;
        std::unordered_map<std::string, std::vector<Byte>> mapDatas;
        std::unordered_map<std::string, uint32_t> mapDataCounts;
        std::unordered_map<std::string, LightDataInfo> mapDataInfos;
        std::unordered_map<std::string, LightDataSizeInfo> mapDataSizeInfos;

        std::vector<std::string> arrTexNames;
        std::unordered_map<std::string, LightTextureInfo> mapTextures;

        LightData();

        const std::vector<std::string> getArrDataNames() const;
        Bool32 hasData(std::string name) const;
        void removeData(std::string name);
        const LightDataInfo &getDataInfo(std::string name) const;
        const LightDataSizeInfo &getDataSizeInfo(std::string name) const;

        void addData(const std::string name, const LightDataInfo &info, const LightDataSizeInfo &sizeInfo);
        void addData(const std::string name, const LightDataInfo &info, void *src, uint32_t size);
        void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const std::string name, const LightDataInfo &info, const LightDataSizeInfo &sizeInfo);
        void setData(const std::string name, void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const std::string name, const LightDataInfo &info, const T &value);
        template<typename T>
        T getData(const std::string name) const;
        template<typename T>
        void setData(const std::string name, const T &value);
        
        template<typename T>
        void addData(const std::string name, const LightDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const std::string name, const uint32_t count) const;
        template<typename T>
        void setData(const std::string name, const std::vector<T> &values);

        template<typename T>
        void addData(const std::string name, const LightDataInfo &info, const T * const pSrc, const uint32_t count);
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

        const std::vector<std::string> getArrTextureNames() const;
        Bool32 hasTexture(std::string name) const;
        void addTexture(std::string name, const LightTextureInfo &texInfo);
        void removeTexture(std::string name);
        const LightTextureInfo &getTextureInfo(std::string name) const;
        void setTexture(std::string name, const LightTextureInfo &texInfo);

    };

    struct LightExportTextureInfo
    {
        const Texture *pTexture;
        const Texture::ImageView *pImageView;
        const Texture::Sampler *pSampler;
        vk::ImageLayout imageLayout;
        LightExportTextureInfo(const Texture *pTexture = nullptr
            , const Texture::ImageView *pImageView = nullptr
            , const Texture::Sampler *pSampler = nullptr
            , vk::ImageLayout imageLayout = vk::ImageLayout()
            );
        LightExportTextureInfo(const LightExportTextureInfo &target);
        LightExportTextureInfo &operator=(const LightExportTextureInfo &target);
    };

    struct LightExportInfo {
        uint32_t dataSize;
        const void *pData;
        uint32_t textureCount;
        const LightExportTextureInfo *pTextureInfos;
        LightExportInfo(uint32_t dataSize = 0u
            , const void *pData = nullptr
            , uint32_t textureCount = 0u
            , const LightExportTextureInfo *pTextureInfos = nullptr
            );
        LightExportInfo(const LightExportInfo &target);
        LightExportInfo &operator=(const LightExportInfo &target);
    };

    class BaseLight : public Base
    {
    public:
        BaseLight();
        void apply();
        LightExportInfo getExportInfo() const;
    protected:
        LightData m_data;
        Bool32 m_dataChanged;
        Bool32 m_dataContentChanged;
        std::unordered_map<std::string, Bool32> m_dataContentChanges;
        Bool32 m_textureChanged;

        ////////applied data
        struct DataSortInfo {
            std::string name;
            uint32_t layoutPriority;
            uint32_t size;
            uint32_t bufferSize;

            DataSortInfo(std::string name = nullptr
                , uint32_t layoutPriority = 0u
                , uint32_t size = 0u
                , uint32_t bufferSize = 0u
                );
        };
        static Bool32 _compareDataInfo(const DataSortInfo &, const DataSortInfo &);
        std::set<DataSortInfo, Bool32(*)(const DataSortInfo &, const DataSortInfo &)> m_sortDataSet;
        std::vector<Byte> m_dataMemoryBuffer; //data result.

        struct TextureSortInfo {
            std::string name;
            uint32_t bindingPriority;
            const LightTextureInfo *pTextureInfo;

            TextureSortInfo(std::string name = nullptr
                , uint32_t bindingPriority = 0u
                , const LightTextureInfo *pTextureInfo = nullptr
                );
        };
        static Bool32 _compareTextureInfo(const TextureSortInfo &, const TextureSortInfo &);
        std::set<TextureSortInfo, Bool32(*)(const TextureSortInfo &, const TextureSortInfo &)> m_sortTexInfosSet;
        std::vector<LightExportTextureInfo> m_textureInfos; //texture result.


        virtual void _apply();
    };

    template <SpaceType SPACE_TYPE>
    class Light : public BaseLight, public Object<SPACE_TYPE>
    {
    public:
        Light();
    protected:
        virtual void _beginRender() override;
        virtual void _endRender() override;
    };
} //namespace kgs

#include "graphics/scene/light.inl"

#endif // !VG_LIGHT_H
