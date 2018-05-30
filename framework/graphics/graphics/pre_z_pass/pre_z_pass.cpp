#include "graphics/pre_z_pass/pre_z_pass.hpp"

namespace vg
{
    const void * PreZPass::DEFAULT_VERT_SHADER_CODE = (const void *)VG_PRE_Z_VERT_CODE;
    const uint32_t PreZPass::DEFAULT_VERT_SHADER_CODE_SIZE = VG_PRE_Z_VERT_CODE_LEN;
    const void * PreZPass::DEFAULT_FRAG_SHADER_CODE = (const void *)VG_PRE_Z_FRAG_CODE;
    const uint32_t PreZPass::DEFAULT_FRAG_SHADER_CODE_SIZE = VG_PRE_Z_FRAG_CODE_LEN;

    PreZPass::PreZPass()
        : Base(BaseType::PRE_Z_PASS)
        , m_pShader()
        , m_pPass()
    {
        m_pShader = std::shared_ptr<Shader>{ new Shader() };
        m_pPass = std::shared_ptr<Pass>{ new Pass(m_pShader.get()) };
        m_pShader->load(DEFAULT_VERT_SHADER_CODE, DEFAULT_VERT_SHADER_CODE_SIZE,
            DEFAULT_FRAG_SHADER_CODE, DEFAULT_FRAG_SHADER_CODE_SIZE);
        _init();
    }

    void PreZPass::setVertShaderModule(vk::ShaderModule * pVertShaderModule)
    {
        m_pShader->setVertShaderModule(pVertShaderModule);
    }

    void PreZPass::setBuildInDataInfo(Pass::BuildInDataInfo info)
    {
        m_pPass->setBuildInDataInfo(info);
    }

    void PreZPass::_setBuildInMatrixData(Pass::BuildInDataType type, Matrix4x4 matrix)
    {
        m_pPass->setBuildInMatrixData(type, matrix);
    }

    void PreZPass::setPolygonMode(vk::PolygonMode polygonMode)
    {
        m_pPass->setPolygonMode(polygonMode);
    }

    void PreZPass::setCullMode(vk::CullModeFlags cullMode)
    {
        m_pPass->setCullMode(cullMode);
    }

    void PreZPass::setFrontFace(vk::FrontFace frontFace)
    {
        m_pPass->setFrontFace(frontFace);
    }

    void PreZPass::setLineWidth(float lineWidth)
    {
        m_pPass->setLineWidth(lineWidth);
    }

    void PreZPass::setViewport(const fd::Viewport &viewport)
    {
        m_pPass->setViewport(viewport);
    }

    void PreZPass::setScissor(const fd::Rect2D &scissor)
    {
        m_pPass->setScissor(scissor);
    }

    void PreZPass::setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value)
    {
        m_pPass->setDepthStencilInfo(value);
    }

    void PreZPass::setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value)
    {
        m_pPass->setDefaultInputAssemblyState(value);
    }

    void PreZPass::setVertexInputFilterInfo(const Pass::VertexInputFilterInfo &value)
    {
        m_pPass->setVertexInputFilterInfo(value);
    }

    void PreZPass::apply()
    {
        m_pPass->apply();
    }

    const Shader * PreZPass::getShader() const
    {
        return m_pShader.get();
    }

    const Pass * PreZPass::getPass() const
    {
        return m_pPass.get();
    }

    void PreZPass::_init()
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