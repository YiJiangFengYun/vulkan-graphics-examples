#ifndef KGS_PASS_H
#define KGS_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/util/util.hpp"

#define KGS_M_MAIN_TEXTURE_NAME "_MainTex"
#define KGS_M_MAIN_COLOR_NAME "_Color"
#define KGS_M_MATRIX_OBJECT_TO_NDC_NAME "_MatrixObjectToNDC"
#define KGS_M_MATRIX_OBJECT_TO_WORLD_NAME "_MatrixObjectToWorld"
#define KGS_M_MATRIX_OBJECT_TO_VIEW_NAME "_MatrixObjectToView"
#define KGS_M_MAIN_TEXTURE_OFFSET_NAME "_MainTexOffset"
#define KGS_M_MAIN_TEXTURE_SCALE_NAME "_MainTexScale"

#define KGS_M_MAIN_TEXTURE_BINDING 0
#define KGS_M_MAIN_COLOR_BINDING 1
#define KGS_M_MATRIX_OBJECT_TO_NDC_BINDING 2
#define KGS_M_MATRIX_OBJECT_TO_WORLD_BINDING 3
#define KGS_M_MATRIX_OBJECT_TO_VIEW_BINDING 4
#define KGS_M_MAIN_TEXTURE_OFFSET_BINDING 5
#define KGS_M_MAIN_TEXTURE_SCALE_BINDING 6
#define KGS_M_OTHER_MIN_BINDING 7

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

			std::uint32_t size;
			std::uint32_t bufferSize;

			LayoutBindingInfo();

			LayoutBindingInfo(std::string name
				, MaterialData::DataType dataType
				, std::uint32_t binding
				, DescriptorType descriptorType
				, std::uint32_t descriptorCount
				, ShaderStageFlags stageFlags
			);

			LayoutBindingInfo(const LayoutBindingInfo &);

			LayoutBindingInfo(const LayoutBindingInfo &&);

			LayoutBindingInfo& operator=(const LayoutBindingInfo &);

			Bool32 operator ==(const LayoutBindingInfo& target) const;

			void updateSize();
		};

		Pass();
		Pass(std::shared_ptr<Shader> pShader);
		~Pass();

		std::shared_ptr<Shader> getShader();
		void setShader(std::shared_ptr<Shader> pShader);

		template <MaterialData::DataType dataType>
		const typename MaterialData::DataTypeInfo<dataType>::ValueType getData(std::string name) const;

		template <MaterialData::DataType dataType>
		void setData(std::string name
			, const typename MaterialData::DataTypeInfo<dataType>::ValueType value
			, uint32_t binding = KGS_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
		);

		const std::shared_ptr<Texture> getMainTexture() const;
		void setMainTexture(const std::shared_ptr<Texture> value);
		const Vector2 getMainTextureOffset() const;
		void setMainTextureOffset(const Vector2 value);
		const Vector2 getMainTextureScale() const;
		void setMainTextureScale(const Vector2 value);

		const Color getMainColor() const;
		void setMainColor(const Color value);

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
