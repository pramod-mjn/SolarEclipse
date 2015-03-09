#pragma once
#include <texture.h>
#include <Pixel.h>
#include <Vector.h>
#include <Line.h>
#include <Transform.h>
#include <Matrix.h>
#include <Camera.h>
#include <cmath>

class Rasterize
{
public:
	Rasterize(SDL_Renderer *renderer, Camera &camera);
	Vec3 Projection(const Vec4& a);
	void SortY(Vec3&, Vec3&, Vec3&);
	void SortY(Vec3& a, Vec3& b, Vec3& c, Vec2& uv1, Vec2& uv2, Vec2& uv3);
	Vec3 SurNormal(Vec4, Vec4, Vec4);
	float IntersectX(int y, Vec3 a, Vec3 b);
	int orient2d(const Vec3& a, const Vec3& b, const Vec3& c);
	int Max3(int a, int b, int c);
	int Min3(int a, int b, int c);

	void Horizon(float y, float preX, float postX, float *zBuffer);
	void Horizon(float y, float v2sam, float preX, float postX, float u1, float u2, 
  Texture &texEarth, float *zBuffer);

	void DrawTriangle(const Vec4& p1, const Vec4& p2, const Vec4& p3, ColorRGB color);
	void DrawSphere(Vec4 center, int radius, ColorRGB color);
	void FillSphere(Vec4 center, int radius, Texture& tex, float *zBUffer);
	

	void FillTriangle(Vec4 p1, Vec4 p2, Vec4 p3, ColorRGB color, float *zBuffer);
	void FillTriangle(Vec4 p1, Vec4 p2, Vec4 p3, Vec2 uv1, Vec2 uv2, Vec2 uv3,
       Texture &texEarth, float *zBuffer);

	float Vinterpolation(float y, float y1, float y2, float v1, float v2)
	{
	  return (v1 + (v2-v1)*(y - y1)/(y2 - y1)*1.0f);
	}
	float Uinterpolation(float x, float x1, float x2, float u1, float u2)
	{
	  if(x1 == x2)
	    return u1;
	  else
	    return (u1 + (u2-u1)*(x - x1)/(x2 - x1)*1.0f);
	}

private:
	SDL_Renderer *m_renderer;
	ColorRGB m_color;
	Camera cam;
	//float zprp;
	//float zview;
	Vec4 m_p[3];
	Vec3 p_p[3];
	Vec4 m_p1, m_p2, m_p3;
	Vec3 p_p1, p_p2, p_p3;
	float consA, consB, consC, consD;
};

