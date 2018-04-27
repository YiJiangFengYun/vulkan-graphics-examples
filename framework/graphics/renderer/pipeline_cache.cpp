#include "graphics/renderer/pipeline_cache.hpp"

namespace vg
{
    PipelineCache::Info::Info()
        : renderPass()
        , pPass()
        , pVertexData()
        , pIndexData()
        , indexSubIndex()
    {

    }

    PipelineCache::Info::Info(const Info &target)
        : renderPass(target.renderPass)
        , pPass(target.pPass)
        , pVertexData(target.pVertexData)
        , pIndexData(target.pIndexData)
        , indexSubIndex(target.indexSubIndex) 
    {
        
    }
			
    PipelineCache::Info::Info(const Info &&target)
        : renderPass(std::move(target.renderPass))
        , pPass(std::move(target.pPass))
        , pVertexData(std::move(target.pVertexData))
        , pIndexData(std::move(target.pIndexData))
        , indexSubIndex(std::move(target.indexSubIndex))
    {

    }

	PipelineCache::Info& PipelineCache::Info::operator=(const Info &target)
    {
        renderPass = target.renderPass;
        pPass = target.pPass;
        pVertexData = target.pVertexData;
        pIndexData = target.pIndexData;
        indexSubIndex = target.indexSubIndex;
        return *this;
    }

    Bool32 PipelineCache::Info::operator==(const Info &rhs) const
    {
		if (renderPass != rhs.renderPass) return VG_FALSE;
		if (pPass->getID() != rhs.pPass->getID()) return VG_FALSE;
		if (pVertexData != nullptr && rhs.pVertexData != nullptr)
		{
			if (pVertexData->getID() != rhs.pVertexData->getID()) return VG_FALSE;
		} 
		else
		{
			if (pVertexData != rhs.pVertexData) return VG_FALSE;
		}

		if (pIndexData != nullptr && rhs.pIndexData != nullptr)
		{
			if (pIndexData->getID() != rhs.pIndexData->getID()) return VG_FALSE;
			if (indexSubIndex != rhs.indexSubIndex) return VG_FALSE;
		}
		else
		{
			if (pIndexData != rhs.pIndexData) return VG_FALSE;
		}


		return VG_TRUE;
    }

	Bool32 PipelineCache::Info::operator!=(const Info &rhs) const
    {
        return !operator==(rhs);
    }

    PipelineCache::Info::Info(vk::RenderPass renderPass
        , Pass *pPass
        , VertexData *pVertexData
        , IndexData *pIndexData
        , uint32_t indexSubIndex
        )
        : renderPass(renderPass)
        , pPass(pPass)
        , pVertexData(pVertexData)
        , pIndexData(pIndexData)
        , indexSubIndex(indexSubIndex)
    {

    }

