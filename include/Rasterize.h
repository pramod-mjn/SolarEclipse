#pragma once
#include <texture.h>
#include <Pixel.h>
#include <Vector.h>
#include <Line.h>
#include <Transform.h>
#include <Matrix.h>
#include <Camera.h>
#include <cmath>
#include <Light.h>
//#include <Sphere.h>
//#include <Global.h>


class Rasterize
{
public:
	Rasterize(SDL_Renderer *renderer, Camera &camera);
	Vec3 Projection(const Vec4& a);
	void DrawTriangle(const Vec4& p1, const Vec4& p2, const Vec4& p3, ColorRGB color);
	
	void FillTriangle(Vertex p1, Vertex p2, Vertex p3, Texture &texObj, float *zBuffer);
	void FillTriangle(Vertex p1, Vertex p2, Vertex p3, Texture &texObj, float *zBuffer, int moon);
	void FillTriangle(Vertex p1, Vertex p2, Vertex p3, ColorRGB color, float *zBuffer);

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
	Vec3 Iinterpolation(float xy, float xy1, float xy2, Vec3 i1, Vec3 i2)
	{
		float diffxy = xy2 - xy1;
		if(diffxy == 0)
			return i1;
		Vec3 res;
		res.x = i1.x + (i2.x - i1.x)*(xy - xy1)/diffxy*1.0f;
		res.y = i1.y + (i2.y - i1.y)*(xy - xy1)/diffxy*1.0f;
		res.z = i1.z + (i2.z - i1.z)*(xy - xy1)/diffxy*1.0f;
		return res;
	}
	ColorRGB GetColor(Vec3 a, ColorRGB col)
	{
		ColorRGB res;
		res.R = a.x * col.R;
		res.G = a.y * col.G;
		res.B = a.z * col.B;
		return res;
	}

	void SortVertex(Vertex& v1, Vertex& v2, Vertex& v3)
	{
		if(v1.position.y > v2.position.y){
		    Vec4::swapp(v1.position, v2.position);
		    Vec3::swapp(v1.normal, v2.normal);
		    Vec2::swapp(v1.uvc, v2.uvc);
		    Vec3::swapp(v1.color, v2.color);
	  	}
	  	if(v2.position.y > v3.position.y){
	    	Vec4::swapp(v2.position, v3.position);
		    Vec3::swapp(v2.normal, v3.normal);
		    Vec2::swapp(v2.uvc, v3.uvc);
		    Vec3::swapp(v2.color, v3.color);
	  	}
	  	if(v1.position.y > v2.position.y){
	    	Vec4::swapp(v1.position, v2.position);
		    Vec3::swapp(v1.normal, v2.normal);
		    Vec2::swapp(v1.uvc, v2.uvc);
		    Vec3::swapp(v1.color, v2.color);
  		}
	}
	Vertex Project(Vertex a)
	{
		Mat4 projectMat = Transform::Perspective(cam.zprp, cam.zview);
		Vec4 temp = projectMat * a.position;
		temp.NormalizeByW();
		a.position = temp;
		return a; 			
	}

	

private:
	SDL_Renderer *m_renderer;
	ColorRGB m_color;
	Camera cam;
	Vec4 m_p[3];
	Vec3 p_p[3];
	Vec4 m_p1, m_p2, m_p3;
	Vec3 p_p1, p_p2, p_p3;
	float consA, consB, consC, consD;
	Vertex m_v1, m_v2, m_v3;
	Vec3 int1, int2, int3;
	
};

