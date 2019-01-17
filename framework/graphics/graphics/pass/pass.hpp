#ifndef VG_PASS_H
#define VG_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/pass/shader.hpp"
#include "graphics/global.hpp"
#include "graphics/pass/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_data.hpp"
#include "graphics/util/util.hpp"
#include "graphics/buffer_data/uniform_buffer_data.hpp"
#include "graphics/texture/texture_default.hpp"
#include "graphics/pass/pass_push_constant.hpp"
#include "graphics/pass/pass_specialization.hpp"
#include "graphics/binding_set/binding_set.hpp"

//to do 
//specilazation constant and push constant
namespace vg
{
    class Pass : public Base
    {
    public:
        enum class BuildInDataType
        {
            MATRIX_OBJECT_TO_NDC = 0,
            MAIN_CLOLOR = 1,
            MATRIX_OBJECT_TO_WORLD = 2,
            MATRIX_OBJECT_TO_VIEW = 3,
            MATRIX_VIEW = 4,
            MATRIX_PROJECTION = 5,
            POS_VIEWER = 6,
            COUNT = 7
        };

        struct BuildInDataInfo
        {
            struct Component {
                BuildInDataType type;
            };

            uint32_t componentCount;
            Component *pComponent;
            BuildInDataInfo(uint32_t componentCount = 0u, Component *pComponent = nullptr);
            BuildInDataInfo(const BuildInDataInfo &target);
            BuildInDataInfo &operator=(const BuildInDataInfo &target);
            Bool32 operator==(const BuildInDataInfo &target) const;
            Bool32 operator!=(const BuildInDataInfo &target) const;
        };

        using PipelineLayoutStateID = uint32_t;
        using PipelineStateID = uint32_t;
        struct PushConstantUpdateInfo
        {
            vk::ShaderStageFlags stageFlags;
            uint32_t offset;
            uint32_t size;
            const void *pData;
            PushConstantUpdateInfo(vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
                , uint32_t offset = 0u
                , uint32_t size = 0u
                , const void *pData = nullptr
                );
            PushConstantUpdateInfo(const PushConstantUpdateInfo &target);
            PushConstantUpdateInfo &operator=(const PushConstantUpdateInfo &target);
        };

        struct VertexInputFilterInfo
        {
            Bool32 filterEnable;
            uint32_t locationCount;
            uint32_t * pLocations;

            VertexInputFilterInfo(Bool32 filterEnable = VG_FALSE
                , uint32_t locationCount = 0u
                , uint32_t * pLocations = nullptr
                );

            VertexInputFilterInfo(const VertexInputFilterInfo &target);
            VertexInputFilterInfo &operator=(const VertexInputFilterInfo &target);
        };

        struct DepthBiasInfo
        {
            Bool32 enable;
            Bool32 dynamic;
            float constantFactor;
            float clamp;
            float slopeFactor;
            DepthBiasInfo(Bool32 enable = VG_FALSE
                , Bool32 dynamic = VG_FALSE
                , float constantFactor = 0.0f
                , float clamp = 0.0f
                , float slopeFactor = 0.0f
                );
            DepthBiasInfo(const DepthBiasInfo &target);
            DepthBiasInfo &operator=(const DepthBiasInfo &target);
            Bool32 operator==(const DepthBiasInfo &target) const;
            Bool32 operator!=(const DepthBiasInfo &target) const;
        };

        //When depth bias is dynamic, it is used to update used depth bias values.
        struct DepthBiasUpdateInfo
        {
            float constantFactor;
            float clamp;
            float slopeFactor;

            DepthBiasUpdateInfo(float constantFactor = 0.0f
                , float clamp = 0.0f
                , float slopeFactor = 0.0f
                );
            DepthBiasUpdateInfo(const DepthBiasUpdateInfo &target);
            DepthBiasUpdateInfo &operator=(const DepthBiasUpdateInfo &target);
        };

        Pass();
        Pass(Shader *pShader);
        ~Pass();

        const Shader *getShader() const;
        Shader *getShader();
        void setShader(Shader *pShader);

        Bool32 hasBuffer(std::string name) const;
        void addBuffer(std::string name, const PassBufferInfo &bufferInfo);
        void removeBuffer(std::string name);
        PassBufferInfo getBuffer(std::string name);
        void setBuffer(std::string name, const PassBufferInfo &bufferInfo);

        Bool32 hasTexture(std::string name) const;
        void addTexture(std::string name, const PassTextureInfo &texInfo);
        void removeTexture(std::string name);
        PassTextureInfo getTexture(std::string name) const;
        void setTexture(std::string name, const PassTextureInfo &texInfo);

        //external uniform buffer
        Bool32 hasExtUniformBuffer(std::string name) const;
        void addExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info);
        void removeExtUniformBuffer(std::string name);
        PassExtUniformBufferInfo getExtUniformBuffer(std::string name) const;
        void setExtUniformBuffer(std::string name, const PassExtUniformBufferInfo &info);

