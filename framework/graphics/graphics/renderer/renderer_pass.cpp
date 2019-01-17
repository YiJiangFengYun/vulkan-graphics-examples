#include "graphics/renderer/renderer_pass.hpp"

namespace vg
{

    const std::array<uint32_t, static_cast<size_t>(Pass::BuildInDataType::COUNT)> buildInDataTypeSizes = {
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_NDC>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MAIN_CLOLOR>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_WORLD>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_OBJECT_TO_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_VIEW>::Type),
        sizeof(BuildInDataTypeTypeInfo<Pass::BuildInDataType::MATRIX_PROJECTION>::Type)
    };

    RendererPass::_BuildInDataCache::_BuildInDataCache(Matrix4x4 matrixObjectToNDC
        , Color mainColor
        , Matrix4x4 matrixObjectToWorld
        , Matrix4x4 matrixObjectToView
        , Matrix4x4 matrixView
        , Matrix4x4 matrixProjection
        , Vector4 posViewer
        )
        : matrixObjectToNDC(matrixObjectToNDC)
        , mainColor(mainColor)
        , matrixObjectToWorld(matrixObjectToWorld)
        , matrixObjectToView(matrixObjectToView)
        , matrixView(matrixView)
        , matrixProjection(matrixProjection)
        , posViewer(posViewer)
    {

    }

    RendererPass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
        , posViewer(target.posViewer)
    {

    }

    RendererPass::_BuildInDataCache &RendererPass::_BuildInDataCache::operator=(const _BuildInDataCache &target)
    {
        matrixObjectToNDC = target.matrixObjectToNDC;
        mainColor = target.mainColor;
        matrixObjectToWorld = target.matrixObjectToWorld;
        matrixObjectToView = target.matrixObjectToView;
        matrixView = target.matrixView;
        matrixProjection = target.matrixProjection;
        posViewer = target.posViewer;
        return *this;
    }

    RendererPass::_BuildInDataCache::_BuildInDataCache(const _BuildInDataCache &&target)
        : matrixObjectToNDC(target.matrixObjectToNDC)
        , mainColor(target.mainColor)
        , matrixObjectToWorld(target.matrixObjectToWorld)
        , matrixObjectToView(target.matrixObjectToView)
        , matrixView(target.matrixView)
        , matrixProjection(target.matrixProjection)
        , posViewer(target.posViewer)
    {
    }

