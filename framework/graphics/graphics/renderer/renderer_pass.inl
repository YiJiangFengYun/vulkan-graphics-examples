namespace vg
{
    template <typename T>
    void RendererPass::_updateBuildInData(Pass::BuildInDataType type, T data)
    {
        Pass::BuildInDataInfo buildInDataInfo;
        if (m_pPass != nullptr) buildInDataInfo = m_pPass->getBuildInDataInfo();
        uint32_t count = static_cast<uint32_t>(BuildInDataType::COUNT);
        uint32_t offset1 = 0u;
        uint32_t i = 0u;
        for (i = 0; i < count; ++i)
        {
            if (i != static_cast<uint32_t>(type))
            {
                offset1 += Pass::buildInDataTypeSizes[i];
            }
            else
            {
                break;
            }
        }

        uint32_t offset2 = 0u;
        uint32_t componentCount = buildInDataInfo.componentCount;
        for (uint32_t componentIndex = 0u; componentIndex < componentCount; ++componentIndex)
        {
            Pass::BuildInDataType comType = (*(buildInDataInfo.pComponent + componentIndex)).type;
            if (type != comType)
            {
                offset2 += Pass::buildInDataTypeSizes[static_cast<uint32_t>(comType)];
            }
            else
            {
                break;
            }
        }

        if ( i < count)
        {
            size_t size = sizeof(T);
            if (memcmp((char *)(&m_buildInDataCache) + offset1, &data, size) != 0) {
                memcpy((char *)(&m_buildInDataCache) + offset1, &data, size);
                m_bindingSet.setData(VG_PASS_BUILDIN_DATA_NAME
                    , &data
                    , size
                    , offset2);
            }
        }
    }
} //vg