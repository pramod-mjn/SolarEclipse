#pragma once

#include <iostream>
#include <cmath>

//------------------- A 2D point struct ---------------------------


struct Point2D
{
	struct { int x, y; }; // for contagious storage in memory
	float depth;
	//Vec3 attributes[2]; // point has only one attribute now, that is color,normal

	Point2D(int xx, int yy) : x(xx) , y(yy) {depth=0;} // initially set the depth value to be 0
	Point2D() {} 
//	~Point2D(void) {}

	bool operator==(const Point2D & point);
	bool operator>(const Point2D & point);
	bool operator<(const Point2D & point);
	Point2D operator+(const Point2D & point);
	Point2D operator-(const Point2D & point);
};

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

//------------------- A 3D vector class ---------------------------

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
	static Point2D ToVec2(const Vec3& a)
	{
		return Point2D(a.x, a.y);
	}
	static void swapp(Vec3& a, Vec3& b)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
		
	}

private:
};

//------------------- A 4D vector class ---------------------------

class Vec4
{
public:
	struct{float x, y, z, w;}; // used struct so that x,y,z,w are contagious in memory

	Vec4(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {} 
	Vec4(float xx, float yy, float zz, float ww=1.f) : x(xx), y(yy), z(zz), w(ww) {}

	//use 3D vector and another element to init
	Vec4(const Vec3& v, float ww=1.0f) : x(v.x), y(v.y), z(v.z) , w(ww) {}

	Vec4 operator + (const Vec4 & other) const
	{
		return Vec4(x+other.x, y+other.y, z+other.z, w+other.w);
	}

	Vec4 operator - (const Vec4 & other) const
	{
		return Vec4(x-other.x, y-other.y, z-other.z, w-other.w);
	}

	Vec4 operator * (float k) const 
	{
		return Vec4(k*x, k*y, k*z, k*w);
	}

	Vec4 operator / (float k) const 
	{
		return Vec4(x/k, y/k, z/k, w/k);
	}

	float  operator[] (size_t i) const
	{
		if(i>=4)
		{
			throw "index access error...";
		}
		return (&x)[i];
	}

	float & operator[] (size_t i)
	{
		if(i>=4)
		{
			throw "index access error...";
		}
		return (&x)[i];
	}
	bool operator == (const Vec4 & other)
	{
		if(x == other.x && y == other.y && z == other.z && w == other.w)
			return true;
		return false;
		// if(abs(x - other.x) < 0)
		// if(abs(y - other.y) < 0)
		// if(abs(z - other.z) < 0)
		// if(abs(w - other.w) < 0)
		// 	return true;
		// return false;
	}

	inline float Length(void) const
	{
		return sqrtf(x*x+y*y+z*z+w*w);
	}

	void NormalizeToUnit(void)
	{
		float invlen = 1/Length();
    	x *= invlen;
    	y *= invlen;
    	z *= invlen;
    	w *= invlen;
	}

	void NormalizeByW(void)
	{
		if(w!=1 && w!=0)
		{
			x = x/w;
			y = y/w;
			z = z/w;
			w = 1;
		}
	}
	static Vec3 Cross(const Vec4& a, const Vec4& b)
	{
		return Vec3(
				a.y * b.z - b.y * a.z,
				-(a.x * b .z - a.z * b.x),
				a.x * b.y - b.x * a.y
				);
	}

	static Vec3 ToVec3(Vec4& a)
	{
		return Vec3(a.x, a.y, a.z);
	}
	Vec3 ToVec3()
	{
		return Vec3(x, y, z);
	}
	static Point2D ToVec2(const Vec4& a)
	{
		return Point2D(a.x, a.y);
	}
	static Vec3 SurNorm(Vec4 a, Vec4 b, Vec4 c)
	{
		Vec4 ab = b - a;
		Vec4 ac = c - a;
		return Vec4::Cross(ac, ab);
	}
	static void swapp(Vec4& a, Vec4& b)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
		std::swap(a.z, b.z);
		std::swap(a.w, b.w);		
	}
	

private:
};

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


struct Vertex{
	Vec4 position;
	Vec2 uvc;
	Vec3 normal;
	Vec3 color;
	int phi_count, theta_count;
	Vertex(){}
	Vertex(Vec4 a, Vec2 b, Vec3 c): position(a), uvc(b), color(c)
	{
		normal = Vec3(0,0,0);
	}
	Vertex(Vec4 a, Vec2 b, Vec3 n, Vec3 c): position(a), uvc(b), normal(n), color(c)
	{}
};
struct Surface{
	Vec4 v1, v2, v3, v4;
	Vec3 normal;
	Surface() {}
	Surface(Vec4 a, Vec4 b, Vec4 c, Vec4 d, Vec3 n): v1(a), v2(b), v3(c), v4(d), normal(n)
	{}
};
;
