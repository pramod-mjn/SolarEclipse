#pragma once
#include <Rasterize.h>
#include <Vector.h>
#include <vector>


class Sphere 
{
public:
	std::vector<Vertex> vertices;
	//std::vector<Vec4> vertices;
	std::vector<Vec3> index;
	//std::vector<Vec2> uvc;
	std::vector<Vec3> SNorm;
	std::vector<Surface> surfaces;

	Sphere(Vec3 center, float radius) : m_radius(radius)
	{
		vertices.resize(441);
		m_center = Vec4(center,0);
		int phi_steps = 20, theta_steps = 20;
		double ang_phi =2 * M_PI / phi_steps, ang_theta = M_PI / theta_steps;
		Vec4 r(0, 0, m_radius, 1);
		
		for(int i=0;i<phi_steps; ++i)
			for(int j=0;j<theta_steps; ++j)
				vertices[i*20 + j].normal = Vec3(0,0,0);
		for (int i = 0; i < phi_steps; ++i)
		{
			
			double theta = - M_PI / 2.0;
			Vec4 a, b, c, d;
			a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
			b = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r;
			Vec2 uv1(i*1.0f/20, 0);
			Vec2 uv2((i+1)*1.0f/20, 0);
			vertices[i*20].position = a+m_center;
			vertices[i*20].uvc = uv1;
			vertices[(i+1)*20].position = b+m_center;
			vertices[(i+1)*20].uvc = uv2;
			


			for (int j = 0; j < theta_steps; ++j)
			{
				//Vec4 a = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta) * r;
				//Vec4 b = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta) * r; 
				c = Transform::RotateY(i* ang_phi) * Transform::RotateX(theta + ang_theta) * r;
				d = Transform::RotateY(i* ang_phi + ang_phi) * Transform::RotateX(theta + ang_theta) * r;
				// vertices.push_back(a+m_center);
				// vertices.push_back(b+m_center);
				// vertices.push_back(c+m_center);
				// vertices.push_back(d+m_center);	
				// Vec2 uv1(i*1.0f/20, j*1.0f/20);
				// Vec2 uv2((i+1)*1.0f/20, j*1.0f/20);
				Vec2 uv3(i*1.0f/20, (j+1)*1.0f/20);
				Vec2 uv4((i+1)*1.0f/20, (j+1)*1.0f/20);
				
				
				Vec3 n = Vec4::SurNorm(a, b, c);
				n.NormalizeToUnit();
				surfaces.push_back(Surface(a+m_center, b+m_center, c+m_center, d+m_center, n));
				//Vec3 n2 = Vec4::SurNorm(b, d, c);
				//n2.NormalizeToUnit();
				//surfaces.push_back(Surface(b+m_center, c+m_center, d+m_center, n2));
				Vertex temp;
				
				vertices[i*20 + (j+1)].position = c+m_center;
				vertices[i*20 + (j+1)].uvc = uv3;
				vertices[(i+1)*20 + (j+1)].position = d+m_center;
				vertices[(i+1)*20 + (j+1)].uvc = uv4;
				vertices[i*20 + j].normal += n;
				vertices[(i+1)*20 + j].normal += n;
				vertices[i*20 + (j+1)].normal += n;
				vertices[(i+1)*20 + (j+1)].normal += n;

				// temp = Vertex(a+m_center, uv1);
				// vertices.push_back(temp); 
				// temp = Vertex(b+m_center, uv2);
				// vertices.push_back(temp);
				// temp = Vertex(c+m_center, uv3);
				// vertices.push_back(temp);
				// temp = Vertex(d+m_center, uv4);
				// vertices.push_back(temp);
				a = c; b = d;
				int size = vertices.size();
				Vec3 indices = Vec3(size-4, size-3, size-2);
				index.push_back(indices);
				indices = Vec3(size-3, size-1, size-2);
				index.push_back(indices);

				theta += ang_theta;
				//std::cout<<index.size()<<" ";
				//std::cout<<vertices.size()<<" ";

			}
		}	
		//Update_vertexNormal();
		// for(int i=0;i<vertices.size();i++)
		// 	if(vertices[i].count == 3)
		// 	std::cout<<i<<" ";
	
	}
	void Update_vertexNormal()
	{
		for(int i=0;i<vertices.size();i++)
		{
			Vec4 point=vertices[i].position;
			for(int j=0;j<surfaces.size();j++)
			{
				if(point == surfaces[j].v1 || point == surfaces[j].v2 || point == surfaces[j].v3 || point == surfaces[j].v4)
				{
					//std::cout<<point<<"\n";
					vertices[i].normal += surfaces[j].normal;
					vertices[i].count += 1;
				}
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
	// void Draw(SDL_Renderer *renderer, Camera& cam, ColorRGB& color)
	// {
	// 	for(int i=0;i<index.size();i++)
	// 	{
	// 		int a = index[i].x;
	// 		int b = index[i].y;
	// 		int c = index[i].z;
	// 		Rasterize(renderer, cam).DrawTriangle(vertices[a].position, vertices[b].position, 
	// 			vertices[c].position, color);
			
	// 	}		
	// }
	// void Fill(SDL_Renderer *renderer, Camera& cam, ColorRGB& color, float *zBuffer)
	// {

	// 	for(int i=0;i<index.size();i++)
	// 	{
	// 		int a = index[i].x;
	// 		int b = index[i].y;
	// 		int c = index[i].z;
			
	// 		//finding surface normal for backface culling
	// 	    Vec4 ab = vertices[b].position - vertices[c].position;
	// 	    Vec4 bc = vertices[a].position - vertices[b].position;
	// 	    Vec3 norm = Vec3::Cross(ab.ToVec3(), bc.ToVec3());
	// 	    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;

	// 	    //if(Vec3::Dot(camNorm, norm) < 0) //Backface culling 
	// 	    	Rasterize(renderer, cam).FillTriangle(vertices[a].position, vertices[b].position, 
	// 	    		vertices[c].position, color, zBuffer);
			
	// 	}
	// }
	void Map(SDL_Renderer *renderer, Camera& cam, Texture &texObj, float *zBuffer)
	{
		for(int i=0;i<20;i++)
		{
			for(int j=0;j<20;j++)
			{
				int a = i*20 + j;
				int b = (i+1)*20 + j;
				int c = i*20 + (j+1);
				int d = (i+1)*20 + (j+1);
			// int a = index[i].x;
			// int b = index[i].y;
			// int c = index[i].z;
			
			// //finding surface normal for backface culling
		 //    Vec4 ab = vertices[b].position - vertices[c].position;
		 //    Vec4 bc = vertices[a].position - vertices[b].position;
		 //    Vec3 norm = Vec3::Cross(ab.ToVec3(), bc.ToVec3());
		 //    Vec3 camNorm = Vec3(0,0,cam.zprp) - cam.lookat;
		 //    //std::cout<<vertices[a].position<<" "<<vertices[b].position<<" "<<vertices[c].position<<"\n";

		    //if(Vec3::Dot(camNorm, norm) >= 0) //Backface culling 
		    	// Rasterize(renderer, cam).FillTriangle(vertices[a].position, vertices[b].position, vertices[c].position,
		    	//  vertices[a].uvc, vertices[b].uvc, vertices[c].uvc, texObj, zBuffer);
		    Rasterize(renderer, cam).FillTriangle(vertices[a], vertices[b], vertices[c], texObj, zBuffer);
			Rasterize(renderer, cam).FillTriangle(vertices[b], vertices[c], vertices[d], texObj, zBuffer);
		    // Rasterize(renderer, cam).FillTriangle(vertices[a].position, vertices[b].position, vertices[c].position,
		    // 	vertices[a].uvc, vertices[b].uvc, vertices[c].uvc, vertices[a].normal, vertices[b].normal,
		    // 	vertices[c].normal, texObj, zBuffer);
			}	
		}
	}
	


	
	
private:
	Vec4 m_center;
	float m_radius;
	//SDL_Renderer *m_renderer;
	
};