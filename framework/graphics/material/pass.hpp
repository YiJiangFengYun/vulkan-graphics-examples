#ifndef VG_PASS_H
#define VG_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/util/util.hpp"

//to do 
//specilazation constant and push constant
namespace vg
{
	class Pass : public Base
	{
	public:
	    using PipelineStateID = uint32_t;	    
	    // to do
	    class SpecializationData 
		{
        public:
		    SpecializationData();
			~SpecializationData();
			SpecializationData(const SpecializationData &);
			SpecializationData(const SpecializationData &&);
			SpecializationData& operator=(const SpecializationData &);

			void init(const void* pData
			    , uint32_t size
				, const vk::SpecializationInfo &info);

			template<typename T>
			void init(const T &data
				, const vk::SpecializationInfo &info);

            const vk::SpecializationInfo getInfo() const;
			const void * getData() const;
			uint32_t getSize() const;

			template<typename T>
			T getData() const;
		private:
			std::vector<vk::SpecializationMapEntry> m_mapEntries;
			vk::SpecializationInfo m_info;
			uint32_t m_size;
			void *m_pData;
		};

		class PushConstantUpdate 
		{
		public:
	        PushConstantUpdate();
			~PushConstantUpdate();

			void init(const void *pData
			     , uint32_t size
				 , vk::ShaderStageFlags stageFlags
				 , uint32_t offset);

			template<typename T>
			void init(const T &data
			    , vk::ShaderStageFlags stageFlags 
				, uint32_t offset);

			vk::ShaderStageFlags getStageFlags() const;
			uint32_t getOffset() const;
			const void *getData() const;
			uint32_t getSize() const;			

            template<typename T>
			T getData() const;
		private:
		   vk::ShaderStageFlags m_stageFlags;
		   uint32_t m_offset;
		   uint32_t m_size;
		   void* m_pData;
		};

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
			, uint32_t binding = VG_M_OTHER_MIN_BINDING
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
			, uint32_t binding = VG_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
		);

		template<typename T>
		void setDataValue(std::string name
			, const std::vector<T> &values
			, uint32_t binding = VG_M_OTHER_MIN_BINDING
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
        
		PipelineStateID getPipelineStateID() const;

		CullModeFlags getCullMode() const;
		void setCullMode(CullModeFlags cullMode);

		FrontFaceType getFrontFace() const;
		void setFrontFace(FrontFaceType frontFace);

		const fd::Viewport &getViewport() const;
		void setViewport(const fd::Viewport &viewport);

		const fd::Rect2D &getScissor() const;
		void setScissor(const fd::Rect2D &scissor);

		const DepthStencilInfo &getDepthStencilStateInfo() const;
		void setDepthStencilStateInfo(const DepthStencilInfo &value);

		const ColorBlendInfo &getColorBlendInfo() const;
		void setColorBlendInfo(const ColorBlendInfo &value);

		const Bool32 IsHasSpecializationData(ShaderStageFlagBits shaderStage) const;
		const Bool32 IsHasSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const std::shared_ptr<SpecializationData> &getSpecializationData(ShaderStageFlagBits shaderStage) const;
		const std::shared_ptr<SpecializationData> &getSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<Pass::SpecializationData>> &getSpecilizationDatas() const;
		std::vector<vk::PushConstantRange> getPushConstantRanges() const;
		std::vector<std::shared_ptr<PushConstantUpdate>> getPushconstantUpdates() const;

		void setSpecializationData(ShaderStageFlagBits shaderStage
		    , void* pData
			, uint32_t size
			, const vk::SpecializationInfo &info);

		template<typename T>
		void setSpecializationData(ShaderStageFlagBits shaderStage
		    , const T &data
			, const vk::SpecializationInfo &info);

	    void setPushConstantRange(std::string name
		    , vk::ShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size);

		void setPushConstantUpdate(std::string name
		    , const void *pData
			, uint32_t size
			, vk::ShaderStageFlags stageFlags
			, uint32_t offset);

        template<typename T>
		void setPushConstantUpdate(std::string name
		    , const T &data
			, vk::ShaderStageFlags stageFlags 
			, uint32_t offset);

		const std::shared_ptr<Shader> & getShader() const;
		const std::shared_ptr<vk::Buffer> &getUniformBuffer() const;
		const std::shared_ptr<vk::DeviceMemory> &getUniformBufferMemory() const;
		const std::shared_ptr<vk::DescriptorSetLayout> &getDescriptorSetLayout() const;
		const std::shared_ptr<vk::DescriptorPool> &getDescriptorPool() const;
		const std::shared_ptr<vk::DescriptorSet> &getDescriptorSet() const;
		const std::shared_ptr<vk::PipelineLayout> &getPipelineLayout() const;  
	private:
		//compositons
		std::shared_ptr<MaterialData> m_pData;
		std::vector<std::string> m_arrLayoutBindNames;
		std::unordered_map<std::string, LayoutBindingInfo> m_mapLayoutBinds;
		std::shared_ptr<vk::Buffer> m_pUniformBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pUniformBufferMemory;
		std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
		std::shared_ptr<vk::PipelineLayout> m_pPipelineLayout;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
		std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;

		Bool32 m_applied;

        PipelineStateID m_pipelineStateID;
		CullModeFlags m_cullMode;
		FrontFaceType m_frontFace;
		fd::Viewport m_viewport;
		fd::Rect2D m_scissor;
		DepthStencilInfo m_depthStencilStateInfo;
		ColorBlendInfo m_colorBlendInfo;
		//todo
		//each stage may own a specilization constant data.
		std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<SpecializationData>> m_mapSpecilizationDatas;
		std::unordered_map<std::string, vk::PushConstantRange> m_mapPushConstantRanges;
		std::vector<std::string> m_arrPushConstantRangeNames;		
		std::unordered_map<std::string, std::shared_ptr<PushConstantUpdate>> m_mapPPushConstantUpdates;
		std::vector<std::string> m_arrPushConstantUpdateNames;		

		MaterialData::BuildInData m_buildInData;

		//cache
		std::vector<vk::DescriptorSetLayoutBinding> m_lastBindings;

		//aggregations
		std::shared_ptr<Shader> m_pShader;
		void _createDescriptorSetLayout();
		void _createPipelineLayout();  
		void _createUniformBuffer();
		void _createDescriptorSet();
		void _updateDescriptorBufferInfo();
		void _updateDescriptorImageInfo();
		void _applyBufferContent();
		void _updatePipelineStateID();

		//tool methods
		void createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer> &pBuffer, std::shared_ptr<vk::DeviceMemory> &pBufferMemory);
	};
}

#include "graphics/material/pass.inl"

#endif // !VG_PASS_H
