#ifndef VG_POST_RENDER_HPP
#define VG_POST_RENDER_HPP

#include "graphics/global.hpp"
#include "graphics/mesh/mesh_2.hpp"
#include "graphics/material/material.hpp"

namespace vg
{
    class PostRender
    {
    public:
        struct BindInfo 
        {
			uint32_t framebufferWidth;
			uint32_t framebufferHeight;

            BindInfo( uint32_t framebufferWidth = 0u
			    , uint32_t framebufferHeight = 0u
                );
        };
    
        struct BindResult
        {
           CmdBuffer *pPostRenderCmdBuffer;
           BindResult(CmdBuffer *pPostRenderCmdBuffer = nullptr
               );
        };

        PostRender();
        const Material * getMaterial() const;
        void setMaterial(const Material * pMaterial);

        Bool32 isValidBindToRender() const;
        void beginBind(const BindInfo info, BindResult *pResult) const;
		void endBind() const;

    private:
       const Material * m_pMaterial;
    };
} //vg

#endif //VG_POST_RENDER_HPP