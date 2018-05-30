#include "foundation/wrapper.hpp"

#include <exception>

namespace fd
{
    std::shared_ptr<vk::Instance> createInstance(const vk::InstanceCreateInfo & createInfo,
        vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto instance = vk::createInstance(createInfo, allocator);
        return std::shared_ptr<vk::Instance>(new vk::Instance(instance),
            [](vk::Instance *p) {
            p->destroy();
        });
    }

    std::shared_ptr<vk::Device> createDevice(const const vk::PhysicalDevice *pPhysicalDevice,
        const vk::DeviceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto device = pPhysicalDevice->createDevice(createInfo, allocator);
        return std::shared_ptr<vk::Device>(new vk::Device(device),
            [](vk::Device *p) {
            p->destroy();
        });
    }

    std::shared_ptr<vk::SwapchainKHR> createSwapchainKHR(
        const vk::Device *pDevice,
        const vk::SwapchainCreateInfoKHR & createInfo,
        vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto swapchain = pDevice->createSwapchainKHR(createInfo, allocator);
        return std::shared_ptr<vk::SwapchainKHR>(new vk::SwapchainKHR(swapchain),
            [pDevice](vk::SwapchainKHR *p)
        {
            pDevice->destroySwapchainKHR(*p);
        }
        );
    }


    std::shared_ptr<vk::ImageView> createImageView(const vk::Device *pDevice,
        const vk::ImageViewCreateInfo & createInfo,
        vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto imageView = pDevice->createImageView(createInfo, allocator);
        return std::shared_ptr<vk::ImageView>(new vk::ImageView(imageView),
            [pDevice](vk::ImageView *p)
        {
            pDevice->destroyImageView(*p);
        });
    }

    std::shared_ptr<vk::CommandPool> createCommandPool(const vk::Device *pDevice,
        const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto commandPool = pDevice->createCommandPool(createInfo, allocator);
        return std::shared_ptr<vk::CommandPool>(new vk::CommandPool(commandPool),
            [pDevice](vk::CommandPool *p) {
            pDevice->destroyCommandPool(*p);
        });
    }

    std::shared_ptr<vk::RenderPass> createRenderPass(const vk::Device *pDevice,
        const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto renderPass = pDevice->createRenderPass(createInfo, allocator);
        return std::shared_ptr<vk::RenderPass>(new vk::RenderPass(renderPass),
            [pDevice](vk::RenderPass *p) {
            pDevice->destroyRenderPass(*p);
        });
    }

    std::shared_ptr<vk::Buffer> createBuffer(const vk::Device *pDevice,
        const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto buffer = pDevice->createBuffer(createInfo, allocator);
        return std::shared_ptr<vk::Buffer>(new vk::Buffer(buffer),
            [pDevice](vk::Buffer *p) {
            pDevice->destroyBuffer(*p);
        });
    }

    std::shared_ptr<vk::Image> createImage(const vk::Device *pDevice,
        const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto image = pDevice->createImage(createInfo, allocator);
        return std::shared_ptr<vk::Image>(new vk::Image(image),
            [pDevice](vk::Image *p) {
            pDevice->destroyImage(*p);
        });
    }

    std::shared_ptr<vk::DeviceMemory> allocateMemory(const vk::Device *pDevice,
        const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto memory = pDevice->allocateMemory(allocateInfo, allocator);
        return std::shared_ptr<vk::DeviceMemory>(new vk::DeviceMemory(memory),
            [pDevice](vk::DeviceMemory *p) {
            pDevice->freeMemory(*p);
        });
    }

    std::shared_ptr<vk::Framebuffer> createFrameBuffer(const vk::Device *pDevice,
        const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto frameBuffer = pDevice->createFramebuffer(createInfo, allocator);
        return std::shared_ptr<vk::Framebuffer>(new vk::Framebuffer(frameBuffer),
            [pDevice](vk::Framebuffer *p) {
            pDevice->destroyFramebuffer(*p);
        });
    }

    std::shared_ptr<vk::Semaphore> createSemaphore(const vk::Device *pDevice,
        const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto semaphore = pDevice->createSemaphore(createInfo, allocator);
        return std::shared_ptr<vk::Semaphore>(new vk::Semaphore(semaphore),
            [pDevice](vk::Semaphore *p) {
            pDevice->destroySemaphore(*p);
        });
    }

    std::shared_ptr<vk::CommandBuffer> allocateCommandBuffer(const vk::Device *pDevice,
        const vk::CommandPool *pCommandPool,
        const vk::CommandBufferAllocateInfo & allocateInfo
        )
    {
        //allocate only one command buffer.
        vk::CommandBufferAllocateInfo tempAllocateInfo = allocateInfo;
        tempAllocateInfo.setCommandBufferCount(1U);
        tempAllocateInfo.setCommandPool(*pCommandPool);
        auto commandBuffer = pDevice->allocateCommandBuffers(tempAllocateInfo)[0];
        return std::shared_ptr<vk::CommandBuffer>(new vk::CommandBuffer(commandBuffer),
            [pDevice, pCommandPool](vk::CommandBuffer *p) {
            pDevice->freeCommandBuffers(*pCommandPool, *p);
        });
    }

