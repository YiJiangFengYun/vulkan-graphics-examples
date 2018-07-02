#include "graphics/pre_depth_pass/pre_depth_pass.hpp"

namespace vg
{
    const void * PreDepthPass::DEFAULT_VERT_SHADER_CODE = (const void *)VG_PRE_DEPTH_VERT_CODE;
    const uint32_t PreDepthPass::DEFAULT_VERT_SHADER_CODE_SIZE = VG_PRE_DEPTH_VERT_CODE_LEN;
    const void * PreDepthPass::DEFAULT_FRAG_SHADER_CODE = (const void *)VG_PRE_DEPTH_FRAG_CODE;
    const uint32_t PreDepthPass::DEFAULT_FRAG_SHADER_CODE_SIZE = VG_PRE_DEPTH_FRAG_CODE_LEN;

    PreDepthPass::PreDepthPass()
        : Base(BaseType::PRE_DEPTH_PASS)
        , m_pShader()
        , m_pPass()
    {
        m_pShader = std::shared_ptr<Shader>{ new Shader() };
        m_pPass = std::shared_ptr<Pass>{ new Pass(m_pShader.get()) };
        m_pShader->load(DEFAULT_VERT_SHADER_CODE, DEFAULT_VERT_SHADER_CODE_SIZE,
            DEFAULT_FRAG_SHADER_CODE, DEFAULT_FRAG_SHADER_CODE_SIZE);
        _init();
    }

    void PreDepthPass::setVertShaderModule(vk::ShaderModule * pVertShaderModule)
    {
        m_pShader->setVertShaderModule(pVertShaderModule);
    }

    void PreDepthPass::setBuildInDataInfo(Pass::BuildInDataInfo info)
    {
        m_pPass->setBuildInDataInfo(info);
    }

    void PreDepthPass::_setBuildInMatrixData(Pass::BuildInDataType type, Matrix4x4 matrix)
    {
        m_pPass->setBuildInMatrixData(type, matrix);
    }

    void PreDepthPass::setPolygonMode(vk::PolygonMode polygonMode)
    {
        m_pPass->setPolygonMode(polygonMode);
    }

    void PreDepthPass::setCullMode(vk::CullModeFlags cullMode)
    {
        m_pPass->setCullMode(cullMode);
    }

    void PreDepthPass::setFrontFace(vk::FrontFace frontFace)
    {
        m_pPass->setFrontFace(frontFace);
    }

    void PreDepthPass::setLineWidth(float lineWidth)
    {
        m_pPass->setLineWidth(lineWidth);
    }

    void PreDepthPass::setViewport(const fd::Viewport &viewport)
    {
        m_pPass->setViewport(viewport);
    }

    void PreDepthPass::setScissor(const fd::Rect2D &scissor)
    {
        m_pPass->setScissor(scissor);
    }

    void PreDepthPass::setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value)
    {
        m_pPass->setDepthStencilInfo(value);
    }

    void PreDepthPass::setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value)
    {
        m_pPass->setDefaultInputAssemblyState(value);
    }

    void PreDepthPass::setVertexInputFilterInfo(const Pass::VertexInputFilterInfo &value)
    {
        m_pPass->setVertexInputFilterInfo(value);
    }

    void PreDepthPass::apply()
    {
        m_pPass->apply();
    }

    const Shader * PreDepthPass::getShader() const
    {
        return m_pShader.get();
    }

    const Pass * PreDepthPass::getPass() const
    {
        return m_pPass.get();
    }

    void PreDepthPass::_init()
    {
        vg::Pass::BuildInDataInfo::Component buildInDataCmp = {
                {vg::Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC},
            };
        vg::Pass::BuildInDataInfo buildInDataInfo;
        buildInDataInfo.componentCount = 1u;
        buildInDataInfo.pComponent = &buildInDataCmp;
        m_pPass->setBuildInDataInfo(buildInDataInfo);
    }
} //vg