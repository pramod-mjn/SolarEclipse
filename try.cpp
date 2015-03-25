#include <iostream>
#include <vector>

class Vec2
{
public:
	struct
	{
		float x, y;
	};
	Vec2(float a, float b) : x(a), y(b) {}
	Vec2() {}
	static void swapp(Vec2& a, Vec2& b)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);		
	}
};

class Vec3
{
public:
	 struct { float x, y, z; }; // used struct, so that x,y,z are contagious 

	//default -> all 0
	Vec3(void)  : x(0.0f), y(0.0f), z(0.0f) {}

	// use 3 elements to init
	Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

	static void swapp(Vec3& a, Vec3& b)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
		
	}

private:
};

class Vec4
{
public:
	struct{float x, y, z, w;}; // used struct so that x,y,z,w are contagious in memory

	Vec4(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {} 
	Vec4(float xx, float yy, float zz, float ww=1.f) : x(xx), y(yy), z(zz), w(ww) {}

	//use 3D vector and another element to init
	Vec4(const Vec3& v, float ww=1.0f) : x(v.x), y(v.y), z(v.z) , w(ww) {}
	static void swapp(Vec4& a, Vec4& b)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
		std::swap(a.w, b.w);		
	}
	

private:
};

struct Vertex{
	Vec4 position;
	Vec2 uvc;
	Vec3 normal;
	Vertex(){}
	Vertex(Vec4 a, Vec2 b, Vec3 n): position(a), uvc(b), normal(n)
	{}
};

void SortVertex(Vertex& v1, Vertex& v2, Vertex& v3)
{
	if(v1.position.y > v2.position.y){
		std::swap(v1.position.x, v2.position.x);
		std::swap(v1.position.y, v2.position.y);
		std::swap(v1.position.z, v2.position.z);
		std::swap(v1.position.w, v2.position.w);
		std::swap(v1.normal.x, v2.normal.x);
		std::swap(v1.normal.y, v2.normal.y);
		std::swap(v1.normal.z, v2.normal.z);
		std::swap(v1.uvc.x, v2.uvc.x);
		std::swap(v1.uvc.y, v2.uvc.y);
	    // Vec4::swapp(v1.position, v2.position);
	    // Vec3::swapp(v1.normal, v2.normal);
	    // Vec2::swapp(v1.uvc, v2.uvc);
  	}
  	if(v2.position.y > v3.position.y){
  		std::swap(v2.position.x, v3.position.x);
		std::swap(v2.position.y, v3.position.y);
		std::swap(v2.position.z, v3.position.z);
		std::swap(v2.position.w, v3.position.w);
		std::swap(v2.normal.x, v3.normal.x);
		std::swap(v2.normal.y, v3.normal.y);
		std::swap(v2.normal.z, v3.normal.z);
		std::swap(v2.uvc.x, v3.uvc.x);
		std::swap(v2.uvc.y, v3.uvc.y);
    	// Vec4::swapp(v2.position, v3.position);
	    // Vec3::swapp(v2.normal, v3.normal);
	    // Vec2::swapp(v2.uvc, v3.uvc);
  	}
  	if(v1.position.y > v2.position.y){
  		std::swap(v1.position.x, v2.position.x);
		std::swap(v1.position.y, v2.position.y);
		std::swap(v1.position.z, v2.position.z);
		std::swap(v1.position.w, v2.position.w);
		std::swap(v1.normal.x, v2.normal.x);
		std::swap(v1.normal.y, v2.normal.y);
		std::swap(v1.normal.z, v2.normal.z);
		std::swap(v1.uvc.x, v2.uvc.x);
		std::swap(v1.uvc.y, v2.uvc.y);
    	// Vec4::swapp(v1.position, v2.position);
	    // Vec3::swapp(v1.normal, v2.normal);
	    // Vec2::swapp(v1.uvc, v2.uvc);
	}
}

  		inline std::ostream& operator << (std::ostream &os, const Vec3 &v3)
{
	os << v3.x << " " << v3.y << " " << v3.z;
	return os;
}

inline std::ostream& operator << (std::ostream &os, const Vec4 &v4)
{
	os << v4.x << " " << v4.y << " " << v4.z << " " << v4.w;
	return os;
}

int main()
{
	std::vector<Vertex> vertices;
	Vertex a, b, c;
	a.position = Vec4(1,2,3,4);
	a.normal = Vec3(1,1,1);
	a.uvc = Vec2(1,1);

	b.position = Vec4(1,4,4,4);
	b.normal = Vec3(0,0,0);
	b.uvc = Vec2(0,0);

	c.position = Vec4(1,1,1,1);
	c.normal = Vec3(0,0,0);
	c.uvc = Vec2(0,0);

	vertices.push_back(a);
	vertices.push_back(b);
	vertices.push_back(c);

	SortVertex(vertices[0], vertices[1], vertices[2]);
	std::cout<<vertices[0].position;
	std::cout<<vertices[1].position;
	std::cout<<vertices[2].position;


}