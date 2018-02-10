#include "vgim/module.hpp"

namespace vgim
{
	static const uint32_t __glsl_shader_vert_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
        0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
        0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
        0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
        0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
        0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
        0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
        0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
        0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
        0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
        0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
        0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
        0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
        0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
        0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
        0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
        0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
        0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
        0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
        0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
        0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
        0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
        0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
        0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
        0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
        0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
        0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
        0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
        0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
        0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
        0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
        0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
        0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
        0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
        0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
        0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
        0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
        0x0000002d,0x0000002c,0x000100fd,0x00010038
    };
    
    static const uint32_t __glsl_shader_frag_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
        0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
        0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
        0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
        0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
        0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
        0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
        0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
        0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
        0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
        0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
        0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
        0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
        0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
        0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
        0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
        0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
        0x00010038
    };
	
    std::shared_ptr<vg::Shader> m_pShader;
    std::shared_ptr<vg::Pass> m_pPass;
    std::shared_ptr<vg::Material> m_pMaterial;
    
    std::shared_ptr<vg::DimSimpleMesh2> m_pMesh;

    std::shared_ptr<vg::VisualObject2> m_pUIObject;

    std::shared_ptr<vg::CameraOP2> m_pCamera;
    std::shared_ptr<vg::Scene2> m_pScene;

    const std::shared_ptr<vg::Material> getMaterial()
	{
		return m_pMaterial;
	}

    const std::shared_ptr<vg::SimpleMesh> getMesh()
    {
        return m_pMesh;   
    }

    const std::shared_ptr<vg::VisualObject2> getUIObject()
    {
        return m_pUIObject;
    }

    const std::shared_ptr<vg::CameraOP2> getCamera()
    {
        return m_pCamera;
    }

    const std::shared_ptr<vg::Scene2> getScene()
    {
        return m_pScene;
    }

    void _createMaterial();
    void _destroyMaterial();
    void _createMesh();
    void _destroyMesh();
    void _createUIObject();
    void _destroyUIObject();
    void _createCamera();
    void _destroyCamera();
    void _createScene();
    void _destroyScene();

	void static moduleCreate()
	{
		if (isInited == VG_TRUE) return;
		_createMaterial();
        _createMesh();
        _createUIObject();
        _createCamera();
        _createScene();
        
		//Indicate module was initialized.
		isInited = VG_IM_TRUE;
	}

	void moduleDestory()
	{
		isInited = VG_IM_FALSE;

        _destroyScene();
        _destroyCamera();      
        _destroyUIObject();
        _destroyMesh();
        _destroyMaterial();
	}

    void updateFromImGUI()
    {
        //update mesh
        auto drawData = ImGui::GetDrawData();
		uint32_t vertexCount = drawData->TotalVtxCount;
        uint32_t indexCount = drawData->TotalIdxCount;
        uint32_t vertexSize = static_cast<uint32_t>(vertexCount * sizeof(ImDrawVert));
        uint32_t indexSize = static_cast<uint32_t>(indexCount * sizeof(ImDrawIdx));

        const auto& pVertexData = m_pMesh->getVertexData();
        const auto& pIndexData = m_pMesh->getIndexData();

        std::vector<vg::MemorySlice> vertexSlices(drawData->CmdListsCount);
        std::vector<vg::MemorySlice> indexSlices(drawData->CmdListsCount);

        uint32_t vertexOffset = 0u;
        uint32_t indexOffset = 0u;
        uint32_t cmdListCount = drawData->CmdListsCount;
        uint32_t vertexSubDataCount = cmdListCount;
        uint32_t indexSubDataCount = 0u;                
        for (uint32_t i = 0; i < cmdListCount; ++i)
        {
             const ImDrawList* cmdList = drawData->CmdLists[i];
             indexSubDataCount += static_cast<uint32_t>(cmdList->CmdBuffer.Size);
        }

        std::vector<uint32_t> vertexCounts(vertexSubDataCount);
        std::vector<uint32_t> vertexBufferSizes(vertexSubDataCount);
        std::vector<uint32_t> indexCounts(indexSubDataCount);
        std::vector<uint32_t> indexBufferSizes(indexSubDataCount);
        std::vector<uint32_t> indexVertexDataIndices(indexSubDataCount);
        std::vector<fd::Rect2D> indexRects(indexSubDataCount);
        vertexSubDataCount = 0u;
        indexSubDataCount = 0u;
        for (uint32_t i = 0; i < cmdListCount; ++i)
        {
            const ImDrawList* cmdList = drawData->CmdLists[i];
            vertexSlices[i].offset = vertexOffset;
            vertexSlices[i].size = static_cast<uint32_t>(cmdList->VtxBuffer.Size) * static_cast<uint32_t>(sizeof(ImDrawVert));
            vertexSlices[i].pMemory = cmdList->VtxBuffer.Data;

            indexSlices[i].offset = indexOffset;
            indexSlices[i].size = static_cast<uint32_t>(cmdList->IdxBuffer.Size) * static_cast<uint32_t>(sizeof(ImDrawIdx));
            indexSlices[i].pMemory = cmdList->IdxBuffer.Data;

            vertexOffset += vertexSlices[i].size;
            indexOffset += vertexSlices[i].size;

            vertexCounts[vertexSubDataCount] = static_cast<uint32_t>(cmdList->VtxBuffer.Size);
            vertexBufferSizes[vertexSubDataCount] = vertexCounts[vertexSubDataCount] * static_cast<uint32_t>(sizeof(ImDrawVert));

            uint32_t cmdCount = cmdList->CmdBuffer.Size;
            for (uint32_t cmdI = 0; cmdI < cmdCount; ++cmdI)
            {
                const ImDrawCmd* pCmd = &cmdList->CmdBuffer[cmdI];

                indexCounts[indexSubDataCount] = pCmd->ElemCount;
                indexBufferSizes[indexSubDataCount] = pCmd->ElemCount * static_cast<uint32_t>(sizeof(ImDrawIdx));
                indexVertexDataIndices[indexSubDataCount] = vertexSubDataCount;

                fd::Rect2D rect;
                rect.x = std::max(pCmd->ClipRect.x, 0.0f);
                rect.y = std::max(pCmd->ClipRect.y, 0.0f);
                rect.width = pCmd->ClipRect.z - pCmd->ClipRect.x;
                rect.height = pCmd->ClipRect.w - pCmd->ClipRect.y;
                indexRects[indexSubDataCount] = rect;
                ++indexSubDataCount;
            }
            ++vertexSubDataCount;
        }

        pVertexData->updateBuffer(vertexSlices, vertexSize, VG_FALSE);
        pIndexData->updateBuffer(indexSlices, indexSize, VG_FALSE);

        pVertexData->updateSubDataCount(vertexSubDataCount);
        pVertexData->updateVertexCount(vertexCounts, vertexSubDataCount);
        pVertexData->updateBufferSize(vertexBufferSizes, vertexSubDataCount);

        pIndexData->updateSubDataCount(indexSubDataCount);
        pIndexData->updateIndexCount(indexCounts, indexSubDataCount);
        pIndexData->updateBufferSize(indexBufferSizes, indexSubDataCount);
        pIndexData->updateVertexDataIndex(indexVertexDataIndices, indexSubDataCount);
        pIndexData->updateClipRect(indexRects, indexSubDataCount);

        //update material
        ImGuiIO& io = ImGui::GetIO();
        float data[4];
        data[0] = 2.0f / io.DisplaySize.x;
        data[1] = 2.0f / io.DisplaySize.y;
        data[2] = -1.0f;
        data[3] = -1.0f;
        m_pPass->setPushConstantUpdate("default", &data, static_cast<uint32_t>(sizeof(data)), 
            vk::ShaderStageFlagBits::eVertex, 0u);
    }

	void _createMaterial()
	{
        //shader
        m_pShader = std::shared_ptr<vg::Shader>(new vg::Shader(__glsl_shader_vert_spv, 
            static_cast<uint32_t>(sizeof(__glsl_shader_vert_spv) / sizeof(uint32_t)),
            __glsl_shader_frag_spv,
            static_cast<uint32_t>(sizeof(__glsl_shader_frag_spv) / sizeof(uint32_t))));
		m_pPass = std::shared_ptr<vg::Pass>(new vg::Pass(m_pShader));

        //push constant
        m_pPass->setPushConstantRange("default", vk::ShaderStageFlagBits::eVertex, static_cast<uint32_t>(sizeof(float) * 0), static_cast<uint32_t>(sizeof(float) * 4));

        //color blend
        uint32_t attachmentCount = 1u;
        std::vector<vk::PipelineColorBlendAttachmentState> blendAttachmentStates(attachmentCount);
        for (uint32_t i = 0; i < attachmentCount; ++i)
        {
            auto blendAttachmentState = blendAttachmentStates[i];
            blendAttachmentState.blendEnable = VK_TRUE;
            blendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | 
                vk::ColorComponentFlagBits::eG | 
                vk::ColorComponentFlagBits::eB | 
                vk::ColorComponentFlagBits::eA;
            blendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
            blendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
            blendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
            blendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
            blendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eDstAlpha;
            blendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
        }

        vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
        colorBlendInfo.attachmentCount = blendAttachmentStates.size();
        colorBlendInfo.pAttachments = blendAttachmentStates.data();
        m_pPass->setColorBlendInfo(colorBlendInfo);

        //depth stencil
        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
        depthStencilInfo.depthTestEnable = VK_FALSE;
        depthStencilInfo.depthWriteEnable = VK_FALSE;
        m_pPass->setDepthStencilInfo(depthStencilInfo);

		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->addPass(m_pPass);
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::TRANSPARENT);
		m_pMaterial->apply();
	}

    void _destroyMaterial()
    {
        m_pMaterial = nullptr;
        m_pPass = nullptr;
        m_pShader = nullptr;
    }

    void _createMesh()
    {
       m_pMesh = static_cast<std::shared_ptr<vg::DimSimpleMesh2>>(new vg::DimSimpleMesh2(vg::MemoryPropertyFlagBits::HOST_VISIBLE));
       const auto& pVertexData = m_pMesh->getVertexData();
       const auto& pIndexData = m_pMesh->getIndexData();
       vk::VertexInputBindingDescription bindingDesc[1] = {};
       bindingDesc[0].stride = sizeof(ImDrawVert);
       bindingDesc[0].inputRate = vk::VertexInputRate::eVertex;

       vk::VertexInputAttributeDescription attributeDesc[3] = {};
       attributeDesc[0].location = 0;
       attributeDesc[0].binding = bindingDesc[0].binding;
       attributeDesc[0].format = vk::Format::eR32G32Sfloat;
       attributeDesc[0].offset = (size_t)(&((ImDrawVert*)0)->pos);
       attributeDesc[1].location = 1;
       attributeDesc[1].binding = bindingDesc[0].binding;
       attributeDesc[1].format = vk::Format::eR32G32Sfloat;
       attributeDesc[1].offset = (size_t)(&((ImDrawVert*)0)->uv);
       attributeDesc[2].location = 2;
       attributeDesc[2].binding = bindingDesc[0].binding;
       attributeDesc[2].format = vk::Format::eR8G8B8A8Unorm;
       attributeDesc[2].offset = (size_t)(&((ImDrawVert*)0)->col);

       vk::PipelineVertexInputStateCreateInfo vertexInfo = {};
       vertexInfo.vertexBindingDescriptionCount = 1;
       vertexInfo.pVertexBindingDescriptions = bindingDesc;
       vertexInfo.vertexAttributeDescriptionCount = 3;
       vertexInfo.pVertexAttributeDescriptions = attributeDesc;

       vk::PipelineInputAssemblyStateCreateInfo iaInfo = {};
       iaInfo.topology = vk::PrimitiveTopology::eTriangleList;
       pVertexData->updateDesData(vertexInfo);
       pIndexData->updateDesData(vk::IndexType::eUint16, iaInfo);
    }

    void _destroyMesh()
    {
        m_pMesh = nullptr;
    }

    void _createUIObject()
    {
        m_pUIObject = std::shared_ptr<vg::VisualObject2>(new vg::VisualObject2());
        m_pUIObject->setMesh(m_pMesh);
        m_pUIObject->setMaterial(m_pMaterial);
    }

    void _destroyUIObject()
    {
        m_pUIObject = nullptr;
    }

    void _createCamera()
    {
        m_pCamera = std::shared_ptr<vg::CameraOP2>(new vg::CameraOP2());
    }

    void _destroyCamera()
    {
        m_pCamera = nullptr;
    }

    void _createScene()
    {
        m_pScene = std::shared_ptr<vg::Scene2>(new vg::Scene2());
        m_pScene->addCamera(m_pCamera);
        m_pScene->addVisualObject(m_pUIObject);
    }

    void _destroyScene()
    {
        m_pScene = nullptr;
    }
} //vgim