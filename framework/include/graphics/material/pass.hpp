#ifndef KGS_PASS_H
#define KGS_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/util/util.hpp"

#define MAIN_TEXTURE_NAME "_MainTex"
#define MAIN_COLOR_NAME "_Color"
#define MAIN_TEXTURE_OFFSET_NAME "_MainTexOffset"
#define MAIN_TEXTURE_SCALE_NAME "_MainTexScale"

#define MAIN_TEXTURE_BINDING 0
#define MAIN_COLOR_BINDING 1
#define MAIN_TEXTURE_OFFSET_BINDING 2
#define MAIN_TEXTURE_SCALE_BINDING 3
#define OTHER_MIN_BINDING 4

namespace kgs
{
	class Pass
	{
	public:
		struct LayoutBindingInfo
		{
			std::string name;
			MaterialData::DataType dataType;
			std::uint32_t binding;
			DescriptorType descriptorType;
			std::uint32_t descriptorCount;
			ShaderStageFlags stageFlags;

			LayoutBindingInfo(std::string name,
				MaterialData::DataType dataType,
				std::uint32_t binding,
				DescriptorType descriptorType,
				std::uint32_t descriptorCount,
				ShaderStageFlags stageFlags):
				name(name),
				dataType(dataType),
				binding(binding),
				descriptorType(descriptorType),
				descriptorCount(descriptorCount),
				stageFlags(stageFlags)
			{
			
			}

			Bool32 operator ==(const LayoutBindingInfo& target) const
			{
				return name == target.name && dataType == target.dataType && binding == target.binding &&
					descriptorType == target.descriptorType && descriptorCount == target.descriptorCount &&
					stageFlags == target.stageFlags;
			}
		};

		Pass();
		~Pass();

		template <MaterialData::DataType dataType>
		typename MaterialData::ValueTypeInfo<dataType>::value_t getData(std::string name)
		{
			return m_pData->getDataValue<dataType>(name);
		}

		template <MaterialData::DataType dataType>
		void setData(std::string name, 
			typename MaterialData::ValueTypeInfo<dataType>::value_t value, 
			uint32_t binding = OTHER_MIN_BINDING, 
			DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER, 
			ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX)
		{
			m_pData->setDataValue<dataType>(name, value);
			//update layout binding information.
			uint32_t descriptorCount = static_cast<uint32_t>(sizeof(value) / sizeof(typename MaterialData::ValueTypeInfo<dataType>::base_t));
			LayoutBindingInfo info(
				name,
				dataType,
				binding,
				descriptorType,
				descriptorCount,
				stageFlags
			);
			setValue(name, info, m_mapLayoutBinds, m_arrLayoutBinds);
		}

		std::shared_ptr<Texture> getMainTexture();
		void setMainTexture(std::shared_ptr<Texture> value);
		Vector2 getMainTextureOffset();
		void setMainTextureOffset(Vector2 value);
		Vector2 getMainTextureScale();
		void setMainTextureScale(Vector2 value);

		Color getMainColor();
		void setMainColor(Color value);

		void apply();
	private:
		//compositons
		std::shared_ptr<MaterialData> m_pData;
		std::vector<LayoutBindingInfo> m_arrLayoutBinds;
		std::unordered_map<std::string, LayoutBindingInfo> m_mapLayoutBinds;
		std::shared_ptr<vk::Buffer> m_pUniformBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pUniformBufferMemory;
		std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
		std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;

		//aggregations
		std::shared_ptr<Context> m_pContext;
		std::shared_ptr<Shader> m_pShader;
		void _createDescriptorSetLayout();
		void _createUniformBuffer();
		void _createDescriptorSet();
		void _updateDescriptorBufferInfo();
		void _updateDescriptorImageInfo();
		void _applyBufferContent();

		//tool methods
		void createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer>& pBuffer, std::shared_ptr<vk::DeviceMemory> pBufferMemory);
	};
}

#endif // !KGS_PASS_H
