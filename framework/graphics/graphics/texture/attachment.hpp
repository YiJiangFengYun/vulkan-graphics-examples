#ifndef VG_ATTACHMENT_HPP
#define VG_ATTACHMENT_HPP

#include "graphics/global.hpp"

namespace vg
{
    class BaseColorAttachment
	{
	public:
	    BaseColorAttachment(Bool32 isInputUsage);
	    virtual ~BaseColorAttachment();
	    virtual uint32_t getColorAttachmentWidth() const = 0;
		virtual uint32_t getColorAttachmentHeight() const =  0;
		virtual uint32_t getColorAttachmentLayers() const = 0;
		virtual const vk::Format getColorAttachmentFormat() const = 0;
		virtual const vk::ImageLayout getColorAttachmentLayout() const = 0;
		virtual const vk::ImageView *getColorAttachmentImageView() const = 0;

	protected:
	    Bool32 m_isInputUsage;
	};

	class BaseDepthStencilAttachment
	{
	public:
	    BaseDepthStencilAttachment(Bool32 isInputUsage);
	    virtual ~BaseDepthStencilAttachment();
	    virtual uint32_t getDepthStencilAttachmentWidth() const = 0;
		virtual uint32_t getDepthStencilAttachmentHeight() const =  0;
		virtual uint32_t getDepthStencilAttachmentLayers() const = 0;
		virtual const vk::Format getDepthStencilAttachmentFormat() const = 0;
		virtual const vk::ImageLayout getDepthStencilAttachmentLayout() const = 0;
		virtual const vk::ImageView *getDepthStencilAttachmentImageView() const = 0;
	protected:
	    Bool32 m_isInputUsage;
	};
} //vg

#endif //VG_ATTACHMENT_HPP