        Bool32 hasData(std::string name) const;
        void removeData(std::string name);

        void addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void addData(const std::string name, const PassDataInfo &info, const void *src, uint32_t size);
        void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
        void setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
        void setData(const std::string name, const void *src, uint32_t size, uint32_t offset);

        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const T &value);
        template<typename T>
        T getData(const std::string name) const;
        template<typename T>
        void setData(const std::string name, const T &value);
        
        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const std::vector<T> &values);
        template <typename T>
        std::vector<T> getData(const std::string name, const uint32_t count) const;
        template<typename T>
        void setData(const std::string name, const std::vector<T> &values);

        template<typename T>
        void addData(const std::string name, const PassDataInfo &info, const T * const pSrc, const uint32_t count);
        template<typename T>
        void getData(const std::string name, const T * const pDst, const uint32_t count);
        template<typename T>
        void setData(const std::string name, const T * const pSrc, const uint32_t count);


        Color getMainColor() const;
        void setMainColor(Color color);

        void setBuildInDataInfo(BuildInDataInfo info);
        const BuildInDataInfo &getBuildInDataInfo() const;

        vk::PolygonMode getPolygonMode() const;
        void setPolygonMode(vk::PolygonMode polygonMode);

        vk::CullModeFlags getCullMode() const;
        void setCullMode(vk::CullModeFlags cullMode);

        vk::FrontFace getFrontFace() const;
        void setFrontFace(vk::FrontFace frontFace);

        float getLineWidth() const;
        void setLineWidth(float lineWidth);

        const fd::Viewport &getViewport() const;
        void setViewport(const fd::Viewport &viewport);

        const fd::Rect2D &getScissor() const;
        void setScissor(const fd::Rect2D &scissor);

        const vk::PipelineDepthStencilStateCreateInfo &getDepthStencilInfo() const;
        void setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value);

        const vk::PipelineColorBlendStateCreateInfo &getColorBlendInfo() const;
        void setColorBlendInfo(const vk::PipelineColorBlendStateCreateInfo &value);

        const vk::PipelineInputAssemblyStateCreateInfo &getDefaultInputAssemblyState() const;
        void setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value);

        Bool32 hasSpecializationInfo(vk::ShaderStageFlagBits shaderStage) const;
        const vk::SpecializationInfo &getSpecializationInfo(vk::ShaderStageFlagBits shaderStage) const;

        const PassSpecializationData &getSpecializationData(vk::ShaderStageFlagBits shaderStage) const;
        const PassSpecializationData &getSpecializationData(vk::ShaderStageFlagBits shaderStage);

        Bool32 hasSpecializationItem(vk::ShaderStageFlagBits shaderStage, std::string name) const;
        void addSpecializationItem(vk::ShaderStageFlagBits shaderStage
            , std::string name
            , uint32_t priority
            , const void *pData
            , uint32_t size
            );
        template <typename T>
        void addSpecializationItem(vk::ShaderStageFlagBits shaderStage
            , std::string name
            , uint32_t priority
            , const T &data
            );
        void removeSpecializationItem(vk::ShaderStageFlagBits shaderStage, std::string name);
        void setSpecializationItem(vk::ShaderStageFlagBits shaderStage
            , std::string name
            , uint32_t priority
            , const void *pData
            , uint32_t size
            );
        template <typename T>
        void setSpecializationItem(vk::ShaderStageFlagBits shaderStage
            , std::string name
            , uint32_t priority
            , const T &data
            );

        Bool32 hasPushConstant(std::string name) const;
        void addPushConstant(std::string name
            , uint32_t priority
            , vk::ShaderStageFlags stageFlags
            , uint32_t size
            );
        void removePushConstant(std::string name);
        const PassPushConstantData::ConstantItem &getPushConstant(std::string name) const;
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

        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
        std::vector<PushConstantUpdateInfo> getPushconstantUpdates() const;

        uint32_t getInstanceCount() const;
        void setInstanceCount(uint32_t count);

        uint32_t getSubpass() const;
        void setSubpass(uint32_t subpass);

        const VertexInputFilterInfo &getVertexInputFilter() const;
        void setVertexInputFilterInfo(const VertexInputFilterInfo &value);

        const DepthBiasInfo &getDepthBiasInfo() const;
        void setDepthBiasInfo(const DepthBiasInfo &value);

        const DepthBiasUpdateInfo &getDepthBiasUpdateInfo() const;
        void setDepthBiasUpdateInfo(const DepthBiasUpdateInfo &value);

        const BindingSet &getBindingSet() const;
        uint32_t getDescriptorSetLayoutCount() const;
        const vk::DescriptorSetLayout *getDescriptorSetLayouts() const;
        uint32_t getDescriptorSetCount() const;
        const vk::DescriptorSet *getDescriptorSets() const;
        uint32_t getDescriptorDynamicOffsetCount() const;
        const uint32_t *getDescriptorDynamicOffsets() const;

        PipelineLayoutStateID getPipelineLayoutStateID() const;
        PipelineStateID getPipelineStateID() const;

        void apply();

        void beginRecord() const;
        void endRecord() const;
    private:
        Color m_mainColor;
        BindingSet m_bindingSet;
        BindingSet::DescriptorSetStateID m_bindingSetDescriptorSetStateID;
        PassExtUniformBuffers m_extUniformBuffers;
        Bool32 m_extUniformBufferChanged;

        vk::PolygonMode m_polygonMode;
        vk::CullModeFlags m_cullMode;
        vk::FrontFace m_frontFace;
        fd::Viewport m_viewport;
        fd::Rect2D m_scissor;
        vk::PipelineDepthStencilStateCreateInfo m_depthStencilInfo;
        std::vector<vk::PipelineColorBlendAttachmentState> m_colorBlendAttachmentStates;
        vk::PipelineColorBlendStateCreateInfo m_colorBlendInfo;
        float m_lineWidth;
        uint32_t m_instanceCount;
        uint32_t m_subpass;
        vk::PipelineInputAssemblyStateCreateInfo m_defaultInputAssemblyState;
        
        //each stage own a specilization constant data.
        std::unordered_map<vk::ShaderStageFlagBits, PassSpecializationData> m_mapSpecilizationDatas;
        vk::Bool32 m_specializationChanged;
        std::unordered_map<vk::ShaderStageFlagBits, Bool32> m_mapSpecializationDataChanges;
        
        PassPushConstantData m_pushConstant;

        BuildInDataInfo m_buildInDataInfo;
        std::vector<BuildInDataInfo::Component> m_buildInDataInfoComponents;

        VertexInputFilterInfo m_vertexInputFilterInfo;
        std::vector<uint32_t> m_vertexInputFilterLocations;

        DepthBiasInfo m_depthBiasInfo;
        DepthBiasUpdateInfo m_depthBiasUpdateInfo;

        ////////applied data

        //external uniform buffer.
        static Bool32 _compareExtUniformBufferInfo(const PassExtUniformBufferInfo &, const PassExtUniformBufferInfo &);
        std::set<PassExtUniformBufferInfo, Bool32(*)(const PassExtUniformBufferInfo &, const PassExtUniformBufferInfo &)> m_sortExtUniformBufferInfoSet;

        //all descriptor sets and layouts
        Bool32 m_descriptorSetsChanged;
        //all descriptor set layouts
        std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
        //all descriptor sets.
        std::vector<vk::DescriptorSet> m_descriptorSets;

        //dynamic offsets.
        Bool32 m_dynamicOffsetsChanged;
        std::vector<uint32_t> m_dynamicOffsets;

        //push constant Ranges.
        Bool32 m_pushConstantChanged;
        struct PushConstantSortInfo {
            std::string name;
            uint32_t priority;

            PushConstantSortInfo(std::string name = nullptr
                , uint32_t priority = 0u
                );
        };
        static Bool32 _comparePushConstantInfo(const PushConstantSortInfo &, const PushConstantSortInfo &);
        std::vector<vk::PushConstantRange> m_pushConstantRanges;
        std::set<PushConstantSortInfo, Bool32(*)(const PushConstantSortInfo &, const PushConstantSortInfo &)> m_sortedPushConstantItems;

        //specialization
        struct SpecializationSortInfo {
            std::string name;
            uint32_t priority;

            SpecializationSortInfo(std::string name = nullptr
                , uint32_t priority = 0u
                );
        };
        static Bool32 _compareSpecializationSortInfo(const SpecializationSortInfo &, const SpecializationSortInfo &);
        using SpecializationSortInfoSet = std::set<SpecializationSortInfo, Bool32(*)(const SpecializationSortInfo &, const SpecializationSortInfo &)>;
        struct SpecializationAppliedData {
            SpecializationSortInfoSet sortSet;
            vk::SpecializationInfo info;
            std::vector<vk::SpecializationMapEntry> mapEntries;
            std::vector<Byte> data;
            SpecializationAppliedData();
        };
        std::unordered_map<vk::ShaderStageFlagBits, SpecializationAppliedData> m_mapSpecializationAppliedData;

        //pipeline layout
        PipelineLayoutStateID m_pipelineLayoutStateID;
        PipelineStateID m_pipelineStateID;

        Shader *m_pShader;

        void _initDefaultBuildInDataInfo();

        void _updatePipelineLayoutStateID();
        void _updatePipelineStateID();
        
        void _applyUniformBufferDynamicOffsets();
    };
}

#include "graphics/pass/pass.inl"

#endif // !VG_PASS_H
