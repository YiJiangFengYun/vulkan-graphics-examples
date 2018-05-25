#include "vgim/module.hpp"

namespace vgim
{
    uint32_t m_canvasWidth;
    uint32_t m_canvasHeight;

    double m_time;
	
    std::shared_ptr<vg::Texture2D> m_pFontTexture;
    std::shared_ptr<vg::Material> m_pMaterial;
    
    std::shared_ptr<vg::DimSimpleMesh2> m_pMesh;

    std::shared_ptr<vg::VisualObject2> m_pUIObject;

    std::shared_ptr<vg::CameraOP2> m_pCamera;
    std::shared_ptr<vg::Scene2> m_pScene;

    Bool32 inited;
    Bool32 isCreatedCanvas;

    Bool32 getInited()
    {
        return inited;
    }

    uint32_t getCanvasWidth()
    {
        return m_canvasWidth;
    }

    uint32_t getCanvasHeight()
    {
        return m_canvasHeight;
    }

    vg::Material *getMaterial()
	{
		return m_pMaterial.get();
	}

    vg::DimSimpleMesh2 *getMesh()
    {
        return m_pMesh.get();   
    }

    vg::VisualObject2 *getUIObject()
    {
        return m_pUIObject.get();
    }

    vg::CameraOP2 *getCamera()
    {
        return m_pCamera.get();
    }

    vg::Scene2 *getScene()
    {
        return m_pScene.get();
    }

	void setShaderPath(const std::string &vertShaderPath, const std::string &fragShaderPath)
	{
		m_pMaterial->getMainShader()->load(vertShaderPath, fragShaderPath);
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
    void _initImGui();
    void _createFontTexture();
    void _destroyFontTexture();

	void moduleCreate(plog::Severity severity, plog::IAppender *appender)
	{
		if (inited == VG_IM_TRUE) return;
        plog::IAppender *callerAppender = appender;
		if (appender == nullptr) {
		    //init default log to write to the windows debug output
			static plog::DebugOutputAppender<plog::TxtFormatter> debugOutputAppender;
			appender = &debugOutputAppender;
		}
		plog::init<VGIM_PLOG_ID>(severity, appender);
        vg::moduleCreate(severity, callerAppender);
		//Indicate module was initialized.
		inited = VG_IM_TRUE;
	}

	void moduleDestory()
	{
		inited = VG_IM_FALSE;
        isCreatedCanvas = VG_IM_FALSE;
        _destroyFontTexture();
        _destroyScene();
        _destroyCamera();      
        _destroyUIObject();
        _destroyMesh();
        _destroyMaterial();
	}

    void moduleCreateCanvas(uint32_t canvasWidth, uint32_t canvasHeight)
    {
        if (isCreatedCanvas == VG_IM_TRUE) return;
        m_canvasWidth = canvasWidth;
        m_canvasHeight = canvasHeight;
        m_time = 0.0;
		_createMaterial();
        _createMesh();
        _createUIObject();
        _createCamera();
        _createScene();
        _initImGui();
        _createFontTexture();
        isCreatedCanvas = VG_IM_TRUE;
    }

    void setLogSeverity(plog::Severity severity)
	{
		plog::get<VGIM_PLOG_ID>()->setMaxSeverity(severity);
	}

    void updateIMGUI(uint32_t canvasWidth
        , uint32_t canvasHeight
        , double currTime
        , float mouseX
        , float mouseY
        , Bool32 mouseDown[3]
        )
    {
        m_canvasWidth = canvasWidth;
        m_canvasHeight = canvasHeight;

        // Dimensions
        ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(canvasWidth), static_cast<float>(canvasHeight));
		io.FontGlobalScale = 1.0f;

        //camera
        auto &pCamera = m_pCamera;
		pCamera->updateProj(fd::Bounds<glm::vec2>(glm::vec2(0.0f, 0.0f), 
		    glm::vec2(static_cast<float>(canvasWidth), static_cast<float>(canvasHeight))));

        //time
        io.DeltaTime = static_cast<float>(currTime - m_time);
        m_time = currTime;

        //input
        io.MousePos = ImVec2(mouseX, mouseY);

        for (uint32_t i = 0; i < 3u; ++i) {
            io.MouseDown[i] = mouseDown[i];            
        }

        // Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
        ImGui::NewFrame();
    }

