#ifndef VG_GEMO_UTIL_HPP
#define VG_GEMO_UTIL_HPP

#include "graphics/global.hpp"

namespace vg
{
    extern Matrix4x4 tranMat3ToMat4(Matrix3x3 srcMat);

    template <typename PointType>
    fd::Bounds<typename PointType> tranBoundsToNewSpace(
        fd::Bounds<typename PointType> bounds, 
		typename PointTypeMapInfo<PointType>::MatrixType matrix,
        Bool32 isProjective)
    {
        auto min = bounds.getMin();
		auto max = bounds.getMax();
		using MatrixType = typename PointTypeMapInfo<PointType>::MatrixType;
		using MatrixVectorType = typename PointTypeMapInfo<PointType>::MatrixVectorType;
		typename PointType::length_type len = PointType::length();
		
		//use permutation combination algorithm to get all points of bounds.
		uint32_t pointCount = static_cast<uint32_t>(std::pow(2u, len));
		std::vector<MatrixVectorType> points(pointCount);
		uint32_t pointIndex = 0u;
		std::vector<Bool32> places(len);
		for (typename PointType::length_type i = 0; i <= len; ++i)
		{
			std::fill(places.begin(), places.end(), VG_FALSE);
			std::fill(places.begin(), places.begin() + i, VG_TRUE);
			do
			{
				typename PointType::length_type j;
				for (j = 0; j < len; ++j)
				{
					if (places[j] == VG_TRUE)
					{
						points[pointIndex][j] = min[j];
					}
					else
					{
						points[pointIndex][j] = max[j];
					}
				}
				points[pointIndex][j] = 1.0f; //final number is 1.0 for point.
				++pointIndex;
			} while (std::prev_permutation(places.begin(), places.end()));
		}

        const typename PointType::value_type epsilon = std::numeric_limits<typename PointType::value_type>::epsilon();

		for (uint8_t i = 0; i < pointCount; ++i)
		{
			points[i] = matrix * points[i];
			if (isProjective)
			{
				//3 is w
				if(glm::abs(points[i][3]) > epsilon)
			    {
			    	points[i] = points[i] / std::abs(points[i][3]);
			    }
			    else
			    {
			    	typename PointType::length_type j;
			    	for (j = 0; j < len; ++j) {
			    		if (j < len - 1) {
			    			//x y value is infinite.
			    			points[i][j] = std::numeric_limits<typename PointType::value_type>::max();
			    		} else {
			    			// z value is zero.
			    			points[i][j] = 0.0f;
			    		}
			    	}
			    }
			}
		}

		PointType minNew;
		PointType maxNew;

		for (typename PointType::length_type i = 0; i < len; ++i)
		{
			typename PointType::value_type min = std::numeric_limits<typename PointType::value_type>::max(), max = -std::numeric_limits<typename PointType::value_type>::max();
			for (uint8_t j = 0; j < pointCount; ++j)
			{
				if (min > points[j][i])min = points[j][i];
				if (max < points[j][i])max = points[j][i];
			}
			minNew[i] = min;
			maxNew[i] = max;
		}

		fd::Bounds<PointType> newBounds(minNew, maxNew);
		return newBounds;
    }
} //vg

#endif //VG_GEMO_UTIL_HPP