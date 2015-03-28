#pragma once
#include <Vector.h>
#include <Pixel.h>
#define Minn(a,b) ((a<b)?a:b)
#define Maxx(a,b) ((a>b)?a:b)

// struct Material {
  	
// 	Material(float a, float b, float c, float d): ka(a), kd(b), ks(c), ns(d)
// 	{}
// };

struct Light {
	Vec3 direction, pos;
	Vec3 light_intensity;
    float ka = 0.1;
    float kd = 0.8;
    float ks = 0.5;
    float ns = 200;
    Light(){}
	Light(float x, float y, float z, float r, float g, float b)
	{
		//position = Vec3(x,y,z);
        direction = Vec3(-1,0,0);    
		light_intensity = Vec3(r,g,b);
	}
	
    //returns normalized color
    Vec3 CalculateLightIntensity(Vertex vex, const Camera & cam) const {
    	
        Vec3 ambient = light_intensity * ka;

    	Vec3 point = Vec3(vex.position.x, vex.position.y, vex.position.z);
        Vec3 N = vex.normal;
        //Vec3 pos = position;
        //Vec3 L = pos - point;
        Vec3 L = direction;
        L.NormalizeToUnit();    
        N.NormalizeToUnit();
        
        double diff_fact = Vec3::Dot(N,L);
        Vec3 R = N * 2.0 * diff_fact  - L;

        diff_fact = Maxx(diff_fact,0.0f);
        diff_fact = Minn(diff_fact,1.0f);
        
        Vec3 diffuse = light_intensity * diff_fact * kd ;
        //specular begin
        Vec3 V = (Vec3(0, 0, cam.zprp) - point); 
        V.NormalizeToUnit();
        
        double spec_fact = Vec3::Dot(V,R);
        spec_fact = Maxx(spec_fact, 0.0f);
        spec_fact = Minn(spec_fact, 1.0f);
        double K = ks * pow(spec_fact, ns);
        Vec3 specular = light_intensity * K;
        Vec3 res= ambient + specular + diffuse;
        res.x = Minn(res.x, 1.0f);
        res.y = Minn(res.y, 1.0f);
        res.z = Minn(res.z, 1.0f);
        res.x = Maxx(res.x, 0.0f);
        res.y = Maxx(res.y, 0.0f);
        res.z = Maxx(res.z, 0.0f);
        return res;
    }
    Vec3 CalculateShadowIntensity(Vertex vex, const Camera & cam) const {
        
        Vec3 ambient = light_intensity * ka;
        return ambient;
    }

};



	 