    std::shared_ptr<vk::Sampler> createSampler(const vk::Device *pDevice,
        const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto sampler = pDevice->createSampler(createInfo, allocator);
        return std::shared_ptr<vk::Sampler>(new vk::Sampler(sampler),
            [pDevice](vk::Sampler *p) {
            pDevice->destroySampler(*p);
        });
    }

    std::shared_ptr<vk::ShaderModule> createShaderModule(const vk::Device *pDevice,
        const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto shaderModule = pDevice->createShaderModule(createInfo, allocator);
        return std::shared_ptr<vk::ShaderModule>(new vk::ShaderModule(shaderModule),
            [pDevice](vk::ShaderModule *p) {
            pDevice->destroyShaderModule(*p);
        });
    }

    std::shared_ptr<vk::DescriptorSetLayout> createDescriptorSetLayout(const vk::Device *pDevice,
        const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto descriptorSetLayout = pDevice->createDescriptorSetLayout(createInfo, allocator);
        return std::shared_ptr<vk::DescriptorSetLayout>(new vk::DescriptorSetLayout(descriptorSetLayout),
            [pDevice](vk::DescriptorSetLayout *p) {
            pDevice->destroyDescriptorSetLayout(*p);
        });
    }

    std::shared_ptr<vk::DescriptorPool> createDescriptorPool(const vk::Device *pDevice,
        const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional < const vk::AllocationCallbacks > allocator)
    {
        auto descriptorPool = pDevice->createDescriptorPool(createInfo, allocator);
        return std::shared_ptr<vk::DescriptorPool>(new vk::DescriptorPool(descriptorPool), 
            [pDevice](vk::DescriptorPool *p) {
            pDevice->destroyDescriptorPool(*p);
        });
    }

    std::shared_ptr<vk::DescriptorSet> allocateDescriptorSet(const vk::Device *pDevice,
        const vk::DescriptorPool *pDescriptorPool, const vk::DescriptorSetAllocateInfo & allocateInfo)
    {
        vk::DescriptorSetAllocateInfo tempAllocInfo = allocateInfo;
        tempAllocInfo.setDescriptorPool(*pDescriptorPool);
        tempAllocInfo.setDescriptorSetCount(1u);
        auto descriptorSet = pDevice->allocateDescriptorSets(tempAllocInfo)[0];
        return std::shared_ptr<vk::DescriptorSet>(new vk::DescriptorSet(descriptorSet), 
            [pDevice, pDescriptorPool](vk::DescriptorSet *p) {
            pDevice->freeDescriptorSets(*pDescriptorPool, *p);
        });
    }

    std::shared_ptr<vk::PipelineLayout> createPipelineLayout(const vk::Device *pDevice,
        const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto pipelineLayout = pDevice->createPipelineLayout(createInfo, allocator);
        return std::shared_ptr<vk::PipelineLayout>(new vk::PipelineLayout(pipelineLayout),
            [pDevice](vk::PipelineLayout *p) {
            pDevice->destroyPipelineLayout(*p);
        });
    }

    std::shared_ptr<vk::Pipeline> createGraphicsPipeline(const vk::Device *pDevice,
        const vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo,
        vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto pipeline = pDevice->createGraphicsPipeline(pipelineCache, createInfo, allocator);
        return std::shared_ptr<vk::Pipeline>(new vk::Pipeline(pipeline),
            [pDevice](vk::Pipeline *p) {
            pDevice->destroyPipeline(*p);
        });
    }

    std::shared_ptr<vk::PipelineCache> createPipelineCache(const vk::Device *pDevice,
        const vk::PipelineCacheCreateInfo &createInfo,
        vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto pipelineCache = pDevice->createPipelineCache(createInfo, allocator);
        return std::shared_ptr<vk::PipelineCache>(new vk::PipelineCache(pipelineCache),
            [pDevice](vk::PipelineCache *p) {
                pDevice->destroyPipelineCache(*p);
        });
    }

    std::shared_ptr<vk::Fence> createFence(const vk::Device *pDevice,
        const vk::FenceCreateInfo &createInfo, vk::Optional<const vk::AllocationCallbacks> allocator)
    {
        auto fence = pDevice->createFence(createInfo, allocator);
        return std::shared_ptr<vk::Fence>(new vk::Fence(fence),
            [pDevice](vk::Fence *p) {
            pDevice->destroyFence(*p);
        });
    }
}