    void updateImGUIRender()
    {
        ImGui::Render();        
        //update mesh
        auto drawData = ImGui::GetDrawData();
		if (drawData == nullptr) return;
		uint32_t vertexCount = drawData != nullptr ? drawData->TotalVtxCount : 0u;
        uint32_t indexCount = drawData != nullptr ? drawData->TotalIdxCount : 0u;
        uint32_t vertexSize = static_cast<uint32_t>(vertexCount * sizeof(ImDrawVert));
        uint32_t indexSize = static_cast<uint32_t>(indexCount * sizeof(ImDrawIdx));

        auto pUIObject = m_pUIObject;
        const auto& pVertexData = m_pMesh->getVertexData();
        const auto& pIndexData = m_pMesh->getIndexData();

		uint32_t cmdListCount = drawData != nullptr ? drawData->CmdListsCount : 0u;

        std::vector<vg::MemorySlice> vertexSlices(cmdListCount);
        std::vector<vg::MemorySlice> indexSlices(cmdListCount);

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
        uint32_t vertexOffset = 0u;
        uint32_t indexOffset = 0u;
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
            indexOffset += indexSlices[i].size;

            vertexCounts[vertexSubDataCount] = static_cast<uint32_t>(cmdList->VtxBuffer.Size);
            vertexBufferSizes[vertexSubDataCount] = vertexCounts[vertexSubDataCount] * static_cast<uint32_t>(sizeof(ImDrawVert));

            uint32_t cmdCount = cmdList->CmdBuffer.Size;
            for (uint32_t cmdI = 0; cmdI < cmdCount; ++cmdI)
            {
                const ImDrawCmd* pCmd = &cmdList->CmdBuffer[cmdI];

                indexCounts[indexSubDataCount] = pCmd->ElemCount;
                indexBufferSizes[indexSubDataCount] = pCmd->ElemCount * static_cast<uint32_t>(sizeof(ImDrawIdx));
                indexVertexDataIndices[indexSubDataCount] = vertexSubDataCount;

                //Calculate clip rectangle that is in valid range of ClipRect is [(0, 0), (1, 1)]
                fd::Rect2D rect;
                rect.x = std::max(pCmd->ClipRect.x, 0.0f) / m_canvasWidth;
                rect.y = std::max(pCmd->ClipRect.y, 0.0f) / m_canvasHeight;
                rect.width = (pCmd->ClipRect.z - pCmd->ClipRect.x) / m_canvasWidth;
                rect.height = (pCmd->ClipRect.w - pCmd->ClipRect.y) / m_canvasHeight;
                indexRects[indexSubDataCount] = rect;
                ++indexSubDataCount;
            }
            ++vertexSubDataCount;
        }

        pVertexData->updateBuffer(vertexSlices, vertexSize, VG_FALSE);
        pIndexData->updateBuffer(indexSlices, indexSize, VG_FALSE);

        pVertexData->updateSubDataCount(vertexSubDataCount);
		if (vertexSubDataCount)
		{
			const auto &firstSubVertexData = pVertexData->getSubVertexDatas()[0];
			pVertexData->updateDesData(firstSubVertexData.vertexInputStateInfo);
			pVertexData->updateVertexCount(vertexCounts);
			pVertexData->updateBufferSize(vertexBufferSizes);
		}

        pIndexData->updateSubDataCount(indexSubDataCount);
		if (indexSubDataCount)
		{
			const auto &firstSubIndexData = pIndexData->getSubIndexDatas()[0];
			pIndexData->updateDesData(firstSubIndexData.indexType, firstSubIndexData.inputAssemblyStateInfo);
			pIndexData->updateIndexCount(indexCounts);
			pIndexData->updateBufferSize(indexBufferSizes);
			pIndexData->updateVertexDataIndex(indexVertexDataIndices);

            pUIObject->setHasClipRect(VG_TRUE);
            pUIObject->updateClipRects(indexRects);
		}


        //update material
        // ImGuiIO& io = ImGui::GetIO();
        // float data[4];
        // data[0] = 2.0f / io.DisplaySize.x;
        // data[1] = 2.0f / io.DisplaySize.y;
        // data[2] = -1.0f;
        // data[3] = -1.0f;
        // m_pPass->setPushConstantUpdate("default", &data, static_cast<uint32_t>(sizeof(data)), 
        //     vk::ShaderStageFlagBits::eVertex, 0u);

		auto pPass = m_pMaterial->getMainPass();

        fd::Viewport viewPort(0.0f, 0.0f, ImGui::GetIO().DisplaySize.x / static_cast<float>(m_canvasWidth), 
            ImGui::GetIO().DisplaySize.y / static_cast<float>(m_canvasHeight), 0.0f, 1.0f);
		pPass->setViewport(viewPort);

        fd::Rect2D scissor(0.0f, 0.0f, ImGui::GetIO().DisplaySize.x / static_cast<float>(m_canvasWidth), 
            ImGui::GetIO().DisplaySize.y / static_cast<float>(m_canvasHeight));
		pPass->setScissor(scissor);

