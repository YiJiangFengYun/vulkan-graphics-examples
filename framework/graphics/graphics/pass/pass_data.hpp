#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/buffer_data/buffer_data.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/pass/pass_option.hpp"


#define VG_PASS_BUILDIN_DATA_NAME "_BuildIn"

#define VG_PASS_PRE_Z_TEXTURE_NAME "_pre_z_depth_tex"
#define VG_PASS_POST_RENDER_TEXTURE_NAME "_post_render_tex"

#define VG_PASS_BUILDIN_DATA_LAYOUT_PRIORITY 0
#define VG_PASS_OTHER_DATA_MAX_LAYOUT_PRIORITY 1

#define VG_PASS_PRE_Z_TEXTURE_BINDING_PRIORITY 1
#define VG_PASS_POST_RENDER_TEXTURE_BINDING_PRIORITY 2
#define VG_PASS_OTHER_MAX_BINDING_PRIORITY 3

namespace vg
{
	struct PassDataInfo {
		uint32_t layoutPriority;
		vk::ShaderStageFlags shaderStageFlags;
		uint32_t size;
		PassDataInfo(uint32_t layoutPriority = 0u
			, vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlags()
			, uint32_t size = 0u
		);
		PassDataInfo(const PassDataInfo &);
		PassDataInfo& operator=(const PassDataInfo &);
		uint32_t getBufferSize() const;
	};

	struct PassTextureInfo {
		const Texture *pTexture;
		const Texture::ImageView *pImageView;
		const Texture::Sampler *pSampler;
		vk::ImageLayout imageLayout;			
		uint32_t bindingPriority;
		ImageDescriptorType descriptorType;
		vk::ShaderStageFlags stageFlags;
		PassTextureInfo(const Texture *pTexture = nullptr
			, const Texture::ImageView *pImageView = nullptr
			, const Texture::Sampler *pSampler = nullptr
			, vk::ImageLayout imageLayout = vk::ImageLayout::eUndefined
		    , uint32_t bindingPriority = 0u
			, ImageDescriptorType descriptorType = ImageDescriptorType::COMBINED_IMAGE_SAMPLER
			, vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
			);
		PassTextureInfo(const PassTextureInfo &);
		PassTextureInfo& operator=(const PassTextureInfo &);
	};

	struct PassBufferInfo {
		const BufferData *pBuffer;
		uint32_t offset;
		uint32_t range;
		uint32_t bindingPriority;
		BufferDescriptorType descriptorType;
		vk::ShaderStageFlags stageFlags;
		PassBufferInfo(const BufferData *pBuffer = nullptr
			, uint32_t offset = 0u
			, uint32_t range = 0u
		    , uint32_t bindingPriority = 0u
		    , BufferDescriptorType descriptorType = BufferDescriptorType::UNIFORM_BUFFER
		    , vk::ShaderStageFlags stageFlags = vk::ShaderStageFlags()
		    );
		PassBufferInfo(const PassBufferInfo &);
		PassBufferInfo& operator=(const PassBufferInfo &);
	};


    struct PassData
	{	
		std::vector<std::string> arrDataNames;
		std::unordered_map<std::string, std::vector<Byte>> mapDatas;
		std::unordered_map<std::string, uint32_t> mapDataCounts;

		std::unordered_map<std::string, PassDataInfo> mapDataInfos;

		std::vector<std::string> arrBufferNames;
		std::unordered_map<std::string, PassBufferInfo> mapBuffers;

		std::vector<std::string> arrTexNames;
		std::unordered_map<std::string, PassTextureInfo> mapTextures;

		const std::vector<std::string> getArrBufferNames() const;
		Bool32 hasBuffer(std::string name) const;
		void addBuffer(std::string name, const PassBufferInfo &bufferInfo);
		void removeBuffer(std::string name);
		const PassBufferInfo &getBuffer(std::string name) const;
		void setBuffer(std::string name, const PassBufferInfo &bufferInfo);

        const std::vector<std::string> getArrTextureNames() const;
		Bool32 hasTexture(std::string name) const;
		void addTexture(std::string name, const PassTextureInfo &texInfo);
		void removeTexture(std::string name);
		const PassTextureInfo &getTexture(std::string name) const;
		void setTexture(std::string name, const PassTextureInfo &texInfo);

        const std::vector<std::string> getArrDataNames() const;
		Bool32 hasData(std::string name) const;
		void removeData(std::string name);
	    const PassDataInfo &getDataInfo(std::string name) const;

		void addData(const std::string name, const PassDataInfo &info, void *src, uint32_t size);
		void getData(const std::string name, void *dst, uint32_t size, uint32_t offset) const;
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

		uint32_t getDataBaseSize(const std::string name) const;

		uint32_t getDataSize(const std::string name) const;

		void memoryCopyData(const std::string name
			, void* dst
			, uint32_t offset
			, uint32_t elementStart
			, uint32_t maxElementCount) const;
	};
} //vg

#include "graphics/pass/pass_data.inl"

#endif //VG_PASS_DATA_HPP