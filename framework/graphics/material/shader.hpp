#ifndef VG_SHADER_H
#define VG_SHADER_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.hpp>
#include <foundation/wrapper.hpp>
#include "graphics/global.hpp"
#include "graphics/module.hpp"

namespace vg
{
	class Shader : public Base
	{
	public:
		Shader();
		Shader(const std::string &vertShaderPath, const std::string &fragShaderPath);
		Shader(const void *codeVertShader, uint32_t sizeVertShader, const void *codeFragShader, uint32_t sizeFragShader);
		Shader(const uint32_t *codeVertShader, uint32_t sizeVertShader, const uint32_t *codeFragShader, uint32_t sizeFragShader);
		~Shader();

		void load(const std::string &vertShaderPath, const std::string &fragShaderPath);
		void load(const void *codeVertShader, uint32_t sizeVertShader, const void *codeFragShader, uint32_t sizeFragShader);
		void load(const uint32_t *codeVertShader, uint32_t sizeVertShader, const uint32_t *codeFragShader, uint32_t sizeFragShader);

		std::shared_ptr<vk::ShaderModule> getVertShaderModule();
		std::shared_ptr<vk::ShaderModule> getFragShaderModule();

		std::vector<vk::PipelineShaderStageCreateInfo> getShaderStageInfos() const;

	private:
		//compositions
		uint32_t m_renderQueueId;
		std::shared_ptr<vk::ShaderModule> m_pVertShaderModule;
		std::shared_ptr<vk::ShaderModule> m_pFragShaderModule;

		//aggregations.

		//tool methods
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const std::vector<char>& code);
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const void *code, uint32_t size);
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const uint32_t *code, uint32_t size);
		std::vector<char> _readFile(const std::string& filePath);
	};
}

#endif // !VG_SHADER_H
