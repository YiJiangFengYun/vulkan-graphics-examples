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

		Ray()
		{

		}

		Ray(ValueType origin, ValueType direction):
			m_origin(origin), m_direction(direction), m_isUpdateCache(FD_TRUE)
		{
#ifdef DEBUG
			vec_value_type distSqr = glm::dot(direction, direction);
			if (distSqr == 0)
				throw std::invalid_argument("Invalid direction, direction length is 0.");
#endif // DEBUG
		}

		Ray(const Ray<ValueType>& target) :
			m_origin(target.m_origin),
			m_direction(target.m_direction),
			m_isUpdateCache(target.m_isUpdateCache),
			m_invDir(target.m_invDir),
			m_signs(target.m_signs)
		{
		}

		Ray& operator =(const Ray<ValueType>& target)
		{
			m_origin = target.m_origin;
			m_direction = target.m_direction;
			m_isUpdateCache = target.m_isUpdateCache;
			m_invDir = target.m_invDir;
			m_signs = target.m_signs;
			return *this;
		}

		ValueType getOrigin()
		{
			return m_origin;
		}

		void setOrigin(ValueType origin)
		{
			m_origin = origin;
			m_isUpdateCache = true;
		}

		ValueType getDirection()
		{
			return m_direction;
		}

		void setDirection(ValueType direction)
		{
			m_direction = direction;
			m_isUpdateCache = true;
		}

		ValueType getInvDir()
		{
			_updateCache();
			return m_invDir;
		}

		ValueType getSigns()
		{
			_updateCache();
			return m_signs;
		}

		/*Returns a point at distance units along the ray.*/
		ValueType getPoint(vec_value_type distance)
		{
			ValueType direction = m_direction;
			glm::normalize(direction);
			direction *= distance;
			direction += m_origin;
			return direction;
		}


	private:
		ValueType m_origin;
		ValueType m_direction;

		//cache
		Bool32 m_isUpdateCache;
		ValueType m_invDir;
		ValueType m_signs;

		inline void _updateCache()
		{
			if (m_isUpdateCache)
			{
				m_isUpdateCache = FD_FALSE;
				length_type length = ValueType::length();
				for (length_type i = 0; i < length; ++i)
				{
					m_invDir[i] = 1 / m_direction[i];
					//negative is 0 and positive is 1, it is better to be used as array index.
					m_signs[i] = m_direction[i] < 0 ? 0 : 1;
				}
			}
		}
	};

	template <typename VecType = glm::vec3>
	class Bounds
	{
	public:
		typedef VecType ValueType;
		typedef typename ValueType::length_type length_type;
		typedef typename ValueType::value_type vec_value_type;
		Bounds()
		{
			_updateSize();
		}

		Bounds(ValueType min, ValueType max):
			m_min(min), m_max(max)
		{
			_updateSize();
		}

		Bounds(const Bounds<ValueType>& target):
			m_min(target.m_min),
			m_max(target.m_max),
			m_size(target.m_size)
		{
		}

		Bounds& operator =(const Bounds<ValueType>& target)
		{
			m_min = target.m_min;
			m_max = target.m_max;
			m_size = target.m_size;
			return *this;
		}

		ValueType getMin() { return m_min; }
		void setMin(ValueType value)
		{
			m_min = value;
			_updateSize();
		}

		ValueType getMax() { return m_max; }
		void setMax(ValueType value)
		{
			m_max = value;
			_updateSize();
		}

		/* Sets the bounds to the min and max value of the box.
           Using this function is faster than assigning min and max separately.*/
		void setMinMax(ValueType min, ValueType max)
		{
			m_min = min;
			m_max = max;
			_updateSize();
		}

		ValueType getSize() { return m_size; }
		void setSize(ValueType size)
		{
#ifdef DEBUG
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				if (size[i] < 0)throw std::invalid_argument("Invalid size of Bounds");
			}
#endif // DEBUG
			m_size = size;
			_updateMax();
		}

		/* The closest point on the bounding box.
           If the point is inside the bounding box, unmodified point position will be returned.
		*/
		ValueType getClosestPoint(ValueType point)
		{
			length_type length = ValueType::length();
			ValueType result;
			for (length_type i = 0; i < length; ++i)
			{
				if (point[i] < m_min[i]) result[i] = m_min[i];
				else if (point[i] < m_max[i]) result[i] = point[i];
				else result[i] = m_max[i];
			}
			return result;
		}

		/*Is point contained in the bounding box?
          If the point passed into Contains is inside the bounding box a value of True is returned.*/
		Bool32 isContains(ValueType point)
		{
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				if (point[i] < m_min[i] || point[i] > m_max[i])
					return false;
			}
			return true;
		}

		/*Expand the bounds by increasing its size by amount along each side.*/
		void expand(vec_value_type amount)
		{
			vec_value_type halt = amount / 2;
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_min[i] -= halt;
				m_max[i] += halt;
				m_size[i] += amount;
			}
		}

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
		Bool32 isIntersects(Bounds<ValueType> bounds)
		{
			vec_value_type length = ValueType::length();
			for (vec_value_type i = 0; i < length; ++i)
			{
				if (m_min[i] > bounds.m_max[i] || m_max[i] < bounds.m_min[i])
					return false;
			}
			return true;
		}

		/*The smallest squared distance between the point and this bounding box.*/
		vec_value_type getSqrDistance(ValueType point)
		{
			ValueType closestPoint = getClosestPoint(point);
			closestPoint -= point;
			return glm::dot(closestPoint, closestPoint);
		}

		~Bounds()
		{

		}

	private:
		ValueType m_min;
		ValueType m_max;
		ValueType m_size;

		inline void _updateSize()
		{
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_size[i] = m_max[i] - m_min[i];
			}
#ifdef DEBUG
			for (length_type i = 0; i < length; ++i)
			{
				if (m_size[i] < 0)
					throw std::invalid_argument("Invalid size of Bounds");
			}
#endif // DEBUG
		}

		inline void _updateMax()
		{
			length_type length = ValueType::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_max[i] = m_min[i] + m_size[i];
			}
		}
	};
}

#endif // !FD_GEMO_H