    size_t PipelineCache::VertexInputBindingDescriptionHash::operator()(const vk::VertexInputBindingDescription & vertexInputBindingDes) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, vertexInputBindingDes.binding);
        boost::hash_combine(seed, vertexInputBindingDes.inputRate);
        boost::hash_combine(seed, vertexInputBindingDes.stride);
        return seed;
    }

    size_t PipelineCache::VertexInputAttributeDescriptionHash::operator()(const vk::VertexInputAttributeDescription & vertexInputAttrDes) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, vertexInputAttrDes.binding);
        boost::hash_combine(seed, vertexInputAttrDes.format);
        boost::hash_combine(seed, vertexInputAttrDes.location);
        boost::hash_combine(seed, vertexInputAttrDes.offset);
        return seed;
    }

    size_t PipelineCache::PipelineVertexInputStateCreateInfoHash::operator()(const vk::PipelineVertexInputStateCreateInfo & pipelineVertexInputStateCreateInfo) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, VkPipelineVertexInputStateCreateFlags(pipelineVertexInputStateCreateInfo.flags));
        boost::hash_combine(seed, pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount);
        boost::hash_combine(seed, pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount);
        VertexInputBindingDescriptionHash vertexInputBindingDescriptionHash;
        for (uint32_t i = 0; i < pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount; ++i)
        {
            boost::hash_combine(seed, vertexInputBindingDescriptionHash(*(pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions + i)));
        }
        VertexInputAttributeDescriptionHash vertexInputAttributeDescriptionHash;
        for (uint32_t i = 0; i < pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount; ++i)
        {
            boost::hash_combine(seed, vertexInputAttributeDescriptionHash(*(pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions + i)));
        }
        return seed;
    }

    size_t PipelineCache::PipelineInputAssemblyStateCreateInfoHash::operator()(const vk::PipelineInputAssemblyStateCreateInfo & pipelineInputAssemblyStateCreateInfo) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, VkPipelineInputAssemblyStateCreateFlags(pipelineInputAssemblyStateCreateInfo.flags));
        boost::hash_combine(seed, pipelineInputAssemblyStateCreateInfo.topology);
        boost::hash_combine(seed, pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable);
        return seed;
    }

    size_t PipelineCache::HashNoState::operator()(const Info& info) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, info.renderPass);
        boost::hash_combine(seed, info.pPass->getID());
        boost::hash_combine(seed, info.pVertexData != nullptr ? info.pVertexData->getID() : 0);
        boost::hash_combine(seed, info.pIndexData != nullptr ? info.pIndexData->getID() : 0);

        return seed;
	}

    size_t PipelineCache::HashFull::operator()(const Info& info) const {
        std::size_t seed = HashNoState()(info);
        boost::hash_combine(seed, info.pPass->getPipelineStateID());
        boost::hash_combine(seed, info.pPass->getSubpass());
        if (info.pIndexData != nullptr) {
            const auto & subIndexDatas = info.pIndexData->getSubIndexDatas();
            const auto & subIndexData = subIndexDatas[info.indexSubIndex];
            boost::hash_combine(seed, PipelineInputAssemblyStateCreateInfoHash()(subIndexData.inputAssemblyStateInfo));

            if (info.pVertexData != nullptr) {
                const auto & subVertexDatas = info.pVertexData->getSubVertexDatas();
                const auto & subVertexData = subVertexDatas[subIndexData.vertexDataIndex];
                boost::hash_combine(seed, PipelineVertexInputStateCreateInfoHash()(subVertexData.vertexInputStateInfo));
            }
        }
        return seed;
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

    void PipelineCache::start()
    {
        m_mapPipelineFullBack = m_mapPipelineFull;
        m_mapPipelineFull.clear();
    }

    std::shared_ptr<vk::Pipeline> PipelineCache::caching(const Info &info)
    {
        //delete old pipeline.
        auto pOldPipeline1 = m_mapPipelineNoState[info];

        auto pOldPipeline2 = m_mapPipelineFullBack[info];

        std::shared_ptr<vk::Pipeline> pPipeline;
        if (pOldPipeline1 != nullptr) 
        {
            if (pOldPipeline2 != nullptr) //Pipeline don't change.
            {
                m_mapPipelineFull[info] = pOldPipeline2;
                m_mapPipelineFullBack.erase(info);
                pPipeline = pOldPipeline2;
            } 
			else if (m_mapPipelineFull[info] != nullptr) //Pipeline don't change.
			{
				pPipeline = m_mapPipelineFull[info];
			}
			else
            {
				
                //has old pipeline. delete it from two map.
                m_mapPipelineNoState.erase(info);
                //m_mapPipelineFullBack.erase(info);
                pPipeline = _createNewPipeline(info);
                m_mapPipelineNoState[info] = pPipeline;
                m_mapPipelineFull[info] = pPipeline;
            }
        }
        else //Old pipeline don't exist.
        {
            pPipeline = _createNewPipeline(info);
            m_mapPipelineNoState[info] = pPipeline;
            m_mapPipelineFull[info] = pPipeline;
        }

        return pPipeline;
    }

    void PipelineCache::end()
    {
        //Delete unuseful pipelines.
        m_mapPipelineFullBack.clear();
    }

    std::shared_ptr<vk::Pipeline> PipelineCache::_createNewPipeline(const Info &info)
    {
        //Create graphics pipeline create info. 
		vk::GraphicsPipelineCreateInfo createInfo = {};

		//Construct shader stage create info.
        const auto &pPass = info.pPass;
		const auto &pShader = pPass->getShader();
		auto &shaderStages = pShader->getShaderStageInfos();

        //Fill specialization data from pass.
        for (auto &shaderStage : shaderStages)
        {
            if (pPass->IsHasSpecializationData(shaderStage.stage)) 
            {
                shaderStage.pSpecializationInfo = &(pPass->getSpecializationData(shaderStage.stage)->getInfo());
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

        auto polygonMode = tranPolygonModeToVK(pPass->getPolygonMode());
        auto cullMode = tranCullModeFlagsToVK(pPass->getCullMode());
		auto frontFace = tranFrontFaceTypeToVK(pPass->getFrontFace());
        auto lineWidth = pPass->getLineWidth();
		//Rasterization info.
		vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
			vk::PipelineRasterizationStateCreateFlags(),  //flags
			VK_FALSE,                                     //depthClampEnable
			VK_FALSE,                                     //rasterizerDiscardEnable
			polygonMode,                                  //polygonMode
			cullMode,                                     //cullMode
			frontFace,                                    //frontFace
			VK_FALSE,                                     //depthBiasEnable
			0.0f,                                         //depthBiasConstantFactor
			0.0f,                                         //depthBiasClamp
			0.0f,                                         //depthBiasSlopeFactor
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

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
			vk::PipelineDynamicStateCreateFlags(),
            dynamicStates.size(),
			dynamicStates.data()
		};
		createInfo.pDynamicState = &dynamicStateCreateInfo;		

		createInfo.layout = *pPass->getPipelineLayout();

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