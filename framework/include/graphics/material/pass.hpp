#ifndef KGS_PASS_H
#define KGS_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/util/util.hpp"

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

		void _setMaterialData(std::shared_ptr<MaterialData> pMaterialData);
	protected:
		void virtual _createBindLayout() = 0;
	private:
		//compositons
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
