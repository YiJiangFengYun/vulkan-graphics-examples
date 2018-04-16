#ifndef VG_PASS_H
#define VG_PASS_H

#include <unordered_map>
#include "graphics/util/find_memory.hpp"
#include "graphics/pass/shader.hpp"
#include "graphics/global.hpp"
#include "graphics/pass/pass_option.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_data.hpp"
#include "graphics/util/util.hpp"
#include "graphics/buffer_data/uniform_buffer_data.hpp"

//to do 
//specilazation constant and push constant
namespace vg
{
	class Pass : public Base
	{
	public:
	    struct ExternalUniformBufferInfo 
		{
			UniformBufferData *pData;
			uint32_t subDataOffset;
			uint32_t subDataCount;
			ExternalUniformBufferInfo(UniformBufferData *pData = nullptr
			    , uint32_t subDataOffset = 0u
				, uint32_t subDataCount = 0u);
		};

	    enum class BuildInDataType
		{
			MATRIX_OBJECT_TO_NDC = 0,
			MAIN_CLOLOR = 1,
			MATRIX_OBJECT_TO_WORLD = 2,
			MATRIX_OBJECT_TO_VIEW = 3,
			MATRIX_VIEW = 4,
			MATRIX_PROJECTION = 5,
			COUNT = 6
		};

        template<BuildInDataType type>
		struct BuildInDataTypeTypeInfo
		{
			using Type = void;
		};

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_NDC>
	    {
	    	using Type = Matrix4x4;
	    };

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MAIN_CLOLOR>
	    {
	    	using Type = Color;
	    };

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_WORLD>
	    {
	    	using Type = Matrix4x4;
	    };

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_OBJECT_TO_VIEW>
	    {
	    	using Type = Matrix4x4;
	    };

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_VIEW>
	    {
	    	using Type = Matrix4x4;
	    };

		template<>
	    struct BuildInDataTypeTypeInfo<BuildInDataType::MATRIX_PROJECTION>
	    {
	    	using Type = Matrix4x4;
	    };

		static const std::array<uint32_t, static_cast<size_t>(BuildInDataType::COUNT)> buildInDataTypeSizes;

		struct BuildInDataInfo
		{
			struct Component {
				BuildInDataType type;
			};

			uint32_t componentCount;
			Component *pComponent;
			BuildInDataInfo();
		};

		struct _BuildInDataCache
		{
			Matrix4x4 matrixObjectToNDC;
			Color     mainColor;
			Matrix4x4 matrixObjectToWorld;
			Matrix4x4 matrixObjectToView;
			Matrix4x4 matrixView;
			Matrix4x4 matrixProjection;

			_BuildInDataCache();

			_BuildInDataCache(Matrix4x4 matrixObjectToNDC
				, Color mainColor
				, Matrix4x4 matrixObjectToWorld
				, Matrix4x4 matrixObjectToView
				, Matrix4x4 matrixView
				, Matrix4x4 matrixProjection);

			_BuildInDataCache(const _BuildInDataCache &target);
			_BuildInDataCache(const _BuildInDataCache &&target);
		};

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

			void init(const vk::SpecializationInfo &info);

            const vk::SpecializationInfo getInfo() const;
			const void * getData() const;
			uint32_t getSize() const;
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
			uint32_t binding;
			DescriptorType descriptorType;
			uint32_t descriptorCount;
			ShaderStageFlags stageFlags;

			uint32_t size;
			uint32_t bufferSize;

			LayoutBindingInfo();

			LayoutBindingInfo(std::string name
				, Bool32 isTexture
				, uint32_t binding
				, DescriptorType descriptorType
				, uint32_t descriptorCount
				, ShaderStageFlags stageFlags
			);

			LayoutBindingInfo(const LayoutBindingInfo &);

			LayoutBindingInfo(const LayoutBindingInfo &&);

			LayoutBindingInfo& operator=(const LayoutBindingInfo &);

			Bool32 operator ==(const LayoutBindingInfo& target) const;
			Bool32 operator !=(const LayoutBindingInfo& target) const;

