#pragma once
#include <Rasterize.h>
#include <Vector.h>
#include <vector>

class Sphere 
{
public:
	std::vector<Vec4> vertices;
	std::vector<Vec3> index;
	std::vector<Vec2> uvc;

	Sphere(Vec3 center, float radius) : m_radius(radius)
	{
		m_center = Vec4(center,0);
		int phi_steps = 20, theta_steps = 20;
		double ang_phi = 2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
		Vec4 r(0, 0, m_radius, 1);
		
		for (int i = 0; i < phi_steps; ++i)
		{
			
			double theta = - M_PI / 2.0;
			Vec4 a, b, c, d;
			a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
			b = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r;

			for (int j = 0; j < theta_steps; ++j)
			{
				//Vec4 a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
				//Vec4 b = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r; 
				c = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta + ang_theta) * r;
				d = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta + ang_theta) * r;
				vertices.push_back(a+m_center);
				vertices.push_back(b+m_center);
				vertices.push_back(c+m_center);
				vertices.push_back(d+m_center);	
				Vec2 uv1(i*1.0f/20, j*1.0f/20);
				Vec2 uv2((i+1)*1.0f/20, j*1.0f/20);
				Vec2 uv3(i*1.0f/20, (j+1)*1.0f/20);
				Vec2 uv4((i+1)*1.0f/20, (j+1)*1.0f/20);
				uvc.push_back(uv1);
				uvc.push_back(uv2);
				uvc.push_back(uv3);
				uvc.push_back(uv4);
				a = c; b = d;
				int size = vertices.size();
				Vec3 indices = Vec3(size-4, size-3, size-2);
				index.push_back(indices);
				indices = Vec3(size-3, size-1, size-2);
				index.push_back(indices);

				theta += ang_theta;

			}
		}

	}
	Vec2 CoorUV(const Vec4& a)
	{
		Vec2 uv;
		Vec3 d = Vec3(a.x, a.y, a.z);
        d.NormalizeToUnit();

      	uv.x = 0.5 + (atan2(d.z, d.x))/2*M_PI;
      	uv.y = 0.5 - (asin(d.z))/M_PI;
      	return uv;

	}
	void Draw(SDL_Renderer *renderer, Camera& cam, ColorRGB& color)
	{
		for(int i=0;i<index.size();i++)
		{
			int a = index[i].x;
			int b = index[i].y;
			int c = index[i].z;
			Rasterize(renderer, cam).DrawTriangle(vertices[a], vertices[b], vertices[c], color);
			
		}		
	}
	void Fill(SDL_Renderer *renderer, Camera& cam, ColorRGB& color, float *zBuffer)
	{
		for(int i=0;i<index.size();i++)
		{
			int a = index[i].x;
			int b = index[i].y;
			int c = index[i].z;
			
			//finding surface normal for backface culling
		    Vec4 ab = vertices[b] - vertices[c];
		    Vec4 bc = vertices[a] - vertices[b];
		    Vec3 norm = Vec3::Cross(ab.ToVec3(), bc.ToVec3());
		    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;

		    //if(Vec3::Dot(camNorm, norm) < 0) //Backface culling 
		    	Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], vertices[c], color, zBuffer);
			
		}
	}
	void Map(SDL_Renderer *renderer, Camera& cam, Texture &texObj, float *zBuffer)
	{
		for(int i=0;i<index.size();i++)
		{
			int a = index[i].x;
			int b = index[i].y;
			int c = index[i].z;
			
			//finding surface normal for backface culling
		    Vec4 ab = vertices[b] - vertices[c];
		    Vec4 bc = vertices[a] - vertices[b];
		    Vec3 norm = Vec3::Cross(ab.ToVec3(), bc.ToVec3());
		    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;

		    //if(Vec3::Dot(camNorm, norm) > 0) //Backface culling 
		    	Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], vertices[c], uvc[a], uvc[b],
		    	uvc[c], texObj, zBuffer);
			
		}
	}
	


	
	
private:
	Vec4 m_center;
	float m_radius;
	//SDL_Renderer *m_renderer;
	
};