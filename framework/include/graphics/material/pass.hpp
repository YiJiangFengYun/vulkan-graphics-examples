#ifndef KGS_PASS_H
#define KGS_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/util/util.hpp"

#define MainColorName "_Color"
#define MainTextureName "_MainTex"

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
		};

		Pass();
		Pass(std::shared_ptr<MaterialData> pMaterialData);
		~Pass();

		template <MaterialData::DataType dataType>
		typename MaterialData::ValueTypeInfo<dataType>::value_t getData(std::string name)
		{
			return m_pData->getDataValue<dataType>(name);
		}

		template <MaterialData::DataType dataType>
		void setData(std::string name, typename MaterialData::ValueTypeInfo<dataType>::value_t value)
		{
			m_pData->setDataValue<dataType>(name, value);
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

		void _setMaterialData(std::shared_ptr<MaterialData> pMaterialData);
	protected:
		void virtual _createBindLayout() = 0;
	private:
		//compositons
		std::shared_ptr<MaterialData> m_pData;
		std::vector<LayoutBindingInfo> m_binds;
		std::shared_ptr<vk::Buffer> m_pUniformBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pUniformBufferMemory;
		std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
		std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;

		//aggregations
		std::shared_ptr<Context> m_pContext;
		std::shared_ptr<Shader> m_pShader;
		std::shared_ptr<MaterialData> m_pMaterialData;
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
