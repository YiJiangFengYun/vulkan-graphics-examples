#ifndef VG_ATTACHMENT_HPP
#define VG_ATTACHMENT_HPP

#include "graphics/global.hpp"

namespace vg
{
    class BaseColorAttachment
	{
	public:
	    BaseColorAttachment();
	    virtual ~BaseColorAttachment();
	    virtual uint32_t getColorAttachmentWidth() const = 0;
		virtual uint32_t getColorAttachmentHeight() const =  0;
		virtual uint32_t getColorAttachmentLayers() const = 0;
		virtual vk::Format getColorAttachmentFormat() const = 0;
		virtual vk::ImageLayout getColorAttachmentLayout() const = 0;
		virtual vk::ImageView *getColorAttachmentImageView() const = 0;

	protected:
	};

	class BaseDepthStencilAttachment
	{
	public:
	    BaseDepthStencilAttachment();
	    virtual ~BaseDepthStencilAttachment();
	    virtual uint32_t getDepthStencilAttachmentWidth() const = 0;
		virtual uint32_t getDepthStencilAttachmentHeight() const =  0;
		virtual uint32_t getDepthStencilAttachmentLayers() const = 0;
		virtual vk::Format getDepthStencilAttachmentFormat() const = 0;
		virtual vk::ImageLayout getDepthStencilAttachmentLayout() const = 0;
		virtual vk::ImageView *getDepthStencilAttachmentImageView() const = 0;
	protected:
	};
} //vg

#endif //VG_ATTACHMENT_HPP