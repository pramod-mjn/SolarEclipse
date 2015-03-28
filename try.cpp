#include <iostream>
#include <vector>
#include <cmath>

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

	Vec3 operator + (const Vec3 & other) const
	{
		return Vec3(x+other.x, y+other.y, z+other.z);
	}

	Vec3 operator - (const Vec3 & other)
	{
		return Vec3(x-other.x, y-other.y, z-other.z);
	}

	void operator-=(const Vec3 & other)
	{
		x = x-other.x; y=y-other.y; z=z-other.z;
	}

	void operator+=(const Vec3 & other)
	{
		x = x+other.x; y=y+other.y; z=z+other.z;
	}

	Vec3 operator * (float k) const 
	{
		return Vec3(k*x, k*y, k*z);
	}

	Vec3 operator / (float k) const
	{
		return Vec3(x/k, y/k, z/k);
	}
	bool operator ==(const Vec3 & other)
	{
		if(x == other.x && y == other.y && z == other.z)
			return true;
		else
			return false;
	}

	float  operator[] (size_t i) const
	{
		if(i>=3)
		{
			throw "index access error...";
		}
		return (&x)[i];
	}

	float & operator[] (size_t i)
	{
		if(i>=3)
		{
			throw "index access error...";
		}
		return (&x)[i];
	}

	inline float Length() const
    {
        return sqrtf(x*x+y*y+z*z);
    }

    void NormalizeToUnit(void)
    {
    	float invlen = 1/Length();
    	x *= invlen;
    	y *= invlen;
    	z *= invlen;
    }

    static Vec3 NormalizeToUnit(const Vec3 &  v)
    {
    	float invlen = 1/v.Length();
    	return Vec3(v.x*invlen, v.y*invlen, v.z*invlen);
    }

	static float Dot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}

	static Vec3 Cross(const Vec3& a, const Vec3& b)
	{
		return Vec3(
				a.y * b.z - b.y * a.z,
				-(a.x * b .z - a.z * b.x),
				a.x * b.y - b.x * a.y
				);
	}
	
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
Vec3 SurNormal(Vec3 a, Vec3 b, Vec3 c)
{
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	return Vec3::Cross(ac, ab);
}

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
	Vec3 tri[3];
	Vec3 light(-1,0,0);
	Vec3 point(100,8,2);

	tri[0] = Vec3(20,0,10); tri[1] = Vec3(20,0,0); tri[2] = Vec3(20,10,0);

	float A,B,C,D;

	//Vec3 norm = SurNormal(tri[0], tri[1], tri[2]);
	Vec3 ab = tri[1] - tri[0];
	Vec3 ac = tri[2] - tri[0];
	std::cout<<ab<<"    "<<ac<<"\n";
	Vec3 norm =  Vec3::Cross(ac, ab);
	A = norm.x;
	B= norm.y;
	C= norm.z;
	D = -(A * tri[0].x + B * tri[0].y + C * tri[0].z);

	float u = - ( A*point.x + B*point.y + C*point.z + D)/(A*(light.x) + B*light.y + C*light.z);
	norm.NormalizeToUnit();
	std::cout<<"norm = "<<norm<<"\n";
	std::cout<<"u = "<<u<<"\n";

	Vec3 temp  = Vec3(point.x + u*(light.x), point.y + u*light.y, point.z + u*light.z);

	Vec3 n1 = Vec3::Cross(tri[0] - temp, tri[1] - temp);
	Vec3 n2 = Vec3::Cross(tri[1] - temp, tri[2] -temp);
	Vec3 n3 = Vec3::Cross(tri[2] - temp, tri[0] - temp);
	n1.NormalizeToUnit();
	n2.NormalizeToUnit();
	n3.NormalizeToUnit();
	int a =Vec3::Dot(n1, norm);
	int b =Vec3::Dot(n2, norm);
	int c =Vec3::Dot(n3, norm);
	
	std::cout<<temp<<"\n";
	std::cout<<a<<" "<<b<<" "<<c <<"\n";
	if ((a>=0 && b>=0 && c>=0) || (a<=0 && b<=0 && c<=0))
		std::cout<<"True";
	else
		std::cout<<"False";

		return 0;





}