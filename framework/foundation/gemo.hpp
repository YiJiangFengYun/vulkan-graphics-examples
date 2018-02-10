#ifndef FD_GEMO_H
#define FD_GEMO_H

#include <stdexcept>
#include <glm/glm.hpp>
#include "foundation/global.hpp"

namespace fd
{
	/*Representation of rays. A ray is an infinite line starting at origin and going in some direction.*/
	template <typename VecType = glm::vec3>
	class Ray
	{
	public:
		typedef VecType ValueType;
		typedef typename ValueType::length_type length_type;
		typedef typename ValueType::value_type vec_value_type;

		Ray();

		Ray(ValueType origin, ValueType direction);

		Ray(const Ray<ValueType>& target);

		Ray& operator =(const Ray<ValueType>& target);

		ValueType getOrigin() const;

		void setOrigin(ValueType origin);

		ValueType getDirection() const;

		void setDirection(ValueType direction);

		ValueType getInvDir() const;

		ValueType getSigns() const;

		/*Returns a point at distance units along the ray.*/
		ValueType getPoint(vec_value_type distance) const;


	private:
		ValueType m_origin;
		ValueType m_direction;

		//cache
		Bool32 m_isUpdateCache;
		ValueType m_invDir;
		ValueType m_signs;

		inline void _updateCache();
	};

	template <typename VecType = glm::vec3>
	class Bounds
	{
	public:
		typedef VecType ValueType;
		typedef typename ValueType::length_type length_type;
		typedef typename ValueType::value_type vec_value_type;
		Bounds();

		~Bounds();		

		Bounds(ValueType min, ValueType max);

		Bounds(const Bounds<ValueType>& target);

		Bounds& operator =(const Bounds<ValueType>& target);

		ValueType getMin() const;

		ValueType getMax() const;

		/* Sets the bounds to the min and max value of the box.
           Using this function is faster than assigning min and max separately.*/
		void setMinMax(ValueType min, ValueType max);

		ValueType getSize();
		void setSize(ValueType size);

		/* The closest point on the bounding box.
           If the point is inside the bounding box, unmodified point position will be returned.
		*/
		ValueType getClosestPoint(ValueType point) const;

		/*Is point contained in the bounding box?
          If the point passed into Contains is inside the bounding box a value of True is returned.*/
		Bool32 isContains(ValueType point) const;

		/*Expand the bounds by increasing its size by amount along each side.*/
		void expand(vec_value_type amount);

		/*Does ray intersect this bounding box?
          if true, return the distance to the ray's origin will be returned,
          or else return number -1.*/
		vec_value_type intersectRay(Ray<ValueType> ray, Bool32 isOnlyForward = true) const;

		/*Does another bounding box intersect with this bounding box?*/
		Bool32 isIntersects(Bounds<ValueType> bounds) const;

		/*Get bounds of intersection of this bounding box and target bounding box*/
		Bool32 intersects(Bounds<ValueType> bounds, Bounds<ValueType> *intersection) const;

		/*The smallest squared distance between the point and this bounding box.*/
		vec_value_type getSqrDistance(ValueType point) const;

	private:
		ValueType m_min;
		ValueType m_max;
		ValueType m_size;

		inline void _updateSize();
		inline void _updateMax();
	};

	struct Viewport
	{
		Viewport(float x_ = 0, float y_ = 0, float width_ = 1, float height_ = 1, float minDepth_ = 0, float maxDepth_ = 1);
		Viewport(const Viewport &target);		
		Viewport(const Viewport &&target);		
		
		Viewport& setX(float x_);

		Viewport& setY(float y_);

		Viewport& setWidth(float width_);

		Viewport& setHeight(float height_);

		Viewport& setMinDepth(float minDepth_);

		Viewport& setMaxDepth(float maxDepth_);

		Viewport &operator=(const Viewport &target);

		bool operator==(Viewport const& rhs) const;

		bool operator!=(Viewport const& rhs) const;

		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;
	};


	struct Rect2D
	{
		Rect2D(float x_ = 0, float y_ = 0, float width_ = 1, float height_ = 1);
		Rect2D(const Rect2D &target);		
		Rect2D(const Rect2D &&target);		

		Rect2D& setX(float x_);

		Rect2D& setY(float y_);

		Rect2D& setWidth(float width_);

		Rect2D& setHeight(float height_);

		Rect2D &operator=(const Rect2D &target);
		
		bool operator==(Rect2D const& rhs) const;

		bool operator!=(Rect2D const& rhs) const;

		float x;
		float y;
		float width;
		float height;
	};
}

#include "foundation/gemo.inl"

#endif // !FD_GEMO_H
