#include "foundation/gemo.hpp"

namespace fd
{
	Viewport::Viewport(float x_
		, float y_
		, float width_
		, float height_
		, float minDepth_
		, float maxDepth_
	)
		: x(x_)
		, y(y_)
		, width(width_)
		, height(height_)
		, minDepth(minDepth_)
		, maxDepth(maxDepth_)
	{
	}

	Viewport& Viewport::setX(float x_)
	{
		x = x_;
		return *this;
	}

	Viewport& Viewport::setY(float y_)
	{
		y = y_;
		return *this;
	}

	Viewport& Viewport::setWidth(float width_)
	{
		width = width_;
		return *this;
	}

	Viewport& Viewport::setHeight(float height_)
	{
		height = height_;
		return *this;
	}

	Viewport& Viewport::setMinDepth(float minDepth_)
	{
		minDepth = minDepth_;
		return *this;
	}

	Viewport& Viewport::setMaxDepth(float maxDepth_)
	{
		maxDepth = maxDepth_;
		return *this;
	}

	bool Viewport::operator==(Viewport const& rhs) const
	{
		return (x == rhs.x)
			&& (y == rhs.y)
			&& (width == rhs.width)
			&& (height == rhs.height)
			&& (minDepth == rhs.minDepth)
			&& (maxDepth == rhs.maxDepth);
	}

	bool Viewport::operator!=(Viewport const& rhs) const
	{
		return !operator==(rhs);
	}


	Rect2D::Rect2D(float x_, float y_, float width_, float height_)
		: x(x_)
		, y(y_)
		, width(width_)
		, height(height_)
	{
	}

	Rect2D& Rect2D::setX(float x_)
	{
		x = x_;
		return *this;
	}

	Rect2D& Rect2D::setY(float y_)
	{
		y = y_;
		return *this;
	}

	Rect2D& Rect2D::setWidth(float width_)
	{
		width = width_;
		return *this;
	}

	Rect2D& Rect2D::setHeight(float height_)
	{
		height = height_;
		return *this;
	}

	bool Rect2D::operator==(Rect2D const& rhs) const
	{
		return (x == rhs.x)
			&& (y == rhs.y)
			&& (width == rhs.width)
			&& (height == rhs.height);
	}

	bool Rect2D::operator!=(Rect2D const& rhs) const
	{
		return !operator==(rhs);
	}
}