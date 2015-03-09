#pragma once


#include <Vector.h>


struct Camera
{
	float zprp, zview, phi, theta, gamma;
	Vec3 lookat;
	Camera() : zprp(-500), zview(-200), phi(0), theta(0), gamma(0) 
	{
		lookat = Vec3(0,0,0);
	}
};