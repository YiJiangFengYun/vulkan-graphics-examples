#ifndef VG_RENDERER_PASS_HPP
#define VG_RENDERER_PASS_HPP

#include "graphics/pass/pass.hpp"

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
    template<Pass::BuildInDataType type>
    struct BuildInDataTypeTypeInfo
    {
        using Type = void;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC>
    {
        using Type = Matrix4x4;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MAIN_CLOLOR>
    {
        using Type = Color;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD>
    {
        using Type = Matrix4x4;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW>
    {
        using Type = Matrix4x4;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_VIEW>
    {
        using Type = Matrix4x4;
    };
    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_PROJECTION>
    {
        using Type = Matrix4x4;
    };

    template<>
    struct BuildInDataTypeTypeInfo<Pass::BuildInDataType::POS_VIEWER>
    {
        using Type = Vector4;
    };

    const std::array<uint32_t, static_cast<size_t>(Pass::BuildInDataType::COUNT)> buildInDataTypeSizes = {
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MAIN_CLOLOR>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_VIEW>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_PROJECTION>::Type),
		sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::POS_VIEWER>::Type)
	};
    
    class RendererPass : public Base {
    public:
        struct _BuildInDataCache
        {
            Matrix4x4 matrixObjectToNDC;
            Color     mainColor;
            Matrix4x4 matrixObjectToWorld;
            Matrix4x4 matrixObjectToView;
            Matrix4x4 matrixView;
            Matrix4x4 matrixProjection;
            Vector4 posViewer;

            _BuildInDataCache(Matrix4x4 matrixObjectToNDC = Matrix4x4(1.0f)
                , Color mainColor = Color(1.0f)
                , Matrix4x4 matrixObjectToWorld = Matrix4x4(1.0f)
                , Matrix4x4 matrixObjectToView = Matrix4x4(1.0f)
                , Matrix4x4 matrixView = Matrix4x4(1.0f)
                , Matrix4x4 matrixProjection = Matrix4x4(1.0f)
                , Vector4 posViewer = Vector4(0.0f, 0.0f, 0.0f, 1.0f)
                );

            _BuildInDataCache(const _BuildInDataCache &target);
            _BuildInDataCache(const _BuildInDataCache &&target);
            _BuildInDataCache &operator=(const _BuildInDataCache &target);
        };

        RendererPass();
        RendererPass(const vg::Pass *pPass);
        void setPass(const vg::Pass *pPass);
        const vg::Pass *getPass() const;

        void setBuildInDataMatrix4x4(Pass::BuildInDataType type, Matrix4x4 matrix);
        void setBuildInDataVector4(Pass::BuildInDataType type, Vector4 vector);

        void beginRecord();
        void endRecord();

        uint32_t getDescriptorSetCount() const;
        const vk::DescriptorSet *getDescriptorSets() const;
        const vk::PipelineLayout *getPipelineLayout() const;
        Pass::PipelineStateID getPipelineStateID() const;

        BindingSet &getBindingSet();

    private:
        const Pass *m_pPass;
        Pass::PipelineLayoutStateID m_passPipelineLayoutStateID;
        Pass::PipelineStateID m_passPipelineStateID;
        _BuildInDataCache m_buildInDataCache;
        BindingSet m_bindingSet;
        BindingSet::DescriptorSetStateID m_bindingSetDescriptorSetStateID;
        Bool32 m_initedBuildInData;
        Pass::BuildInDataInfo m_currBuildInDataInfo;

        //all descriptor set layouts
        std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
        //all descriptor sets.
        std::vector<vk::DescriptorSet> m_descriptorSets;

        std::shared_ptr<vk::PipelineLayout> m_pPipelineLayout;

        Pass::PipelineStateID m_pipelineStateID;

        void _apply();

        void _updatePipelineStateID();

        void _initBuildInData();
        
        template <typename T>
        void _updateBuildInData(Pass::BuildInDataType type, T data);
    };

    class RendererPassCache
    {
    public:
        RendererPassCache();
        ~RendererPassCache();

        /**
         * When frame begin, It is called to move cache to back and make cache
         * to empty. After rendering process, we will pick useful pipeline from back.
         **/
        void begin();

        /**
         *  Pass and object is two dependencies of renderer pass. 
         **/
        RendererPass *get(const Pass *pPass, InstanceID objectID);

        /**
         * When frame end,  it is called to delete all useless cached pass.
         **/
        void end();

    private:
        //Map between pass and renderer pass, key is instance ID of pass.
        std::unordered_map<std::string, std::shared_ptr<RendererPass>> m_mapPasses;
        std::unordered_map<std::string, std::shared_ptr<RendererPass>> m_mapPassesBack;
        std::shared_ptr<RendererPass> _createNewRendererPass(const Pass *pPass);
    };
} //vg

#include "graphics/renderer/renderer_pass.inl"

#endif //VG_RENDERER_PASS_HPP