		m_pUIObject->setMaterialCount(m_pMesh->getSubMeshCount());

		m_pUIObject->setMaterial(m_pMaterial.get());
		
    }

	void _createMaterial()
	{
		m_pMaterial = std::shared_ptr<vg::Material>(new vg::Material());
		m_pMaterial->setRenderPriority(0u);
		m_pMaterial->setRenderQueueType(vg::MaterialShowType::TRANSPARENT);

		auto pPass = m_pMaterial->getMainPass();
		pPass->setFrontFace(vk::FrontFace::eClockwise);
        //push constant
        // pPass->setPushConstantRange("default", vk::ShaderStageFlagBits::eVertex, static_cast<uint32_t>(sizeof(float) * 0), static_cast<uint32_t>(sizeof(float) * 4));

        //color blend
        uint32_t attachmentCount = 1u;
        std::vector<vk::PipelineColorBlendAttachmentState> blendAttachmentStates(attachmentCount);
        for (uint32_t i = 0; i < attachmentCount; ++i)
        {
            auto &blendAttachmentState = blendAttachmentStates[i];
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
		pPass->setColorBlendInfo(colorBlendInfo);

        //depth stencil
        vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;
        depthStencilInfo.depthTestEnable = VK_FALSE;
        depthStencilInfo.depthWriteEnable = VK_FALSE;
		pPass->setDepthStencilInfo(depthStencilInfo);

		
		m_pMaterial->apply();
	}

    void _destroyMaterial()
    {
        m_pMaterial = nullptr;
    }

    void _createMesh()
    {
       m_pMesh = static_cast<std::shared_ptr<vg::DimSimpleMesh2>>(new vg::DimSimpleMesh2(vk::MemoryPropertyFlagBits::eHostVisible));
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
        m_pUIObject->setMesh(m_pMesh.get());
		m_pUIObject->setMaterialCount(1u);
        m_pUIObject->setMaterial(m_pMaterial.get());
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
        m_pScene->setIsRightHand(VG_TRUE);
        m_pScene->addCamera(m_pCamera.get());
        m_pScene->addVisualObject(m_pUIObject.get());
    }

    void _destroyScene()
    {
        m_pScene = nullptr;
    }

    void _initImGui()
    {
        ImGui::CreateContext();

        //style
        ImGui::StyleColorsDark();
        // // Color scheme
		// ImGuiStyle& style = ImGui::GetStyle();
		// style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		// style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		// style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.0f, 0.0f, 0.1f);
		// style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
		// style.Colors[ImGuiCol_Header] = ImVec4(0.8f, 0.0f, 0.0f, 0.4f);
		// style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
		// style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
		// style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    }

    void _createFontTexture()
    {
        ImGuiIO& io = ImGui::GetIO();

        unsigned char* pixels;
        int32_t width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        uint32_t size = static_cast<uint32_t>(width * height * 4 * sizeof(char));

        m_pFontTexture = std::shared_ptr<vg::Texture2D>(new vg::Texture2D(vk::Format::eR8G8B8A8Unorm, 
            VG_FALSE, width, height));
		vg::TextureDataInfo layoutInfo;
		vg::TextureDataInfo::Component component;
		component.mipLevel = 0u;
		component.layerCount = 1u;
		component.baseArrayLayer = 0u;
		component.size = size;
		layoutInfo.componentCount = 1u;
		layoutInfo.pComponent = &component;
		m_pFontTexture->applyData(layoutInfo, reinterpret_cast<void *>(pixels), size);
        
        io.Fonts->TexID = reinterpret_cast<void *>(VkImage(*(m_pFontTexture->getImage()->getImage())));

		auto pPass = m_pMaterial->getMainPass();

		//main texture
		std::string name = "main_texture";
		vg::PassTextureInfo info = {
			m_pFontTexture.get(),
			nullptr,
			nullptr,
			vk::ImageLayout::eUndefined,
			VG_PASS_OTHER_MIN_BINDING_PRIORITY,
			vg::ImageDescriptorType::COMBINED_IMAGE_SAMPLER,
			vk::ShaderStageFlagBits::eFragment,
		};
		if (pPass->hasTexture(name) == VG_FALSE)
		{
			pPass->addTexture(name, info);
		}
		else
		{
			pPass->setTexture(name, info);
		}
    }

    void _destroyFontTexture()
    {
		auto pPass = m_pMaterial->getMainPass();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->TexID = reinterpret_cast<void *>(0);
		std::string name = "main_texture";
		pPass->removeTexture(name);
        m_pFontTexture = nullptr;
    }  
} //vgim