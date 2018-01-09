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

		ValueType getOrigin();

		void setOrigin(ValueType origin);

		ValueType getDirection();

		void setDirection(ValueType direction);

		ValueType getInvDir();

		ValueType getSigns();

		/*Returns a point at distance units along the ray.*/
		ValueType getPoint(vec_value_type distance);


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

		Bounds(ValueType min, ValueType max);

		Bounds(const Bounds<ValueType>& target);

		Bounds& operator =(const Bounds<ValueType>& target);

		ValueType getMin();

		ValueType getMax();

		/* Sets the bounds to the min and max value of the box.
           Using this function is faster than assigning min and max separately.*/
		void setMinMax(ValueType min, ValueType max);

		ValueType getSize();
		void setSize(ValueType size);

		/* The closest point on the bounding box.
           If the point is inside the bounding box, unmodified point position will be returned.
		*/
		ValueType getClosestPoint(ValueType point);

		/*Is point contained in the bounding box?
          If the point passed into Contains is inside the bounding box a value of True is returned.*/
		Bool32 isContains(ValueType point);

		/*Expand the bounds by increasing its size by amount along each side.*/
		void expand(vec_value_type amount);

		//todo
		/*Does ray intersect this bounding box?
          if true, return the distance to the ray's origin will be returned,
          or else return number -1.*/
		//vec_value_type intersectRay(Ray<ValueType> ray)
		//{
		//	////reference:
		//	//// http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
		//	vec_value_type tMin, tMax, tYMin, tYMax, tZMin, tZMax;
		//	ValueType origin = ray.getOrigin();
		//	ValueType dir = ray.getDirection();
		//	ValueType invDir = ray.getInvDir();
		//	ValueType signs = ray.getSigns();
		//	vec_value_type Epsilon = std::numeric_limits<vec_value_type>::epsilon();
		//	ValueType minMax[2] = { m_min, m_max };
		//	if (glm::abs(dir[0]) > Epsilon)
		//	{
		//		tMin = (minMax[static_cast<size_t>(1 - signs[0])][0] - origin[0]) * invDir[0];
		//		tMax = (minMax[static_cast<size_t>(signs[0])][0] - origin[0]) * invDir[0];

		//		if (tMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
		//	}
		//	else
		//	{
		//		if (dir[0] < minMax[0][0] || dir[0] > minMax[1][0]) return -1;

		//		tMin = - std::numeric_limits<vec_value_type>::max();
		//		tMax = std::numeric_limits<vec_value_type>::max();
		//	}

		//	if (glm::abs(dir[1]) > Epsilon)
		//	{
		//		tYMin = (minMax[static_cast<size_t>(1 - signs[1])][1] - origin[1]) * invDir[1];
		//		tYMax = (minMax[static_cast<size_t>(signs[1])][1] - origin[1]) * invDir[1];

		//		if (tYMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
		//	}
		//	else
		//	{
		//		if (dir[1] < minMax[0][1] || dir[1] > minMax[1][1]) return -1;

		//		tYMin = - std::numeric_limits<vec_value_type>::max();
		//		tYMax = std::numeric_limits<vec_value_type>::max();
		//	}

		//	if ((tMin > tYMax) || (tYMin > tMax))
		//	{
		//		return -1;
		//	}
		//	if (tYMin > tMin) tMin = tYMin;
		//	if (tYMax < tMax) tMax = tYMax;


		//	if (glm::abs(dir[2]) > Epsilon)
		//	{
		//		tZMin = (minMax[static_cast<size_t>(1 - signs[2])][2] - origin[2]) * invDir[2];
		//		tZMax = (minMax[static_cast<size_t>(signs[2])][2] - origin[2]) * invDir[2];

		//		if (tZMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
		//	}
		//	else
		//	{
		//		if (dir[2] < minMax[0][2] || dir[2] > minMax[1][2]) return -1;

		//		tZMin = - std::numeric_limits<vec_value_type>::max();
		//		tZMax = std::numeric_limits<vec_value_type>::max();
		//	}

		//	if ((tMin > tZMax) || (tZMin > tMax))
		//	{
		//		return -1;
		//	}
		//	if (tZMin > tMin) tMin = tZMin;
		//	if (tZMax < tMax) tMax = tZMax;

		//	//            if(tMin == std::numeric_limits<vec_value_type>::min() || 
		//	//                 tMax == std::numeric_limits<vec_value_type>::max()) return -1; // it is not possible.

		//	//calculate distance.
		//	vec_value_type d;
		//	if (tMin < 0)
		//	{
		//		d = tMax;
		//	}
		//	else
		//	{
		//		d = tMin;
		//	}

		//	return d * glm::length(dir);
		//}

		/*Does another bounding box intersect with this bounding box?*/
		Bool32 isIntersects(Bounds<ValueType> bounds);

		/*The smallest squared distance between the point and this bounding box.*/
		vec_value_type getSqrDistance(ValueType point);

		~Bounds();

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

		Viewport& setX(float x_);

		Viewport& setY(float y_);

		Viewport& setWidth(float width_);

		Viewport& setHeight(float height_);

		Viewport& setMinDepth(float minDepth_);

		Viewport& setMaxDepth(float maxDepth_);

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

		Rect2D& setX(float x_);

		Rect2D& setY(float y_);

		Rect2D& setWidth(float width_);

		Rect2D& setHeight(float height_);

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
