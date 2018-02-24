#include "graphics/material/shader.hpp"

namespace vg
{

	Shader::Shader() 
		: Base(BaseType::SHADER)
		, m_pVertShaderModule()
		, m_pFragShaderModule()
	{

	}

	Shader::Shader(const std::string &vertShaderPath, const std::string &fragShaderPath)
		: Base(BaseType::SHADER)
	{
		load(vertShaderPath, fragShaderPath);
	}

	Shader::Shader(const void *codeVertShader, uint32_t sizeVertShader, const void *codeFragShader, uint32_t sizeFragShader)
	    :  Base(BaseType::SHADER)
	{
		load(codeVertShader, sizeVertShader, codeFragShader, sizeFragShader);
	}

	Shader::Shader(const uint32_t *codeVertShader, uint32_t sizeVertShader, const uint32_t *codeFragShader, uint32_t sizeFragShader)
	    :  Base(BaseType::SHADER)
	{
		load(codeVertShader, sizeVertShader, codeFragShader, sizeFragShader);
	}

	Shader::~Shader()
	{
	}

	void Shader::load(const std::string &vertShaderPath, const std::string &fragShaderPath)
	{
		auto vertShaderCode = _readFile(vertShaderPath);
		auto fragShaderCode = _readFile(fragShaderPath);
		m_pVertShaderModule = _createShaderModule(vertShaderCode);
		m_pFragShaderModule = _createShaderModule(fragShaderCode);
	}

	void Shader::load(const void *codeVertShader, uint32_t sizeVertShader, 
	    const void *codeFragShader, uint32_t sizeFragShader)
	{
		m_pVertShaderModule = _createShaderModule(codeVertShader, sizeVertShader);
		m_pFragShaderModule = _createShaderModule(codeFragShader, sizeFragShader);
	}

	void Shader::load(const uint32_t *codeVertShader, uint32_t sizeVertShader, 
	    const uint32_t *codeFragShader, uint32_t sizeFragShader)
	{
		m_pVertShaderModule = _createShaderModule(codeVertShader, sizeVertShader);
		m_pFragShaderModule = _createShaderModule(codeFragShader, sizeFragShader);
	}

	std::shared_ptr<vk::ShaderModule> Shader::getVertShaderModule()
	{
		return m_pVertShaderModule;
	}

	std::shared_ptr<vk::ShaderModule> Shader::getFragShaderModule()
	{
		return m_pFragShaderModule;
	}

	std::vector<vk::PipelineShaderStageCreateInfo> Shader::getShaderStageInfos() const
	{
		uint32_t count = 0u;
		if (m_pVertShaderModule != nullptr)
		{
			++count;
		}
		if (m_pFragShaderModule != nullptr)
		{
			++count;
		}

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages(count);
		count = 0u;
		if (m_pVertShaderModule != nullptr)
		{
			shaderStages[count].flags = vk::PipelineShaderStageCreateFlags();
			shaderStages[count].stage = vk::ShaderStageFlagBits::eVertex;
			shaderStages[count].module = *m_pVertShaderModule;
			shaderStages[count].pName = "main";
			++count;
		}
		if (m_pFragShaderModule != nullptr)
		{
			shaderStages[count].flags = vk::PipelineShaderStageCreateFlags();
			shaderStages[count].stage = vk::ShaderStageFlagBits::eFragment;
			shaderStages[count].module = *m_pFragShaderModule;
			shaderStages[count].pName = "main";
			++count;
		}
		return shaderStages;
	}

	std::shared_ptr<vk::ShaderModule> Shader::_createShaderModule(const std::vector<char>& code)
	{
		auto device = pApp->getDevice();

		std::vector<uint32_t> codeAligned((code.size() - 1) / sizeof(uint32_t) + 1);
		memcpy(codeAligned.data(), code.data(), code.size());

		vk::ShaderModuleCreateInfo createInfo = {
			vk::ShaderModuleCreateFlags(),
			codeAligned.size() * sizeof(uint32_t),
			codeAligned.data()
		};

		return fd::createShaderModule(device, createInfo);
	}

	std::shared_ptr<vk::ShaderModule> Shader::_createShaderModule(const void* code, uint32_t size)
	{
		auto device = pApp->getDevice();

		std::vector<uint32_t> codeAligned((size - 1) / sizeof(uint32_t) + 1);
		memcpy(codeAligned.data(), code, size);

		vk::ShaderModuleCreateInfo createInfo = {
			vk::ShaderModuleCreateFlags(),
			codeAligned.size() * sizeof(uint32_t),
			codeAligned.data()
		};

		return fd::createShaderModule(device, createInfo);
	}

	std::shared_ptr<vk::ShaderModule> Shader::_createShaderModule(const uint32_t* code, uint32_t size)
	{
		auto device = pApp->getDevice();

		vk::ShaderModuleCreateInfo createInfo = {
			vk::ShaderModuleCreateFlags(),
			size,
			code
		};

		return fd::createShaderModule(device, createInfo);
	}



	std::vector<char> Shader::_readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}