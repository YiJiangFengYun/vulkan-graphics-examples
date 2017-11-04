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
	class Pass : public Base
	{
	public:
		struct LayoutBindingInfo
		{
			std::string name;
			Bool32 isTexture;
			std::uint32_t binding;
			DescriptorType descriptorType;
			std::uint32_t descriptorCount;
			ShaderStageFlags stageFlags;

			std::uint32_t size;
			std::uint32_t bufferSize;

			LayoutBindingInfo();

			LayoutBindingInfo(std::string name
				, Bool32 isTexture
				, std::uint32_t binding
				, DescriptorType descriptorType
				, std::uint32_t descriptorCount
				, ShaderStageFlags stageFlags
			);

			LayoutBindingInfo(const LayoutBindingInfo &);

			LayoutBindingInfo(const LayoutBindingInfo &&);

			LayoutBindingInfo& operator=(const LayoutBindingInfo &);

			Bool32 operator ==(const LayoutBindingInfo& target) const;

			void updateSize(const std::shared_ptr<MaterialData> &pMaterialData);
		};

		Pass();
		Pass(std::shared_ptr<Shader> pShader);
		~Pass();

		std::shared_ptr<Shader> getShader();
		void setShader(std::shared_ptr<Shader> pShader);

		const std::shared_ptr<Texture> &getTexture(std::string name) const;

		void setTexture(std::string name
			, const std::shared_ptr<Texture> &pTex
			, uint32_t binding = KGS_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
		);

		template <typename T>
		T getDataValue(std::string name) const;

		template <typename T>
		std::vector<T> getDataValue(std::string name, uint32_t count) const;

		template<typename T>
		void setDataValue(std::string name
			, const T &value
			, uint32_t binding = KGS_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
		);

		template<typename T>
		void setDataValue(std::string name
			, const std::vector<T> &values
			, uint32_t binding = KGS_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
		);

		const std::shared_ptr<Texture> &getMainTexture() const;
		void setMainTexture(const std::shared_ptr<Texture> value);

		Color getMainColor() const;
		void setMainColor(Color color);


		void _setBuildInMatrixData(Matrix4x4 matrixObjectToNDC
			, Matrix4x4 matrixObjectToView
			, Matrix4x4 matrixObjectToWorld
		);

		void apply();

		CullModeFlags getCullMode() const;
		void setCullMode(CullModeFlags cullMode);

		FrontFaceType getFrontFace() const;
		void setFrontFace(FrontFaceType frontFace);

		std::shared_ptr<Shader> _getShader();
		std::shared_ptr<vk::Buffer> _getUniformBuffer();
		std::shared_ptr<vk::DeviceMemory> _getUniformBufferMemory();
		std::shared_ptr<vk::DescriptorSetLayout> _getDescriptorSetLayout();
		std::shared_ptr<vk::DescriptorPool> _getDescriptorPool();
		std::shared_ptr<vk::DescriptorSet> _getDescriptorSet();
	private:
		//compositons
		std::shared_ptr<MaterialData> m_pData;
		std::vector<std::string> m_arrLayoutBindNames;
		std::unordered_map<std::string, LayoutBindingInfo> m_mapLayoutBinds;
		std::shared_ptr<vk::Buffer> m_pUniformBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pUniformBufferMemory;
		std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
		std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;
		Bool32 m_applied;

		CullModeFlags m_cullMode;
		FrontFaceType m_frontFace;

		MaterialData::BuildInData m_buildInData;

		//aggregations
		std::shared_ptr<Shader> m_pShader;
		void _createDescriptorSetLayout();
		void _createUniformBuffer();
		void _createDescriptorSet();
		void _updateDescriptorBufferInfo();
		void _updateDescriptorImageInfo();
		void _applyBufferContent();

		//tool methods
		void createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer> &pBuffer, std::shared_ptr<vk::DeviceMemory> &pBufferMemory);
	};
}

#include "graphics/material/pass.inl"

#endif // !KGS_PASS_H
