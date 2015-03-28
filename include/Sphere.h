#pragma once
#include <Rasterize.h>
#include <Vector.h>
#include <vector>


class Sphere 
{
public:

	int phi_steps, theta_steps;
	std::vector<Vertex> vertices;
	//std::vector<Vec4> vertices;
	std::vector<Vec3> index;
	//std::vector<Vec2> uvc;
	std::vector<Vec3> SNorm;
	std::vector<Surface> surfaces;

	Sphere(Vec3 center, float radius, int resn) : m_radius(radius), resolution(resn)
	{
		extern Light light;
		phi_steps = resolution, theta_steps = resolution;
	int phi_vertex_count = phi_steps + 1 + 1;
	int theta_vertex_count = theta_steps + 1;

		vertices.resize(phi_vertex_count * theta_vertex_count);
		vertices[0].phi_count =phi_vertex_count;
		vertices[0].theta_count = theta_vertex_count;
		m_center = Vec4(center,0);

		double ang_phi =2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
		Vec4 r(0, 0, m_radius, 1);
		
		for(int i=0;i< phi_vertex_count; ++i)
			for(int j=0;j< theta_vertex_count; ++j) 
				vertices[j*(phi_vertex_count) + i].normal = Vec3(0,0,0);

		for (int i = 0; i < phi_vertex_count; ++i)
		{
			
			double theta = - M_PI / 2.0;
			
			for (int j = 0; j < theta_vertex_count; ++j)
					{

				Vec4 a;
				a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
				theta += ang_theta;

				vertices[j * phi_vertex_count + i].position = a + m_center;
				vertices[j * phi_vertex_count + i].uvc = Vec2(i*1.0f/(phi_vertex_count-1), j*1.0f/(theta_vertex_count-1));
			}
		}

		//calculating normal
		for(int i=0; i<= phi_steps; i++)
			for(int j =0; j< theta_steps; j++)
			{
				Vec4 a = vertices[j*phi_vertex_count + i].position;
				Vec4 b = vertices[j*phi_vertex_count + i+1].position;
				Vec4 c = vertices[(j+1)*phi_vertex_count + i].position;
				Vec3 norm = Vec4::Cross(c-a,b-a);
				vertices[j*phi_vertex_count + i].normal += norm;
				vertices[j*phi_vertex_count + i+1].normal += norm;
				vertices[(j+1)*phi_vertex_count + i].normal += norm;
				vertices[(j+1)*phi_vertex_count + i+1].normal += norm;
			}	
	
	}
	
	void Draw(SDL_Renderer *renderer, Camera& cam, ColorRGB& color)
	{
		for(int i=0;i<= phi_steps;i++)
		{
			for(int j=0;j< theta_steps;j++)
			{
				int a = j*(phi_steps + 2) + i;
				int b = (j)*(phi_steps + 2) + i+1;
				int c = (j+1)*(phi_steps + 2) + i;
				int d = (j+1)*(phi_steps + 2) + (i+1);

	    		Rasterize(renderer, cam).DrawTriangle(vertices[a].position, vertices[b].position, 
	    		vertices[c].position, color);
	    		Rasterize(renderer, cam).DrawTriangle(vertices[b].position, vertices[c].position, 
	    		vertices[d].position, color);	
		    	
		    }	
			
		}	
	}
	void Fill(SDL_Renderer *renderer, Camera& cam, ColorRGB& color, float *zBuffer)
	{
		for(int i=0;i<= phi_steps;i++)
		{
			for(int j=0;j< theta_steps;j++)
			{
				int a = j*(phi_steps + 2) + i;
				int b = (j)*(phi_steps + 2) + i+1;
				int c = (j+1)*(phi_steps + 2) + i;
				int d = (j+1)*(phi_steps + 2) + (i+1);

				//finding surface normal for backface culling
			    Vec4 ab = vertices[b].position - vertices[a].position;
			    Vec4 ac = vertices[c].position - vertices[a].position;
			    Vec3 norm = Vec3::Cross(ac.ToVec3(), ab.ToVec3());
			    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;
			
			
		    	//if(Vec3::Dot(camNorm, norm) < 0) //Backface culling 
		    	{
		    		Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], 
		    		vertices[c], color, zBuffer);
		    		Rasterize(renderer, cam).FillTriangle(vertices[b], vertices[c], 
		    		vertices[d], color, zBuffer);	
		    	}
		    }	
			
		}
	}
	void Map(SDL_Renderer *renderer, Camera& cam, Texture &texObj, float *zBuffer)
	{
		
		for(int i=0;i<=phi_steps;i++)
		{
			for(int j=0;j<theta_steps;j++)	
			{
				int a = j*(phi_steps + 2) + i;
				int b = (j)*(phi_steps + 2) + (i+1);
				int c = (j+1)*(phi_steps + 2) + (i);
				int d = (j+1)*(phi_steps + 2) + (i+1);
						
				//finding surface normal for backface culling
			    Vec4 ab = vertices[b].position - vertices[a].position;
			    Vec4 ac = vertices[c].position - vertices[a].position;
			    Vec3 norm = Vec3::Cross(ac.ToVec3(), ab.ToVec3());
			    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;

			    //if(Vec3::Dot(camNorm, norm) >= 0) //Backface culling 
			    if(resolution > 20)
			    {
			    	Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], vertices[c], texObj, zBuffer);
					Rasterize(renderer, cam).FillTriangle(vertices[b], vertices[c], vertices[d], texObj, zBuffer);
				}
				else
				{
					Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], vertices[c], texObj, zBuffer, 1);
					Rasterize(renderer, cam).FillTriangle(vertices[b], vertices[c], vertices[d], texObj, zBuffer, 1);	
				}
			}	
		}
	}	
	
private:
	Vec4 m_center;
	float m_radius;
	
	int resolution;
	//SDL_Renderer *m_renderer;
	
};