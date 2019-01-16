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

    const std::array<uint32_t, static_cast<size_t>(Pass::BuildInDataType::COUNT)> buildInDataTypeSizes;
    
    class RendererPass {
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
        RendererPass(vg::Pass pass);
        void setPass(vg::Pass pass);

        void setBuildInDataMatrix4x4(Pass::BuildInDataType type, Matrix4x4 matrix);
        void setBuildInDataVector4(Pass::BuildInDataType type, Vector4 vector);

    private:
        const vg::Pass *m_pPass;
        _BuildInDataCache m_buildInDataCache;
        BindingSet m_bindingSet;

        void _initBuildInData();
        
        template <typename T>
        void _updateBuildInData(Pass::BuildInDataType type, T data);

    };
} //vg

#include "graphics/renderer/renderer_pass.inl"

#endif //VG_RENDERER_PASS_HPP