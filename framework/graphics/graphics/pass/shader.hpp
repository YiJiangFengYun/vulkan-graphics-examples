#ifndef VG_SHADER_H
#define VG_SHADER_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.hpp>
#include <foundation/wrapper.hpp>
#include "graphics/global.hpp"
#include "graphics/app/app.hpp"

namespace vg
{
	class Shader : public Base
	{
	public:
		Shader();
		Shader(const std::string &vertShaderPath, const std::string &fragShaderPath);
		Shader(const void *codeVertShader, uint32_t sizeVertShader, const void *codeFragShader, uint32_t sizeFragShader);
		Shader(const uint32_t *codeVertShader, uint32_t sizeVertShader, const uint32_t *codeFragShader, uint32_t sizeFragShader);
		Shader(vk::ShaderModule * pVertShaderModule, vk::ShaderModule * pFragShaderModule);
		~Shader();

		void load(const std::string &vertShaderPath, const std::string &fragShaderPath);
		void load(const void *codeVertShader, uint32_t sizeVertShader, const void *codeFragShader, uint32_t sizeFragShader);
		void load(const uint32_t *codeVertShader, uint32_t sizeVertShader, const uint32_t *codeFragShader, uint32_t sizeFragShader);

		const vk::ShaderModule *getVertShaderModule() const;
		void setVertShaderModule(vk::ShaderModule * pVertShaderModule);
		const vk::ShaderModule *getFragShaderModule() const;
		void setFragShaderModule(vk::ShaderModule * pFragShaderModule);

	    std::vector<vk::PipelineShaderStageCreateInfo> getShaderStageInfos() const;

	private:
	    vk::ShaderModule *m_pVertShaderModule;
		vk::ShaderModule *m_pFragShaderModule;
		//compositions
		std::string m_vertShaderPath;
		std::string m_fragShaderPath;
		std::shared_ptr<vk::ShaderModule> m_pMyVertShaderModule;
		std::shared_ptr<vk::ShaderModule> m_pMyFragShaderModule;

		//aggregations.

		//tool methods
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const std::vector<char>& code);
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const void *code, uint32_t size);
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const uint32_t *code, uint32_t size);
		std::vector<char> _readFile(const std::string& filePath);
	};
}

#endif // !VG_SHADER_H
