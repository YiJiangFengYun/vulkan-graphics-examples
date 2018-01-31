#include "graphics/renderer/pipeline_cache.hpp"

namespace vg
{
    PipelineCache::Info::Info()
        : renderPass()
        , pPass()
        , pVertexData()
        , vertexSubIndex()
        , pIndexData()
        , indexSubIndex()
    {

    }

    PipelineCache::Info::Info(const Info &target)
        : renderPass(target.renderPass)
        , pPass(target.pPass)
        , pVertexData(target.pVertexData)
        , vertexSubIndex(target.vertexSubIndex)
        , pIndexData(target.pIndexData)
        , indexSubIndex(target.indexSubIndex) 
    {
        
    }
			
    PipelineCache::Info::Info(const Info &&target)
        : renderPass(std::move(target.renderPass))
        , pPass(std::move(target.pPass))
        , pVertexData(std::move(target.pVertexData))
        , vertexSubIndex(std::move(target.vertexSubIndex))
        , pIndexData(std::move(target.pIndexData))
        , indexSubIndex(std::move(target.indexSubIndex))
    {

    }

	PipelineCache::Info& PipelineCache::Info::operator=(const Info &target)
    {
        renderPass = target.renderPass;
        pPass = target.pPass;
        pVertexData = target.pVertexData;
        vertexSubIndex = target.vertexSubIndex;
        pIndexData = target.pIndexData;
        indexSubIndex = target.indexSubIndex;
        return *this;
    }

    bool PipelineCache::Info::operator==(const Info &rhs) const
    {
        return renderPass == rhs.renderPass &&
            pPass->getID() == rhs.pPass->getID() &&
            pVertexData->getID() == rhs.pVertexData->getID() &&
            vertexSubIndex ==  rhs.vertexSubIndex &&
            pIndexData->getID() == rhs.pIndexData->getID() &&
            indexSubIndex == rhs.indexSubIndex;
    }

	bool PipelineCache::Info::operator!=(const Info &rhs) const
    {
        return !operator==(rhs);
    }

    PipelineCache::Info::Info(vk::RenderPass renderPass
        , std::shared_ptr<Pass> pPass
        , std::shared_ptr<VertexData> pVertexData
        , uint32_t vertexSubIndex
        , std::shared_ptr<IndexData> pIndexData
        , uint32_t indexSubIndex
        )
        : renderPass(renderPass)
        , pPass(pPass)
        , pVertexData(pVertexData)
        , vertexSubIndex(vertexSubIndex)
        , pIndexData(pIndexData)
        , indexSubIndex(indexSubIndex)
    {

    }

    size_t PipelineCache::HashNoState::operator()(const Info& info) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, info.renderPass);
        boost::hash_combine(seed, info.pPass->getID());
        boost::hash_combine(seed, info.pVertexData->getID());
        boost::hash_combine(seed, info.vertexSubIndex);
        boost::hash_combine(seed, info.pIndexData->getID());
        boost::hash_combine(seed, info.indexSubIndex);
        return seed;
	}

    size_t PipelineCache::HashFull::operator()(const Info& info) const {
        std::size_t seed = HashNoState()(info);
        boost::hash_combine(seed, info.pPass->getPipelineStateID());
        boost::hash_combine(seed, info.pVertexData->getPipelineStateID());
        boost::hash_combine(seed, info.pIndexData->getPipelineStateID());
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
            else 
            {
                //has old pipeline. delete it from two map.
                m_mapPipelineNoState.erase(info);
                m_mapPipelineFullBack.erase(info);
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

        const auto &pVertexData = info.pVertexData;
		const VertexData::SubVertexData &subVertexData = pVertexData->getSubVertexDatas()[info.vertexSubIndex];
        const auto &pIndexData = info.pIndexData;
		const IndexData::SubIndexData &subIndexData = pIndexData->getSubIndexDatas()[info.indexSubIndex];

		createInfo.pVertexInputState = &subVertexData.vertexInputStateInfo;
		createInfo.pInputAssemblyState = &subIndexData.inputAssemblyStateInfo;

        auto cullMode = tranCullModeFlagsToVK(pPass->getCullMode());
		auto frontFace = tranFrontFaceTypeToVK(pPass->getFrontFace());
		//Rasterization info.
		vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
			vk::PipelineRasterizationStateCreateFlags(),  //flags
			VK_FALSE,                                     //depthClampEnable
			VK_FALSE,                                     //rasterizerDiscardEnable
			vk::PolygonMode::eFill,                       //polygonMode
			cullMode,                                     //cullMode
			frontFace,                                    //frontFace
			VK_FALSE,                                     //depthBiasEnable
			0.0f,                                         //depthBiasConstantFactor
			0.0f,                                         //depthBiasClamp
			0.0f,                                         //depthBiasSlopeFactor
			1.0f                                          //lineWidth
		};
		createInfo.pRasterizationState = &rasterizationStateCreateInfo;

		auto depthStencilStateInfoOfPass = pPass->getDepthStencilStateInfo();

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

		uint32_t attachmentCount = 1u;
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
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
			vk::PipelineDynamicStateCreateFlags(),
            dynamicStates.size(),
			dynamicStates.data()
		};
		createInfo.pDynamicState = &dynamicStateCreateInfo;		

		createInfo.layout = *pPass->getPipelineLayout();

		createInfo.renderPass = info.renderPass;
		createInfo.subpass = 0;
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