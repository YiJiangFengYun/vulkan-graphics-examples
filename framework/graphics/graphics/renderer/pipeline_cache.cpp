#include "graphics/renderer/pipeline_cache.hpp"

namespace vg
{
    PipelineCache::Info::Info(vk::RenderPass renderPass
        , const Pass *pPass
        , const RendererPass *pRendererPass        
        , const VertexData *pVertexData
        , const IndexData *pIndexData
        , uint32_t indexSubIndex
        )
        : renderPass(renderPass)
        , pPass(pPass)
        , pRendererPass(pRendererPass)
        , pVertexData(pVertexData)
        , pIndexData(pIndexData)
        , indexSubIndex(indexSubIndex)
    {

    }

    PipelineCache::InfoFullKey::InfoFullKey(Info info)
        : renderPass(info.renderPass)
        , pPass(info.pPass)
        , pRendererPass(info.pRendererPass)
        , pVertexData(info.pVertexData)
        , pIndexData(info.pIndexData)
        , indexSubIndex(info.indexSubIndex)

        , passPipelineStateID(info.pRendererPass->getPipelineStateID())
        , passSubPass(info.pPass->getSubpass())
        , inputAssemblyStateInfo()
        , vertexInputStateInfo()
        , vertexBindingDeses()
        , vertexAttributeDeses()
    {
        if (info.pIndexData != nullptr) {
            const auto & subIndexDatas = info.pIndexData->getSubIndexDatas();
            const auto & subIndexData = subIndexDatas[info.indexSubIndex];
            inputAssemblyStateInfo = subIndexData.inputAssemblyStateInfo;

            if (info.pVertexData != nullptr) {
                const auto & subVertexDatas = info.pVertexData->getSubVertexDatas();
                const auto & subVertexData = subVertexDatas[subIndexData.vertexDataIndex];
                const auto & targetVertexInputStateInfo = subVertexData.vertexInputStateInfo;
                vertexInputStateInfo = targetVertexInputStateInfo;
                vertexBindingDeses.resize(vertexInputStateInfo.vertexBindingDescriptionCount);
                memcpy(vertexBindingDeses.data(), targetVertexInputStateInfo.pVertexBindingDescriptions, 
                    vertexInputStateInfo.vertexBindingDescriptionCount * sizeof(vk::VertexInputBindingDescription));
                vertexAttributeDeses.resize(vertexInputStateInfo.vertexAttributeDescriptionCount);
                memcpy(vertexAttributeDeses.data(), targetVertexInputStateInfo.pVertexAttributeDescriptions,
                    vertexInputStateInfo.vertexAttributeDescriptionCount * sizeof(vk::VertexInputAttributeDescription));
                vertexInputStateInfo.pVertexBindingDescriptions = vertexBindingDeses.data();
                vertexInputStateInfo.pVertexAttributeDescriptions = vertexAttributeDeses.data();
            }
        }
    }

    PipelineCache::InfoFullKey::InfoFullKey(const InfoFullKey & target)
        : renderPass(target.renderPass)
        , pPass(target.pPass)
        , pRendererPass(target.pRendererPass)
        , pVertexData(target.pVertexData)
        , pIndexData(target.pIndexData)
        , indexSubIndex(target.indexSubIndex)

        , passPipelineStateID(target.passPipelineStateID)
        , passSubPass(target.passSubPass)
        , inputAssemblyStateInfo(target.inputAssemblyStateInfo)
        , vertexInputStateInfo(target.vertexInputStateInfo)
        , vertexBindingDeses(target.vertexBindingDeses)
        , vertexAttributeDeses(target.vertexAttributeDeses)
    {

        vertexInputStateInfo.pVertexBindingDescriptions = vertexBindingDeses.data();
        vertexInputStateInfo.pVertexAttributeDescriptions = vertexAttributeDeses.data();
    }
            
