#pragma once

#include <Matrix.h>
#include <Vector.h>
#include <cmath>

class Transform
{
public:
	static Mat4 Translate(const Vec3 & vec)
	{
		return Mat4(
					1, 0, 0, vec.x,
					0, 1, 0, vec.y,
					0, 0, 1, vec.z,
					0, 0, 0, 1
					);
	}
	static Mat4 RotateX(float angle)
	{
		float a=cosf(angle),b=sinf(angle);

		return Mat4(
					1, 0, 0, 0, 
					0, a, -b, 0,
					0, b, a, 0,
					0, 0, 0, 1
					);
	}

	static Mat4 RotateY(float angle)
	{
		float a=cosf(angle),b=sinf(angle);

		return Mat4(
					a, 0, b, 0,
					0, 1, 0, 0,
					-b, 0, a, 0,
					0, 0, 0, 1
					);
	}
	static Mat4 RotateY(float angle, const Vec4& vec)
	{
		float a = cosf(angle), b = sinf(angle);

		return Mat4(a, 0, b, vec.x*(1-a) - b * vec.z,
					0, 1, 0 , 0,
					-b, 0, a, vec.z*(1-a)+b*vec.x,
					0, 0, 0, 1);
	}
 
	static Mat4 RotateZ(float angle)
	{
		float a=cosf(angle),b=sinf(angle);

		return Mat4(
					a, -b, 0, 0,
					b, a, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
					);	
	}
	static Mat4 RotateZ(float angle, const Vec4& vec)
	{
		float a = cosf(angle), b = sinf(angle);

		return Mat4(a, -b, 0, vec.x*(1-a) + b * vec.y,
					b, a, 0 , vec.y*(1-a) - b*vec.x, 	
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	static Mat4 Scale(const Vec3& sc, const Vec3& vec)
	{
		return Mat4(
					sc.x, 0, 0, (1-sc.x)*vec.x,
					0, sc.y, 0, (1-sc.y)*vec.y,
					0, 0, sc.z, (1-sc.z)*vec.z,
					0, 0, 0, 1
					);
	}

	static Mat4 Perspective(float zPersp, float zView)
	{
		float dp = zPersp - zView;
		return Mat4(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, -zView/dp, zView*(zPersp/dp),
				0,0, -1/dp, zPersp/dp
				);
	}

	static Mat4 Orthographic(float right, float left, float top, float bottom, float near, float far)
	{
		return Mat4(
					2/(right-left), 0, 0, -(right+left)/(right-left),
					0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom),
					0, 0, -2/(far-near), (far+near)/(far-near),
					0, 0, 0, 1
					);
	}

};