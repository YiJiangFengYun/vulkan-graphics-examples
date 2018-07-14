#ifndef VG_MATERIAL_H
#define VG_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/pre_depth_pass/pre_depth_pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/material/cmd.hpp"
namespace vg
{
    class Material : public Base
    {
    public:
        struct BindInfo 
        {
            uint32_t preDepthFramebufferWidth;
            uint32_t preDepthFramebufferHeight;
            uint32_t trunkFramebufferWidth;
            uint32_t trunkFramebufferHeight;
            const Matrix4x4 *pProjMatrix;
            const Matrix4x4 *pViewMatrix;
            InstanceID objectID;
            const Matrix4x4 *pModelMatrix;
            const BaseMesh *pMesh;
            uint32_t subMeshIndex;
            Bool32 hasClipRect;
            const fd::Rect2D clipRect;
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
            fd::CostTimer *pPreparingPipelineCostTimer;
            fd::CostTimer *pPreparingCommandBufferCostTimer;
#endif //DEBUG and VG_ENABLE_COST_TIMER

            BindInfo(uint32_t preDepthFramebufferWidth = 0u
                , uint32_t preDepthFramebufferHeight = 0u
                , uint32_t trunkFramebufferWidth = 0u
                , uint32_t trunkFramebufferHeight = 0u
                , const Matrix4x4 *pProjMatrix = nullptr
                , const Matrix4x4 *pViewMatrix = nullptr
                , InstanceID objectID = 0
                , const Matrix4x4 *pModelMatrix = nullptr
                , const BaseMesh *pMesh = nullptr
                , uint32_t subMeshIndex = 0u
                , Bool32 hasClipRect = VG_FALSE
                , const fd::Rect2D clipRect = fd::Rect2D()
#if defined(DEBUG) && defined(VG_ENABLE_COST_TIMER)
                , fd::CostTimer *pPreparingPipelineCostTimer = nullptr
                , fd::CostTimer *pPreparingCommandBufferCostTimer = nullptr
#endif //DEBUG and VG_ENABLE_COST_TIMER
                );
        };
    
        struct BindResult
        {
            CmdBuffer *pPreDepthCmdBuffer;
            CmdBuffer *pBranchCmdBuffer;
            CmdBuffer *pTrunkRenderPassCmdBuffer;
            CmdBuffer *pTrunkWaitBarrierCmdBuffer;
            BindResult(CmdBuffer *pPreDepthCmdBuffer = nullptr
                , CmdBuffer *pBranchCmdBuffer = nullptr
                , CmdBuffer *pTrunkRenderPassCmdBuffer = nullptr
                , CmdBuffer *pTrunkWaitBarrierCmdBuffer = nullptr
                );
        };


        struct EndBindInfo 
        {
            InstanceID objectID;
            uint32_t subMeshIndex;
            EndBindInfo(InstanceID objectID = 0
                , uint32_t subMeshIndex = 0u);
        };

        struct MaterialCreateInfo
        {
            Bool32 onlyOnce;
            Bool32 createPreDepthPass;
            MaterialCreateInfo(Bool32 onlyOnce = VG_FALSE, Bool32 createPreDepthPass = VG_FALSE);
        };

        Material(Bool32 onlyOnce = VG_FALSE);
        Material(MaterialCreateInfo createInfo);
        ~Material();

        uint32_t getPassCount() const;
        const Pass *getPassWithIndex(uint32_t index) const;
        Pass *getPassWithIndex(uint32_t index);
        const Pass * const *getPasses() const;
        Pass * const *getPasses();
        Bool32 isHas(const Pass *pPass) const;

        const Shader *getMainShader() const;
        Shader *getMainShader();
        const Pass *getMainPass() const;
        Pass *getMainPass();
        

        const PreDepthPass * getPreDepthPass() const;
        PreDepthPass * getPreDepthPass();

        MaterialShowType getShowType();
        void setRenderQueueType(MaterialShowType type);
        uint32_t getRenderPriority();
        void setRenderPriority(uint32_t priority);
        /*Call the apply methods of all passes in the material.*/
        virtual void apply();

        void beginBind(const BindInfo info, BindResult *pResult) const;
        void endBind(const EndBindInfo info) const;

        std::string name;
    protected:
        Bool32 m_onlyOnce;
        std::shared_ptr<InstanceID> m_pBindTargetID; 
        MaterialShowType m_renderQueueType;
        uint32_t m_renderPriority;
        std::shared_ptr<Shader> m_pMainShader; 
        std::shared_ptr<Pass> m_pMainPass;

        std::shared_ptr<PreDepthPass> m_pPreDepthPass;

        std::vector<Pass *> m_arrPasses;
        std::unordered_map<InstanceID, Pass *> m_mapPasses;

        virtual void _addPass(Pass *pPass);
        virtual void _removePass(Pass *pPass);

        virtual void _beginBind(const BindInfo info, BindResult *pResult) const;
        virtual void _endBind(const EndBindInfo info) const;
    };
}

#endif // !VG_MATERIAL_H
