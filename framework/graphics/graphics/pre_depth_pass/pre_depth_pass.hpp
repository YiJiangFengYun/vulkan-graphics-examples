#ifndef VG_PRE_DEPTH_PASS_HPP
#define VG_PRE_DEPTH_PASS_HPP

#include "graphics/global.hpp"
#include "graphics/base.hpp"
#include "graphics/pass/shader.hpp"
#include "graphics/pass/pass.hpp"

namespace vg
{
    extern "C" const unsigned char VG_PRE_DEPTH_VERT_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_VERT_CODE_LEN;
    extern "C" const unsigned char VG_PRE_DEPTH_FRAG_CODE[];
    extern "C" const size_t VG_PRE_DEPTH_FRAG_CODE_LEN;
    class PreDepthPass : public Base
    {
    public:
        static const void * DEFAULT_VERT_SHADER_CODE;
        static const uint32_t DEFAULT_VERT_SHADER_CODE_SIZE;
        static const void * DEFAULT_FRAG_SHADER_CODE;
        static const uint32_t DEFAULT_FRAG_SHADER_CODE_SIZE;

        PreDepthPass();

        void setVertShaderModule(vk::ShaderModule * pVertShaderModule);

        void setBuildInDataInfo(Pass::BuildInDataInfo info);
        void _setBuildInMatrixData(Pass::BuildInDataType type, Matrix4x4 matrix);

        void setPolygonMode(vk::PolygonMode polygonMode);

        void setCullMode(vk::CullModeFlags cullMode);

        void setFrontFace(vk::FrontFace frontFace);

        void setLineWidth(float lineWidth);

        void setViewport(const fd::Viewport &viewport);

        void setScissor(const fd::Rect2D &scissor);

        void setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value);

        void setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value);

        void setVertexInputFilterInfo(const Pass::VertexInputFilterInfo &value);

        void apply();

        const Shader * getShader() const;
        const Pass * getPass() const;

    private:
        std::shared_ptr<Shader> m_pShader;
        std::shared_ptr<Pass> m_pPass;
        void _init();
    };
} //vg

#endif //VG_PRE_DEPTH_PASS_HPP