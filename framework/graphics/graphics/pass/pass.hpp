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
#include "graphics/texture/texture_default.hpp"

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
			PRE_Z_DEPTH_RESULT = 6,
			POST_RENDER_RESULT = 7,
			COUNT = 8
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

		struct VertexInputFilterInfo
		{
			Bool32 filterEnable;
			uint32_t locationCount;
			uint32_t * pLocations;

			VertexInputFilterInfo(Bool32 filterEnable = VG_FALSE
			    , uint32_t locationCount = 0u
				, uint32_t * pLocations = nullptr);
		};

		Pass();
		Pass(Shader *pShader);
		~Pass();

		const Shader *getShader() const;
		Shader *getShader();		
		void setShader(Shader *pShader);

		Bool32 hasBuffer(std::string name) const;
		void addBuffer(std::string name, const PassBufferInfo &bufferInfo);
		void removeBuffer(std::string name);
		PassBufferInfo getBuffer(std::string name);
		void setBuffer(std::string name, const PassBufferInfo &bufferInfo);

		Bool32 hasTexture(std::string name) const;
		void addTexture(std::string name, const PassTextureInfo &texInfo);
		void removeTexture(std::string name);
		PassTextureInfo getTexture(std::string name) const;
		void setTexture(std::string name, const PassTextureInfo &texInfo);

        Bool32 hasData(std::string name) const;
		void removeData(std::string name);

		void addData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
		void addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size);
		void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
		void setData(const std::string name, const PassDataInfo &info, const PassDataSizeInfo &sizeInfo);
		void setData(const std::string name, void *src, uint32_t size, uint32_t offset);

        template<typename T>
		void addData(const std::string name, const PassDataInfo &info, const T &value);
		template<typename T>
		T getData(const std::string name) const;
		template<typename T>
		void setData(const std::string name, const T &value);
        
		template<typename T>
		void addData(const std::string name, const PassDataInfo &info, const std::vector<T> &values);
		template <typename T>
		std::vector<T> getData(const std::string name, const uint32_t count) const;
		template<typename T>
		void setData(const std::string name, const std::vector<T> &values);

		template<typename T>
		void addData(const std::string name, const PassDataInfo &info, const T * const pSrc, const uint32_t count);
		template<typename T>
		void getData(const std::string name, const T * const pDst, const uint32_t count);
		template<typename T>
		void setData(const std::string name, const T * const pSrc, const uint32_t count);


		Color getMainColor() const;
		void setMainColor(Color color);

        void setBuildInDataInfo(BuildInDataInfo info);
		const BuildInDataInfo &getBuildInDataInfo() const;

		void setBuildInMatrixData(BuildInDataType type, Matrix4x4 matrix);

		vk::PolygonMode getPolygonMode() const;
		void setPolygonMode(vk::PolygonMode polygonMode);

		vk::CullModeFlags getCullMode() const;
		void setCullMode(vk::CullModeFlags cullMode);

		vk::FrontFace getFrontFace() const;
		void setFrontFace(vk::FrontFace frontFace);

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

		const vk::PipelineInputAssemblyStateCreateInfo &getDefaultInputAssemblyState() const;
		void setDefaultInputAssemblyState(vk::PipelineInputAssemblyStateCreateInfo & value);

		const Bool32 IsHasSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const SpecializationData *getSpecializationData(vk::ShaderStageFlagBits shaderStage) const;

		const std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<Pass::SpecializationData>> &getSpecilizationDatas() const;
		std::vector<vk::PushConstantRange> getPushConstantRanges() const;
		std::vector<std::shared_ptr<PushConstantUpdate>> getPushconstantUpdates() const;
		
		void setSpecializationData(vk::ShaderStageFlagBits shaderStage
			, const vk::SpecializationInfo &info);

		Bool32 hasPushConstantRange(std::string name) const;
		void addPushConstantRange(std::string name, vk::ShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			);
		void removePushConstantRange(std::string name);
	    void setPushConstantRange(std::string name
		    , vk::ShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			);

		Bool32 hasPushConstantUpdate(std::string name) const;
		void addPushConstantUpdate(std::string name
		    , const void *pData
			, uint32_t size
			, vk::ShaderStageFlags stageFlags
			, uint32_t offset
			);
		void removePushConstantUpdate(std::string name);
		void setPushConstantUpdate(std::string name
		    , const void *pData
			, uint32_t size
			, vk::ShaderStageFlags stageFlags
			, uint32_t offset
			);
        
		template<typename T>
		void addPushConstantUpdate(std::string name
		    , const T &data
			, vk::ShaderStageFlags stageFlags
			, uint32_t offset
			);

        template<typename T>
		void setPushConstantUpdate(std::string name
		    , const T &data
			, vk::ShaderStageFlags stageFlags 
			, uint32_t offset
			);

		uint32_t getInstanceCount() const;
		void setInstanceCount(uint32_t count);

		uint32_t getSubpass() const;
		void setSubpass(uint32_t subpass);

		const VertexInputFilterInfo &getVertexInputFilter() const;
		void setVertexInputFilterInfo(const VertexInputFilterInfo &value);

        //external uniform buffer
	    uint32_t getExtUniformBufferCount() const;
		const ExternalUniformBufferInfo * getExtUniformBuffers() const;
		void setExtUniformBufferCount(uint32_t value);
		void setExtUniformBuffers(fd::ArrayProxy<ExternalUniformBufferInfo> extUniformBuffers
			, uint32_t offset = 0u
			);

		const BufferData &getBufferData() const;
		const vk::DescriptorSetLayout *getDescriptorSetLayout() const;
		const vk::DescriptorPool *getDescriptorPool() const;
		const vk::DescriptorSet *getDescriptorSet() const;
		const vk::PipelineLayout *getPipelineLayout() const;
		uint32_t getUsingDescriptorSetCount() const;
		const vk::DescriptorSet *getUsingDescriptorSets() const;
		uint32_t getUsingDescriptorDynamicOffsetCount() const;
		const uint32_t *getUsingDescriptorDynamicOffsets() const;

		PipelineStateID getPipelineStateID() const;

		void apply();

		void beginRecord() const;
		void endRecord() const;
	private:
		PassData m_data;
		Bool32 m_dataChanged;
		Bool32 m_dataContentChanged;
		std::unordered_map<std::string, Bool32> m_dataContentChanges;
		Bool32 m_textureChanged;
		Bool32 m_bufferChanged;

		vk::PolygonMode m_polygonMode;
		vk::CullModeFlags m_cullMode;
		vk::FrontFace m_frontFace;
		fd::Viewport m_viewport;
		fd::Rect2D m_scissor;
		vk::PipelineDepthStencilStateCreateInfo m_depthStencilInfo;
		std::vector<vk::PipelineColorBlendAttachmentState> m_colorBlendAttachmentStates;
		vk::PipelineColorBlendStateCreateInfo m_colorBlendInfo;
		float m_lineWidth;
		uint32_t m_instanceCount;
		uint32_t m_subpass;
		vk::PipelineInputAssemblyStateCreateInfo m_defaultInputAssemblyState;
		
		//each stage may own a specilization constant data.
		std::unordered_map<vk::ShaderStageFlagBits, std::shared_ptr<SpecializationData>> m_mapSpecilizationDatas;
		std::unordered_map<std::string, vk::PushConstantRange> m_mapPushConstantRanges;
		std::vector<std::string> m_arrPushConstantRangeNames;		
		std::unordered_map<std::string, std::shared_ptr<PushConstantUpdate>> m_mapPPushConstantUpdates;
		std::vector<std::string> m_arrPushConstantUpdateNames;		

		BuildInDataInfo m_buildInDataInfo;		
		std::vector<BuildInDataInfo::Component> m_buildInDataInfoComponents;
		_BuildInDataCache m_buildInDataCache;

		VertexInputFilterInfo m_vertexInputFilterInfo;
		std::vector<uint32_t> m_vertexInputFilterLocations;

		//external uniform buffer data.
		uint32_t m_extUniformBufferCount;
		std::vector<ExternalUniformBufferInfo> m_extUniformBuffers;

		////////applied data

		//Build in buffer.
		struct DataSortInfo {
			std::string name;
			vk::ShaderStageFlags shaderStageFlags;
			uint32_t layoutPriority;
			uint32_t size;
			uint32_t bufferSize;

			DataSortInfo(std::string name = nullptr
			    , vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
				, uint32_t layoutPriority = 0u
				, uint32_t size = 0u
				, uint32_t bufferSize = 0u
				);
		};
		static Bool32 _compareDataInfo(const DataSortInfo &, const DataSortInfo &);
		std::set<DataSortInfo, Bool32(*)(const DataSortInfo &, const DataSortInfo &)> m_sortDataSet;
		BufferData m_dataBuffer;

		//build in descriptor set layout.
		struct BufferTextureSortInfo {
			std::string name;
			uint32_t bindingPriority;			
			Bool32 isTexture;
			const void *pInfo;

			BufferTextureSortInfo(std::string name = nullptr
				, uint32_t bindingPriority = 0u			
			    , Bool32 isTexture = VG_FALSE
				, const void *pInfo = nullptr
				);
		};
		static Bool32 _compareBufferTextureInfo(const BufferTextureSortInfo &, const BufferTextureSortInfo &);
		std::set<BufferTextureSortInfo, Bool32(*)(const BufferTextureSortInfo &, const BufferTextureSortInfo &)> m_sortBufferTexInfosSet;
		Bool32 m_descriptorSetChanged;
		uint32_t m_layoutBindingCount;		
        std::vector<vk::DescriptorSetLayoutBinding> m_descriptorSetLayoutBindings;
		struct UpdateDescriptorSetInfo {
			std::vector<vk::DescriptorBufferInfo> bufferInfos;
			std::vector<vk::DescriptorImageInfo> imageInfos;

			UpdateDescriptorSetInfo(std::vector<vk::DescriptorBufferInfo> bufferInfos = std::vector<vk::DescriptorBufferInfo>()
				, std::vector<vk::DescriptorImageInfo> imageInfos = std::vector<vk::DescriptorImageInfo>()
				);
		};
		std::vector<UpdateDescriptorSetInfo> m_updateDescriptorSetInfos;
        std::shared_ptr<vk::DescriptorSetLayout> m_pDescriptorSetLayout;		

		//build in descriptor pool.
		std::unordered_map<vk::DescriptorType, uint32_t> m_poolSizeInfos;
		std::shared_ptr<vk::DescriptorPool> m_pDescriptorPool;

		//build in descriptor set
        std::shared_ptr<vk::DescriptorSet> m_pDescriptorSet;

		//all descriptor sets and layouts
		Bool32 m_descriptorSetsChanged;
        //all descriptor set layouts
	    std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
		//all descriptor sets.
		std::vector<vk::DescriptorSet> m_descriptorSets;
		std::vector<uint32_t> m_dynamicOffsets;

		//push constant Ranges.
		Bool32 m_pushConstantChanged;
		std::vector<vk::PushConstantRange> m_pushConstantRanges;

		//pipeline layout
		std::shared_ptr<vk::PipelineLayout> m_pPipelineLayout;
		Bool32 m_pipelineLayoutChanged;
        PipelineStateID m_pipelineStateID;

		Shader *m_pShader;		

		void _initDefaultBuildInDataInfo();
		void _initBuildInData();
		
		template <typename T>
		void _updateBuildInData(BuildInDataType type, T data);
        void _updatePipelineStateID();
		void _applyUniformBufferDynamicOffsets();
	};
}

#include "graphics/pass/pass.inl"

#endif // !VG_PASS_H