///RendererPass
    RendererPass::RendererPass()
        : m_pPass()
        , m_passPipelineLayoutStateID()
        , m_passPipelineStateID()
        , m_buildInDataCache()
        , m_bindingSet()
        , m_bindingSetDescriptorSetStateID()
        , m_initedBuildInData()
        , m_currBuildInDataInfo()
        , m_descriptorSetLayouts()
        , m_descriptorSets()
        , m_pPipelineLayout()
        , m_pipelineStateID()
 
    {
        _initBuildInData();
    }
        
    RendererPass::RendererPass(const vg::Pass *pPass)
        : RendererPass()
    {
        m_pPass = pPass;
        _initBuildInData();
    }

    void RendererPass::setPass(const vg::Pass *pPass)
    {
        m_pPass = pPass;
        _initBuildInData();
    }

    const vg::Pass *RendererPass::getPass() const
    {
        return m_pPass;
    }

    void RendererPass::setBuildInDataMatrix4x4(Pass::BuildInDataType type, Matrix4x4 matrix)
    {
        _updateBuildInData(type, matrix);
    }


    void RendererPass::setBuildInDataVector4(Pass::BuildInDataType type, Vector4 vector)
    {
        _updateBuildInData(type, vector);
    }

    void RendererPass::beginRecord()
    {
        _apply();
        m_bindingSet.beginRecord();
    }
        
    void RendererPass::endRecord()
    {
        m_bindingSet.endRecord();
    }

    uint32_t RendererPass::getDescriptorSetCount() const
    {
        return static_cast<uint32_t>(m_descriptorSets.size());
    }
        
    const vk::DescriptorSet *RendererPass::getDescriptorSets() const
    {
        return m_descriptorSets.data();
    }

    const vk::PipelineLayout *RendererPass::getPipelineLayout() const
    {
        return m_pPipelineLayout.get();
    }

    Pass::PipelineStateID RendererPass::getPipelineStateID() const
    {
        return m_pipelineStateID;
    }

    void RendererPass::_apply()
    {
        auto pPass = m_pPass;
        auto &bindingSet = m_bindingSet;
        _initBuildInData();
        bindingSet.apply();
        Bool32 descriptorSetsChanged = VG_FALSE;
        Bool32 pipelineLayoutChanged = VG_FALSE;
        Bool32 pipelineChanged = VG_FALSE;
        if (m_bindingSetDescriptorSetStateID != bindingSet.getDescriptorSetStateID()) {
            m_bindingSetDescriptorSetStateID = bindingSet.getDescriptorSetStateID();
            descriptorSetsChanged = VG_TRUE;
            pipelineLayoutChanged = VG_TRUE;
        }

        if (m_passPipelineLayoutStateID != pPass->getPipelineLayoutStateID()) {
            m_passPipelineLayoutStateID = pPass->getPipelineLayoutStateID();
            pipelineLayoutChanged = VG_TRUE;
        }

        if (m_passPipelineStateID != pPass->getPipelineStateID()) {
            m_passPipelineStateID = pPass->getPipelineStateID();
            pipelineChanged = VG_TRUE;
        }

        if (descriptorSetsChanged) {
            auto pLayout = m_bindingSet.getDescriptorSetLayout();
            auto pSet = m_bindingSet.getDescriptorSet();
            uint32_t layoutCount = pLayout != nullptr ? 1 : 0;
            uint32_t setCount = pSet != nullptr ? 1 : 0;
            layoutCount += pPass->getDescriptorSetLayoutCount();
            setCount += pPass->getDescriptorSetCount();

            if (layoutCount != setCount) {
                throw std::runtime_error(
                    "Descriptor Set layout count of bindingSet is not equal to count of its descriptor set."
                );
            }

            std::vector<vk::DescriptorSetLayout> setLayouts(layoutCount);
            std::vector<vk::DescriptorSet> sets(layoutCount);

            uint32_t index = 0u;
            if (pLayout != nullptr) {
                setLayouts[index] = *pLayout;
                sets[index] = *pSet;
                ++index;
            }

            auto count = m_pPass->getDescriptorSetLayoutCount();
            for (auto i = 0; i < count; ++i) {
                setLayouts[index] = *(pPass->getDescriptorSetLayouts() + i);
                sets[index] = *(pPass->getDescriptorSets() + i);
                ++index;
            }

            if (m_descriptorSetLayouts != setLayouts) {
                pipelineLayoutChanged = VG_TRUE;
                m_descriptorSetLayouts = setLayouts;
            }

            if (m_descriptorSets != sets) {
                m_descriptorSets = sets;
            }

            descriptorSetsChanged = VG_FALSE;
        }

        if (pipelineLayoutChanged) {
            vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
                vk::PipelineLayoutCreateFlags(),                         //flags
                static_cast<uint32_t>(m_descriptorSetLayouts.size()),    //setLayoutCount
                m_descriptorSetLayouts.data(),                           //pSetLayouts
                static_cast<uint32_t>(pPass->getPushConstantRanges().size()),       //pushConstantRangeCount
                pPass->getPushConstantRanges().data()                              //pPushConstantRanges
            };

            auto pDevice = pApp->getDevice();
            m_pPipelineLayout = fd::createPipelineLayout(pDevice, pipelineLayoutCreateInfo);

            pipelineLayoutChanged = VG_FALSE;
        }

        if (pipelineChanged) {
            _updatePipelineStateID();
            pipelineChanged = VG_FALSE;
        }

    }

    void RendererPass::_updatePipelineStateID()
    {
         ++m_pipelineStateID;
        if ( m_pipelineStateID == std::numeric_limits<Pass::PipelineStateID>::max())
        {
            m_pipelineStateID = 1;
        }
    }

    void RendererPass::_initBuildInData()
    {
        uint32_t size = 0u;
        Pass::BuildInDataInfo buildInDataInfo;
        if (m_pPass == nullptr) buildInDataInfo = m_pPass->getBuildInDataInfo();
        if (! m_initedBuildInData || m_currBuildInDataInfo != buildInDataInfo) {
            if (! m_initedBuildInData) m_initedBuildInData = VG_TRUE;
            auto componentCount = buildInDataInfo.componentCount;
            for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
            {
                Pass::BuildInDataType type = (*(buildInDataInfo.pComponent + componentIndex)).type;
                uint32_t count = static_cast<uint32_t>(Pass::BuildInDataType::COUNT);
                for (uint32_t i = 0; i < count; ++i)
                {
                    if (i == static_cast<uint32_t>(type))
                    {
                        size += buildInDataTypeSizes[i];
                        break;
                    }
                }
            }
    
            if (size > 0)
            {
                PassDataInfo info = {
                    VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY,
                    vk::ShaderStageFlagBits::eAllGraphics,
                };
                PassDataSizeInfo sizeInfo = {
                    size,
                };
                if (m_bindingSet.hasData(VG_PASS_BUILDIN_DATA_NAME) == VG_FALSE)
                {
                    m_bindingSet.addData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
                }
                else
                {
                    m_bindingSet.setData(VG_PASS_BUILDIN_DATA_NAME, info, sizeInfo);
                }
    
                uint32_t offset1 = 0u;
                for (uint32_t componentIndex = 0; componentIndex < componentCount; ++componentIndex)
                {
                    Pass::BuildInDataType type = (*(buildInDataInfo.pComponent + componentIndex)).type;
                    uint32_t count = static_cast<uint32_t>(Pass::BuildInDataType::COUNT);
                    uint32_t offset2= 0u;
                    for (uint32_t i = 0; i < count; ++i)
                    {
                        if (i == static_cast<uint32_t>(type))
                        {
                            m_bindingSet.setData(VG_PASS_BUILDIN_DATA_NAME
                                , ((char *)(&m_buildInDataCache) + offset2)
                                , buildInDataTypeSizes[static_cast<uint32_t>(type)]
                                , offset1);
                            break;
                        }
                        else
                        {
                            offset2 += buildInDataTypeSizes[i];
                        }
                    }
                    offset1 += buildInDataTypeSizes[static_cast<uint32_t>(type)];
                }
            }
            else if (m_bindingSet.hasData(VG_PASS_BUILDIN_DATA_NAME))
            {
                m_bindingSet.removeData(VG_PASS_BUILDIN_DATA_NAME);
            }
        }
    }

    RendererPassCache::RendererPassCache()
        : m_mapPasses()
        , m_mapPassesBack()
    {

    }

    RendererPassCache::~RendererPassCache()
    {

    }

    void RendererPassCache::begin()
    {
        m_mapPassesBack = m_mapPasses;
        m_mapPasses.clear();
    }

    RendererPass *RendererPassCache::get(const Pass *pPass)
    {
        RendererPass * pCurr;
        if (m_mapPasses[pPass->getID()] != nullptr) {
            pCurr = m_mapPasses[pPass->getID()].get();
        } else if (m_mapPassesBack[pPass->getID()] != nullptr) {
            m_mapPasses[pPass->getID()] = m_mapPassesBack[pPass->getID()];
            m_mapPassesBack.erase(pPass->getID());
            pCurr = m_mapPassesBack[pPass->getID()].get();
        } else {
            auto pNew = _createNewRendererPass(pPass);
            m_mapPasses[pPass->getID()] = pNew;
            pCurr = pNew.get();
        }

        return pCurr;
    }

    void RendererPassCache::end()
    {
        //Delete useless renderer passes.
        m_mapPassesBack.clear();
    }

    std::shared_ptr<RendererPass> RendererPassCache::_createNewRendererPass(const Pass *pPass)
    {
        auto pRendererPass = std::shared_ptr<RendererPass>{new RendererPass(pPass)};
        return pRendererPass;
    }


} //vg