    PipelineCache::InfoFullKey & PipelineCache::InfoFullKey::InfoFullKey::operator=(const InfoFullKey & target)
    {
        renderPass = target.renderPass;
        pPass = target.pPass;
        pRendererPass = target.pRendererPass;
        pVertexData = target.pVertexData;
        pIndexData = target.pIndexData;
        indexSubIndex = target.indexSubIndex;

        passPipelineStateID = target.passPipelineStateID;
        passSubPass = target.passSubPass;
        inputAssemblyStateInfo = target.inputAssemblyStateInfo;
        vertexInputStateInfo = target.vertexInputStateInfo;
        vertexBindingDeses = target.vertexBindingDeses;
        vertexAttributeDeses = target.vertexAttributeDeses;

        vertexInputStateInfo.pVertexBindingDescriptions = vertexBindingDeses.data();
        vertexInputStateInfo.pVertexAttributeDescriptions = vertexAttributeDeses.data();

        return *this;
    }

    size_t PipelineCache::Hash::operator()(const InfoFullKey & info) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, info.renderPass);
        boost::hash_combine(seed, info.pPass->getID());
        boost::hash_combine(seed, info.pRendererPass->getID());
        boost::hash_combine(seed, info.pVertexData != nullptr ? info.pVertexData->getID() : 0);
        boost::hash_combine(seed, info.pIndexData != nullptr ? info.pIndexData->getID() : 0);
        boost::hash_combine(seed, info.indexSubIndex);
        return seed;
    }

    Bool32 PipelineCache::EqualNoState::operator()(const InfoFullKey & lhs, const InfoFullKey & rhs) const
    {
        if (lhs.renderPass != rhs.renderPass) return VG_FALSE;
        if (lhs.pPass->getID() != rhs.pPass->getID()) return VG_FALSE;
        if (lhs.pRendererPass->getID() != rhs.pRendererPass->getID()) return VG_FALSE;
        if (lhs.pVertexData != nullptr && rhs.pVertexData != nullptr)
        {
            if (lhs.pVertexData->getID() != rhs.pVertexData->getID()) return VG_FALSE;
        } 
        else
        {
            if (lhs.pVertexData != rhs.pVertexData) return VG_FALSE;
        }

        if (lhs.pIndexData != nullptr && rhs.pIndexData != nullptr)
        {
            if (lhs.pIndexData->getID() != rhs.pIndexData->getID()) return VG_FALSE;
            if (lhs.indexSubIndex != rhs.indexSubIndex) return VG_FALSE;
        }
        else
        {
            if (lhs.pIndexData != rhs.pIndexData) return VG_FALSE;
        }


        return VG_TRUE;
    }

    Bool32 PipelineCache::EqualFull::operator()(const InfoFullKey & lhs, const InfoFullKey & rhs) const
    {
        if (EqualNoState()(lhs, rhs) == VG_FALSE) return VG_FALSE;
        if (lhs.passPipelineStateID != rhs.passPipelineStateID) return VG_FALSE;
        if (lhs.passSubPass != rhs.passSubPass) return VG_FALSE;
        if (lhs.pIndexData != nullptr && rhs.pIndexData != nullptr) {
            if (lhs.inputAssemblyStateInfo != rhs.inputAssemblyStateInfo) return VG_FALSE;
        }
        if (lhs.pVertexData != nullptr && rhs.pVertexData != nullptr) {
            if (lhs.vertexInputStateInfo.flags != rhs.vertexInputStateInfo.flags) return VG_FALSE;
            if (lhs.vertexInputStateInfo.vertexBindingDescriptionCount != rhs.vertexInputStateInfo.vertexBindingDescriptionCount) return VG_FALSE;
            if (lhs.vertexInputStateInfo.vertexAttributeDescriptionCount != rhs.vertexInputStateInfo.vertexAttributeDescriptionCount) return VG_FALSE;
            for (uint32_t i = 0; i < lhs.vertexInputStateInfo.vertexBindingDescriptionCount; ++i) {
                const auto & lhsBindingDes = *(lhs.vertexInputStateInfo.pVertexBindingDescriptions + i);
                const auto & rhsBindingDes = *(rhs.vertexInputStateInfo.pVertexBindingDescriptions + i);
                if (lhsBindingDes != rhsBindingDes) return VG_FALSE;
            }
            for (uint32_t i = 0; i < lhs.vertexInputStateInfo.vertexAttributeDescriptionCount; ++i) {
                const auto & lhsAttrDes = *(lhs.vertexInputStateInfo.pVertexAttributeDescriptions + i);
                const auto & rhsAttrDes = *(rhs.vertexInputStateInfo.pVertexAttributeDescriptions + i);
                if (lhsAttrDes != rhsAttrDes) return VG_FALSE;
            }
        }

        return VG_TRUE;
    }

    PipelineCache::PipelineCache()
        : m_mapPipelineNoState()
        , m_mapPipelineFull()
        , m_mapPipelineFullBack()
    {
        _createPipelineCache();
    }
    
    PipelineCache::~PipelineCache()
    {

    }

    void PipelineCache::begin()
    {
        m_mapPipelineFullBack = m_mapPipelineFull;
        m_mapPipelineFull.clear();
    }

    std::shared_ptr<vk::Pipeline> PipelineCache::get(const Info & info)
    {
        //delete old pipeline.

        auto fullInfo = InfoFullKey(info);

        auto pOldPipeline1 = m_mapPipelineNoState[fullInfo];

        auto pOldPipeline2 = m_mapPipelineFullBack[fullInfo];

        std::shared_ptr<vk::Pipeline> pPipeline;
        if (pOldPipeline1 != nullptr) 
        {
            if (pOldPipeline2 != nullptr) //Pipeline don't change.
            {
                m_mapPipelineFull[fullInfo] = pOldPipeline2;
                m_mapPipelineFullBack.erase(fullInfo);
                pPipeline = pOldPipeline2;
            } 
            else if (m_mapPipelineFull[fullInfo] != nullptr) //Pipeline don't change.
            {
                pPipeline = m_mapPipelineFull[fullInfo];
            }
            else
            {
                
                //has old pipeline. delete it from two map.
                m_mapPipelineNoState.erase(fullInfo);
                //m_mapPipelineFullBack.erase(info);
                pPipeline = _createNewPipeline(info);
                m_mapPipelineNoState[fullInfo] = pPipeline;
                m_mapPipelineFull[fullInfo] = pPipeline;
            }
        }
        else //Old pipeline don't exist.
        {
            pPipeline = _createNewPipeline(info);
            m_mapPipelineNoState[fullInfo] = pPipeline;
            m_mapPipelineFull[fullInfo] = pPipeline;
        }

        return pPipeline;
    }

    void PipelineCache::end()
    {
        //Delete unuseful pipelines.
        m_mapPipelineFullBack.clear();
    }

    std::shared_ptr<vk::Pipeline> PipelineCache::_createNewPipeline(const Info & info)
    {
        //Create graphics pipeline create info. 
        vk::GraphicsPipelineCreateInfo createInfo = {};

        //Construct shader stage create info.
        auto pPass = info.pPass;
        auto pRendererPass = info.pRendererPass;
        auto pShader = pPass->getShader();
        auto shaderStages = pShader->getShaderStageInfos();

        //Fill specialization data from pass.
        for (auto &shaderStage : shaderStages)
        {
            if (pPass->hasSpecializationInfo(shaderStage.stage)) 
            {
                shaderStage.pSpecializationInfo = &(pPass->getSpecializationInfo(shaderStage.stage));
            }
        }

        createInfo.stageCount = shaderStages.size();
        createInfo.pStages = shaderStages.data();

        uint32_t vertexSubIndex = 0u;

        if (info.pIndexData != nullptr) {
            const auto &pIndexData = info.pIndexData;
            const IndexData::SubIndexData &subIndexData = pIndexData->getSubIndexDatas()[info.indexSubIndex];
            createInfo.pInputAssemblyState = &subIndexData.inputAssemblyStateInfo;
            vertexSubIndex = subIndexData.vertexDataIndex;         
        } else {
            vk::PipelineInputAssemblyStateCreateInfo emptyAssemblyState = pPass->getDefaultInputAssemblyState();
            createInfo.pInputAssemblyState = &emptyAssemblyState;
        }

        vk::PipelineVertexInputStateCreateInfo newVertexInputStateInfo;
        std::vector<vk::VertexInputAttributeDescription> newAttrDeses;
        if (info.pVertexData != nullptr) {
            const auto &pVertexData = info.pVertexData;
            const VertexData::SubVertexData &subVertexData = pVertexData->getSubVertexDatas()[vertexSubIndex];

            //pass filter
            const auto &vertexInputFilter = pPass->getVertexInputFilter();
            if (vertexInputFilter.filterEnable == VG_TRUE)
            {
                const vk::PipelineVertexInputStateCreateInfo & originVertexInputStateInfo = subVertexData.vertexInputStateInfo;
                newVertexInputStateInfo = originVertexInputStateInfo;
                uint32_t attrDesCount = originVertexInputStateInfo.vertexAttributeDescriptionCount;                
                newAttrDeses.resize(attrDesCount); //allocate enough space and use its part.;
                uint32_t newAttrDesIndex = 0u;
                uint32_t filterDesCount = vertexInputFilter.locationCount;
                for (uint32_t attrDesIndex = 0u; attrDesIndex < attrDesCount; ++attrDesIndex)
                {
                    for (uint32_t filterDesIndex = 0u; filterDesIndex < filterDesCount; ++filterDesIndex)
                    {
                        if ((originVertexInputStateInfo.pVertexAttributeDescriptions + attrDesIndex)->location == 
                            *(vertexInputFilter.pLocations + filterDesIndex))
                        {
                            newAttrDeses[newAttrDesIndex] = *(originVertexInputStateInfo.pVertexAttributeDescriptions + attrDesIndex);
                            ++newAttrDesIndex;
                            break;
                        }
                    }
                }
                newVertexInputStateInfo.vertexAttributeDescriptionCount = newAttrDesIndex;
                newVertexInputStateInfo.pVertexAttributeDescriptions = newAttrDeses.data();
                createInfo.pVertexInputState = &newVertexInputStateInfo;
            }
            else
            {
                createInfo.pVertexInputState = &subVertexData.vertexInputStateInfo;
            }
        } else {
            vk::PipelineVertexInputStateCreateInfo emptyInputState{};
            createInfo.pVertexInputState = &emptyInputState;
        }

        auto polygonMode = pPass->getPolygonMode();
        auto cullMode = pPass->getCullMode();
        auto frontFace = pPass->getFrontFace();
        auto lineWidth = pPass->getLineWidth();
        auto &depthBiasInfo = pPass->getDepthBiasInfo();
        //Rasterization info.
        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
            vk::PipelineRasterizationStateCreateFlags(),  //flags
            VK_FALSE,                                     //depthClampEnable
            VK_FALSE,                                     //rasterizerDiscardEnable
            polygonMode,                                  //polygonMode
            cullMode,                                     //cullMode
            frontFace,                                    //frontFace
            depthBiasInfo.enable,                         //depthBiasEnable
            depthBiasInfo.constantFactor,                 //depthBiasConstantFactor
            depthBiasInfo.clamp,                          //depthBiasClamp
            depthBiasInfo.slopeFactor,                    //depthBiasSlopeFactor
            lineWidth                                     //lineWidth
        };
        createInfo.pRasterizationState = &rasterizationStateCreateInfo;

        auto depthStencilStateInfoOfPass = pPass->getDepthStencilInfo();

        //depth and stencil info.
        createInfo.pDepthStencilState = &depthStencilStateInfoOfPass;

        const auto& colorBlendInfoOfPass = pPass->getColorBlendInfo();
        //color blend info
        vk::PipelineColorBlendAttachmentState defaultColorBlendAttachmentState = {
            VK_FALSE,                                //blendEnable
            vk::BlendFactor::eOne,                  //srcColorBlendFactor
            vk::BlendFactor::eZero,                  //dstColorBlendFactor
            vk::BlendOp::eAdd,                       //colorBlendOp
            vk::BlendFactor::eOne,                  //srcAlphaBlendFactor
            vk::BlendFactor::eZero,                  //desAlphaBlendFactor
            vk::BlendOp::eAdd,                       //alphaBlendOp
            vk::ColorComponentFlagBits::eR
            | vk::ColorComponentFlagBits::eG
            | vk::ColorComponentFlagBits::eB
            | vk::ColorComponentFlagBits::eA  //colorWriteMask
        };

        uint32_t attachmentCount = colorBlendInfoOfPass.attachmentCount;
        if (attachmentCount <= 0) attachmentCount = 1u;
        std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates(attachmentCount);
        for (uint32_t i = 0; i < attachmentCount; ++i)
        {
            if (i < colorBlendInfoOfPass.attachmentCount)
            {
                colorBlendAttachmentStates[i] = *(colorBlendInfoOfPass.pAttachments + i);
            }
            else
            {
                colorBlendAttachmentStates[i] = defaultColorBlendAttachmentState;
            }
        }

        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = colorBlendInfoOfPass;
        colorBlendStateCreateInfo.attachmentCount = colorBlendAttachmentStates.size();
        colorBlendStateCreateInfo.pAttachments = colorBlendAttachmentStates.data();
        createInfo.pColorBlendState = &colorBlendStateCreateInfo;

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {
            vk::PipelineViewportStateCreateFlags(),                  //flags
            1u,                                                      //viewportCount
            nullptr,                                               //pViewports
            1u,                                                      //scissorCount
            nullptr                                                 //pScissors
        };
        createInfo.pViewportState = &viewportStateCreateInfo;

        //Multisample info.
        vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
            vk::PipelineMultisampleStateCreateFlags(),              //flags
            vk::SampleCountFlagBits::e1,                            //rasterizationSamples
            VK_FALSE,                                               //sampleShadingEnable
            0.0f,                                                   //minSampleShading
            nullptr,                                                //pSampleMask
            VK_FALSE,                                               //alphaToCoverageEnable
            VK_FALSE                                                //alphaToOneEnable
        };
        createInfo.pMultisampleState = &multisampleStateCreateInfo;

        std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
            vk::DynamicState::eLineWidth
        };

        if (depthBiasInfo.dynamic == VG_TRUE) {
            dynamicStates.resize(dynamicStates.size() + 1u);
            dynamicStates[dynamicStates.size() - 1u] = vk::DynamicState::eDepthBias;
        }

        vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
            vk::PipelineDynamicStateCreateFlags(),
            dynamicStates.size(),
            dynamicStates.data()
        };
        createInfo.pDynamicState = &dynamicStateCreateInfo;        

        createInfo.layout = *pRendererPass->getPipelineLayout();

        createInfo.renderPass = info.renderPass;
        createInfo.subpass = pPass->getSubpass();
        createInfo.basePipelineHandle = nullptr;
        createInfo.basePipelineIndex = -1;
        
        auto pDevice = pApp->getDevice();
        auto pPipeline = fd::createGraphicsPipeline(pDevice, *m_pPipelineCache, createInfo);

        return pPipeline;
    }

    void PipelineCache::_createPipelineCache()
    {
        vk::PipelineCacheCreateInfo createInfo = {
            vk::PipelineCacheCreateFlags()
        };
        auto pDevice = pApp->getDevice();
        m_pPipelineCache = fd::createPipelineCache(pDevice, createInfo);
    }
} //!vg