			void updateSize(const PassData *pPassData);
			void updateSize(const uint32_t dataSize);
		};

		Pass();
		Pass(Shader *pShader);
		~Pass();

		Shader *getShader() const;
		void setShader(Shader *pShader);

		const Texture *getTexture(std::string name) const;

		void setTexture(std::string name
			, const Texture *pTex
			, uint32_t binding = VG_M_OTHER_MIN_BINDING
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::FRAGMENT
			, DescriptorType descriptorType = DescriptorType::COMBINED_IMAGE_SAMPLER
			, const Texture::ImageView *pImageView = nullptr
			, const Texture::Sampler *pSampler = nullptr
		);

		template <typename T>
		T getDataValue(std::string name) const;

		template <typename T>
		std::vector<T> getDataValue(std::string name, uint32_t count) const;

		void getDataValue(const std::string name, void *dst, uint32_t size, uint32_t offset) const;

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

		void setDataValue(const std::string name
		    , void *src, uint32_t size, uint32_t offset
			, uint32_t binding = VG_M_OTHER_MIN_BINDING
			, DescriptorType descriptorType = DescriptorType::UNIFORM_BUFFER
			, ShaderStageFlags stageFlags = ShaderStageFlagBits::VERTEX
			, uint32_t descriptorCount = 1u);

		const Texture *getMainTexture() const;
		void setMainTexture(const Texture *value
		    , ShaderStageFlags stageFlags = ShaderStageFlagBits::FRAGMENT
			, DescriptorType descriptorType = DescriptorType::COMBINED_IMAGE_SAMPLER
		    , const Texture::ImageView *pImageView = nullptr
			, const Texture::Sampler *pSampler = nullptr
			);

		Color getMainColor() const;
		void setMainColor(Color color);


        void setBuildInDataInfo(BuildInDataInfo info);
		const BuildInDataInfo &getBuildInDataInfo() const;

		void _setBuildInMatrixData(BuildInDataType type, Matrix4x4 matrix);

		void apply();
        
		PipelineStateID getPipelineStateID() const;

		PolygonMode getPolygonMode() const;
		void setPolygonMode(PolygonMode polygonMode);

		CullModeFlags getCullMode() const;
		void setCullMode(CullModeFlags cullMode);

		FrontFaceType getFrontFace() const;
		void setFrontFace(FrontFaceType frontFace);

		float getLineWidth() const;
		void setLineWidth(float lineWidth);

		const fd::Viewport &getViewport() const;
		void setViewport(const fd::Viewport &viewport);

		const fd::Rect2D &getScissor() const;
		void setScissor(const fd::Rect2D &scissor);

		const vk::PipelineDepthStencilStateCreateInfo &getDepthStencilInfo() const;
		void setDepthStencilInfo(const vk::PipelineDepthStencilStateCreateInfo &value);

		const vk::PipelineColorBlendStateCreateInfo &getColorBlendInfo() const;
		void setColorBlendInfo(const vk::PipelineColorBlendStateCreateInfo &value);

		const Bool32 IsHasSpecializationData(ShaderStageFlagBits shaderStage) const;
		const Bool32 IsHasSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const SpecializationData *getSpecializationData(ShaderStageFlagBits shaderStage) const;
		const SpecializationData *getSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<Pass::SpecializationData>> &getSpecilizationDatas() const;
		std::vector<vk::PushConstantRange> getPushConstantRanges() const;
		std::vector<std::shared_ptr<PushConstantUpdate>> getPushconstantUpdates() const;

