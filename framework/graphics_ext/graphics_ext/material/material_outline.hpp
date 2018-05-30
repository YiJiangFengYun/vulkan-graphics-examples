#ifndef VGE_MATERIAL_OUTLINE_HPP
#define VGE_MATERIAL_OUTLINE_HPP

#include "graphics_ext/global.hpp"

namespace vge
{
    class MaterialOutline : public vg::Material
    {
    public:
        struct CreateInfo
        {
            vg::Color outlineColor;

            CreateInfo(vg::Color outlineColor = vg::Color(1.0f));
        };

        MaterialOutline(CreateInfo info = CreateInfo());


        vg::Pass * getPassOutline() const;
        
        float getOutlineWidth() const;
        void setOutlineWidth(float value);

        vg::Color getOutlineColor() const;
        void setOutlineColor(vg::Color value);

        virtual void apply() override;

    private:
        CreateInfo m_info;
        std::shared_ptr<vg::Shader> m_pShaderOutline;
        std::shared_ptr<vg::Pass> m_pPassOutline;

        struct OutlineInfoVert
        {
            float outlineWidth = 1u;
        } m_outlineInfoVert;

        struct OutlineInfoFrag
        {
            vg::Color outlineColor = vg::Color(1.0f);
        } m_outlineInfoFrag;

        virtual void _beginBind(const BindInfo info, BindResult *pResult) const override;

        void _createOtherPasses();
        void _initPasses();
        
        void _applyInfos();

    };
} //vge

#endif //VGE_MATERIAL_OUTLINE_HPP