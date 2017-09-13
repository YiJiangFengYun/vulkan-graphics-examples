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
		typedef VecType value_type;
		typedef value_type::length_type length_type;
		typedef value_type::value_type vec_value_type;

		Ray()
		{

		}

		Ray(value_type origin, value_type direction):
			m_origin(origin), m_direction(direction), m_isUpdateCache(FD_TRUE)
		{
#ifdef DEBUG
			vec_value_type distSqr = glm::dot(direction, direction);
			if (distSqr == 0)
				throw std::invalid_argument("Invalid direction, direction length is 0.");
#endif // DEBUG
		}

		Ray(const Ray<VecType>& target) :
			m_origin(target.m_origin),
			m_direction(target.m_direction),
			m_isUpdateCache(target.m_isUpdateCache),
			m_invDir(target.m_invDir),
			m_signs(target.m_signs)
		{
		}

		Ray& operator =(const Ray<VecType>& target)
		{
			m_origin = target.m_origin;
			m_direction = target.m_direction;
			m_isUpdateCache = target.m_isUpdateCache;
			m_invDir = target.m_invDir;
			m_signs = target.m_signs;
			return *this;
		}

		value_type getOrigin()
		{
			return m_origin;
		}

		void setOrigin(value_type origin)
		{
			m_origin = origin;
			m_isUpdateCache = true;
		}

		value_type getDirection()
		{
			return m_direction;
		}

		void setDirection(value_type direction)
		{
			m_direction = direction;
			m_isUpdateCache = true;
		}

		value_type getInvDir()
		{
			_updateCache();
			return m_invDir;
		}

		value_type getSigns()
		{
			_updateCache();
			return m_signs;
		}

		/*Returns a point at distance units along the ray.*/
		value_type getPoint(vec_value_type distance)
		{
			value_type direction = m_direction;
			glm::normalize(direction);
			direction *= distance;
			direction += m_origin;
			return direction;
		}


	private:
		value_type m_origin;
		value_type m_direction;

		//cache
		Bool32 m_isUpdateCache;
		value_type m_invDir;
		value_type m_signs;

		inline _updateCache()
		{
			if (m_isUpdateCache)
			{
				m_isUpdateCache = FD_FALSE;
				length_type length = value_type::length();
				for (length_type i = 0; i < length_type; ++i)
				{
					m_invDir[i] = 1 / m_direction[i];
					//negative is 0 and positive is 1, it is better to be used as array index.
					m_signs[i] = m_direction[i] < 0 ? 0 : 1;
				}
			}
		}
	};

	template <typename VecType = glm::vec3>
	class Bound
	{
	public:
		typedef VecType value_type;
		typedef value_type::length_type length_type;
		typedef value_type::value_type vec_value_type;
		Bound()
		{
			_updateSize();
		}

		Bound(value_type min, value_type max):
			m_min(min), m_max(max)
		{
			_updateSize();
		}

		Bound(const Bound<VecType>& target):
			m_min(target.m_min),
			m_max(target.m_max),
			m_size(target.m_size)
		{
		}

		Bound& operator =(const Bound<VecType>& target)
		{
			m_min = target.m_min;
			m_max = target.m_max;
			m_size = target.m_size;
			return *this;
		}

		value_type getMin() { return m_min; }
		void setMin(value_type value)
		{
			m_min = value;
			_updateSize();
		}

		value_type getMax() { return m_max; }
		void setMax(value_type value)
		{
			m_max = value;
			_updateSize();
		}

		void setMinMax(value_type min, value_type max)
		{
			m_min = min;
			m_max = max;
			_updateSize();
		}

		value_type getSize() { return m_size; }
		void setSize(value_type size)
		{
#ifdef DEBUG
			length_type length = value_type::length();
			for (length_type i = 0; i < length; ++i)
			{
				if (size[i] < 0)throw std::invalid_argument("Invalid size of Bounds");
			}
#endif // DEBUG
			m_size = size;
			_updateMax();
		}

		
		value_type getClosestPoint(value_type point)
		{
			length_type length = value_type::length();
			value_type result;
			for (length_type i = 0; i < length; ++i)
			{
				if (point[i] < m_min[i]) result[i] = m_min[i];
				else if (point[i] < m_max[i]) result[i] = point[i];
				else result[i] = m_max[i];
			}
			return result;
		}

		Bool32 isContains(value_type point)
		{
			length_type length = value_type::length();
			for (length_type i = 0; i < length; ++i)
			{
				if (point[i] < m_min[i] || point[i] > m_max[i])
					return false;
			}
			return true;
		}

		void expand(value_type::value_type amount)
		{
			vec_value_type halt = amount / 2;
			length_type length = value_type::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_min[i] -= halt;
				m_max[i] += halt;
				m_size[i] += amount;
			}
		}

		vec_value_type intersectRay(Ray<VecType> ray)
		{
			////reference:
			//// http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
			//VecType origin = ray.getOrigin();
			//VecType dir = ray.getDirection();
			//VecType invDir = ray.getInvDir();
			//VecType signs = ray.getSigns();
			//var minMaxValues : Vector.<Vector.<Number>> = m_minMaxValuesContainerHelper;
			//if (minMaxValues == null)
			//{
			//	m_minMaxValuesContainerHelper = minMaxValues = new Vector.<Vector.<Number>>(2);
			//	minMaxValues.fixed = true;
			//}
			//minMaxValues[0] = minMax[0].values;
			//minMaxValues[1] = minMax[1].values;

			//var origin_values : Vector.<Number> = origin.values;
			//var dir_values : Vector.<Number> = dir.values;
			//var inDir_values : Vector.<Number> = inDir.values;

			//var default_precision : Number = Geom.default_precision;
			//if (Math.abs(dir_values[0]) > default_precision)
			//{
			//	tMin = (minMaxValues[1 - signs[0]][0] - origin_values[0]) * inDir_values[0];
			//	tMax = (minMaxValues[signs[0]][0] - origin_values[0]) * inDir_values[0];

			//	if (tMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
			//}
			//else
			//{
			//	if (dir_values[0] < minMaxValues[0][0] || dir_values[0] > minMaxValues[1][0]) return -1;

			//	tMin = Number.MIN_VALUE;
			//	tMax = Number.MAX_VALUE;
			//}

			//if (Math.abs(dir_values[1]) > default_precision)
			//{
			//	tYMin = (minMaxValues[1 - signs[1]][1] - origin_values[1]) * inDir_values[1];
			//	tYMax = (minMaxValues[signs[1]][1] - origin_values[1]) * inDir_values[1];

			//	if (tYMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
			//}
			//else
			//{
			//	if (dir_values[1] < minMaxValues[0][1] || dir_values[1] > minMaxValues[1][1]) return -1;

			//	tYMin = Number.MIN_VALUE;
			//	tYMax = Number.MAX_VALUE;
			//}

			//if ((tMin > tYMax) || (tYMin > tMax))
			//{
			//	return -1;
			//}
			//if (tYMin > tMin) tMin = tYMin;
			//if (tYMax < tMax) tMax = tYMax;


			//if (Math.abs(dir_values[2]) > default_precision)
			//{
			//	tZMin = (minMaxValues[1 - signs[2]][2] - origin_values[2]) * inDir_values[2];
			//	tZMax = (minMaxValues[signs[2]][2] - origin_values[2]) * inDir_values[2];

			//	if (tZMax < 0) return -1; //ray is only positive direction. if tMax < 0, tMax and tMin < 0.
			//}
			//else
			//{
			//	if (dir_values[2] < minMaxValues[0][2] || dir_values[2] > minMaxValues[1][2]) return -1;

			//	tZMin = Number.MIN_VALUE;
			//	tZMax = Number.MAX_VALUE;
			//}

			//if ((tMin > tZMax) || (tZMin > tMax))
			//{
			//	return -1;
			//}
			//if (tZMin > tMin) tMin = tZMin;
			//if (tZMax < tMax) tMax = tZMax;

			////            if(tMin == Number.MIN_VALUE || tMax == Number.MAX_VALUE) return -1; // it is not possible.

			////calculate distance.
			//var d : Number;
			//if (tMin < 0)
			//{
			//	d = tMax;
			//}
			//else
			//{
			//	d = tMin;
			//}

			//return d * dir.magnitude;
		}

		Bool32 isIntersects(Bound<VecType> bounds)
		{
			vec_value_type length = VecType::length();
			for (vec_value_type i = 0; i < length; ++i)
			{
				if (m_min[i] > bounds.m_max[i] || m_max[i] < bounds.m_min[i])
					return false;
			}
			return true;
		}

		vec_value_type getSqrDistance(VecType point)
		{
			VecType closestPoint = getClosestPoint(point);
			closestPoint -= point;
			return glm::dot(closestPoint, closestPoint);
		}

		~Bound()
		{

		}

	private:
		value_type m_min;
		value_type m_max;
		value_type m_size;

		inline void _updateSize()
		{
			length_type length = value_type::length();
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
			length_type length = value_type::length();
			for (length_type i = 0; i < length; ++i)
			{
				m_max[i] = m_min[i] + m_size[i];
			}
		}
	};
}

#endif // !FD_GEMO_H