		void setSpecializationData(ShaderStageFlagBits shaderStage
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

		const uint32_t getInstanceCount() const;
		void setInstanceCount(uint32_t count);

		const ExternalUniformBufferInfo getExternalUniformBufferInfo() const;
		void setExternalUniformBufferData(ExternalUniformBufferInfo value);

		void updateExternalUniformBufferData();
		void applyUniformBufferDynamicOffsets();

		const vk::Buffer *getUniformBuffer() const;
		const vk::DeviceMemory *getUniformBufferMemory() const;
		const vk::DescriptorSetLayout *getDescriptorSetLayout() const;
		const vk::DescriptorPool *getDescriptorPool() const;
		const vk::DescriptorSet *getDescriptorSet() const;
		const vk::PipelineLayout *getPipelineLayout() const;
		uint32_t getUsingDescriptorSetCount() const;
		const vk::DescriptorSet *getUsingDescriptorSets() const;
		uint32_t getUsingDescriptorDynamicOffsetCount() const;
		const uint32_t *getUsingDescriptorDynamicOffsets() const;
	private:
		//compositons
		std::shared_ptr<PassData> m_pData;
		Bool32 m_dataChanged;
		std::vector<std::string> m_arrLayoutBindNames;
		std::unordered_map<std::string, LayoutBindingInfo> m_mapLayoutBinds;
		std::shared_ptr<vk::Buffer> m_pUniformBuffer;
		std::shared_ptr<vk::DeviceMemory> m_pUniformBufferMemory;
		uint32_t m_uniformBufferSize;
		std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;
		std::shared_ptr<vk::PipelineLayout> m_pPipelineLayout;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;
		std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;

		Bool32 m_applied;

        PipelineStateID m_pipelineStateID;
		PolygonMode m_polygonMode;
		CullModeFlags m_cullMode;
		FrontFaceType m_frontFace;
		fd::Viewport m_viewport;
		fd::Rect2D m_scissor;
		vk::PipelineDepthStencilStateCreateInfo m_depthStencilInfo;
		std::vector<vk::PipelineColorBlendAttachmentState> m_colorBlendAttachmentStates;
		vk::PipelineColorBlendStateCreateInfo m_colorBlendInfo;
		float m_lineWidth;

		uint32_t m_instanceCount;
		//todo
		//each stage may own a specilization constant data.
		std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<SpecializationData>> m_mapSpecilizationDatas;
		std::unordered_map<std::string, vk::PushConstantRange> m_mapPushConstantRanges;
		std::vector<std::string> m_arrPushConstantRangeNames;		
		std::unordered_map<std::string, std::shared_ptr<PushConstantUpdate>> m_mapPPushConstantUpdates;
		std::vector<std::string> m_arrPushConstantUpdateNames;		

		BuildInDataInfo m_buildInDataInfo;		
		std::vector<BuildInDataInfo::Component> m_buildInDataInfoComponents;
		_BuildInDataCache m_buildInDataCache;

		//external uniform buffer data.
		ExternalUniformBufferInfo m_externalUniformBufferInfo;

		//cache
		std::vector<vk::DescriptorSetLayoutBinding> m_lastBindings;
		std::vector<std::string> m_lastLayoutBindNames;
		std::unordered_map<std::string, LayoutBindingInfo> m_lastLayoutBinds;
		std::vector<vk::PushConstantRange> m_lastPushConstantRanges;
	    std::vector<vk::DescriptorSetLayout> m_lastsetLayouts;
		std::unordered_map<vk::DescriptorType, uint32_t> m_lastPoolSizeInfos;
		Bool32 m_needReAllocateDescriptorSet;
		Bool32 m_needUpdateDescriptorInfo;
		std::vector<vk::DescriptorSet> m_usingDescriptorSets;
		std::vector<uint32_t> m_usingDynamicOffsets;

		//aggregations
		Shader *m_pShader;
		void _createPipelineLayout();  
		void _createUniformBuffer();
		void _createDescriptorSet();
		void _beginCheckNeedUpdateDescriptorInfo();
		void _updateDescriptorBufferInfo();
		void _updateDescriptorImageInfo();
		void _endCheckNeedUpdateDescriptorInfo();
		void _applyBufferContent();
		void _updatePipelineStateID();

		void _initDefaultBuildInDataInfo();
		void _initBuildInData();
		template <typename T>
		void _updateBuildInData(BuildInDataType type, T data);
		//tool methods
		void createBuffer(vk::DeviceSize size, std::shared_ptr<vk::Buffer> &pBuffer, std::shared_ptr<vk::DeviceMemory> &pBufferMemory);
	};
}

#include "graphics/pass/pass.inl"

#endif // !VG_PASS_H
