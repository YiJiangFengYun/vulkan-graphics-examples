#include "graphics/util/gemo_util.hpp"

namespace vg
{
    Matrix4x4 tranMat3ToMat4(Matrix3x3 srcMat)
	{
		Matrix4x4 resultMat(1.0f);
		resultMat[0] = { srcMat[0][0], srcMat[0][1], 0.0f, srcMat[0][2] };
		resultMat[1] = { srcMat[1][0], srcMat[1][1], 0.0f, srcMat[1][2] };
		resultMat[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		resultMat[3] = { srcMat[2][0], srcMat[2][1], 0.0f, srcMat[2][2] };
		return resultMat;
	}
} //vg