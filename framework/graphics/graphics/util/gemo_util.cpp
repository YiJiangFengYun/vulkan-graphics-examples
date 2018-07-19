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

    Matrix4x4 lookAtRH
	(
		const Vector3 &eye,
		const Vector3 &center,
		const Vector3 &up
	)
	{
		const Vector3 f(normalize(center - eye));
		const Vector3 s(normalize(cross(f, up)));
		const Vector3 u(cross(s, f));

		Matrix4x4 Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = f.x;
		Result[1][1] = f.y;
		Result[2][1] = f.z;
		Result[0][2] = u.x;
		Result[1][2] = u.y;
		Result[2][2] = u.z;
		Result[3][0] =-dot(s, eye);
		Result[3][1] =-dot(f, eye);
		Result[3][2] =-dot(u, eye);
		return Result;
	}

	Matrix4x4 lookAtLH
	(
		const Vector3 &eye,
		const Vector3 &center,
		const Vector3 &up
	)
	{
		const Vector3 f(normalize(center - eye));
		const Vector3 s(normalize(cross(up, f)));
		const Vector3 u(cross(f, s));

		Matrix4x4 Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = f.x;
		Result[1][2] = f.y;
		Result[2][2] = f.z;
		Result[3][0] = -dot(s, eye);
		Result[3][1] = -dot(u, eye);
		Result[3][2] = -dot(f, eye);
		return Result;
	}
} //vg