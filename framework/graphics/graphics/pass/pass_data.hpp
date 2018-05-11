#ifndef VG_PASS_DATA_HPP
#define VG_PASS_DATA_HPP

#include "graphics/global.hpp"
#include "graphics/texture/texture.hpp"


#define VG_M_MAIN_TEXTURE_NAME "_MainTex"
#define VG_M_BUILDIN_NAME "_BuildIn"
#define VG_M_PRE_Z_TEXTURE_NAME "_pre_z_depth_tex"
#define VG_M_POST_RENDER_TEXTURE_NAME "_post_render_tex"

#define VG_M_BUILDIN_BINDING_PRIORITY 0
#define VG_M_MAIN_TEXTURE_BINDING_PRIORITY 1
#define VG_M_PRE_Z_TEXTURE_BINDING_PRIORITY 2
#define VG_M_POST_RENDER_TEXTURE_BINDING_PRIORITY 3
#define VG_M_OTHER_MAX_BINDING_PRIORITY 4

namespace vg
{
    struct PassData
	{
		struct TexData {
			const Texture *pTexture;
			const Texture::ImageView *pImageView;
			const Texture::Sampler *pSampler;
			vk::ImageLayout imageLayout;

			TexData(const Texture *pTexture = nullptr
			    , const Texture::ImageView *pImageView = nullptr
				, const Texture::Sampler *pSampler = nullptr
				, vk::ImageLayout imageLayout = vk::ImageLayout::eUndefined
				);

			TexData(const TexData &);

			TexData &operator=(const TexData &);
		};
		
		std::vector<std::string> arrDataNames;
		std::unordered_map<std::string, std::vector<Byte>> mapDatas;
		std::unordered_map<std::string, uint32_t> mapDataCounts;
		std::vector<std::string> arrTexNames;
		std::unordered_map<std::string, TexData> mapTextures;

		TexData getTexture(std::string name) const;
		void setTexture(std::string name, TexData texData);

		void getDataValue(const std::string name, void *dst, uint32_t size, uint32_t offset) const;

		template<typename T>
		T getDataValue(const std::string name) const;

		template <typename T>
		std::vector<T> getDataValue(const std::string name, const uint32_t count) const;

		void setDataValue(const std::string name, void *src, uint32_t size, uint32_t offset);

		template<typename T>
		void setDataValue(const std::string name, const T &value);

		template<typename T>
		void setDataValue(const std::string name, const std::vector<T> &values);

		template<typename T>
		void setDataValue(const std::string name, const T * const pValue, const uint32_t count);

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