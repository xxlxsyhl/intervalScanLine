#ifndef VEC_H
#define VEC_H
#include <cmath>
#include "utils.h"
#include "def.h"

class Vec3f;
typedef Vec3f Point3f;
typedef Vec3f Color3f;

class Vec3f 
{
public:
	union 
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
	//constructor
	Vec3f() :x(0), y(0), z(0) {};
	Vec3f(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};

	//operator
	Vec3f& operator = (const Vec3f &v) { x = v.x; y = v.y; z = v.z; return *this; }

	Vec3f& operator += (float num) { x += num; y += num; z += num; return *this; }
	Vec3f& operator += (const Vec3f &v) { x += v.x; y += v.y; z += v.z; return *this; }

	Vec3f& operator -= (float num) { x -= num; y -= num; z -= num; return *this; }
	Vec3f& operator -= (const Vec3f &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

	Vec3f& operator *= (float num) { x *= num; y *= num; z *= num; return *this; }
	Vec3f& operator *= (const Vec3f &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	Vec3f& operator /= (float num) { x /= num; y /= num; z /= num; return *this; }
	Vec3f& operator /= (const Vec3f &v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

	friend Vec3f operator + (const Vec3f &v, double num) { return Vec3f(v.x + num, v.y + num, v.z + num); }
	friend Vec3f operator + (double num, const Vec3f &v) { return Vec3f(num + v.x, num + v.y, num + v.z); }
	friend Vec3f operator + (const Vec3f &u, const Vec3f &v) { return Vec3f(u.x + v.x, u.y + v.y, u.z + v.z); }
	friend Vec3f operator - (const Vec3f &v, double num) { return Vec3f(v.x - num, v.y - num, v.z - num); }
	friend Vec3f operator - (double num, const Vec3f &v) { return Vec3f(num - v.x, num - v.y, num - v.z); }
	friend Vec3f operator - (const Vec3f &u, const Vec3f &v) { return Vec3f(u.x - v.x, u.y - v.y, u.z - v.z); }
	friend Vec3f operator * (const Vec3f &v, double num) { return Vec3f(v.x * num, v.y * num, v.z * num); }
	friend Vec3f operator * (double num, const Vec3f &v) { return Vec3f(num * v.x, num * v.y, num * v.z); }
	friend Vec3f operator * (const Vec3f &u, const Vec3f &v) { return Vec3f(u.x * v.x, u.y * v.y, u.z * v.z); }
	friend Vec3f operator / (const Vec3f &v, double num) { return Vec3f(v.x / num, v.y / num, v.z / num); }
	friend Vec3f operator / (double num, const Vec3f &v) { return Vec3f(num / v.x, num / v.y, num / v.z); }
	friend Vec3f operator / (const Vec3f &u, const Vec3f &v) { return Vec3f(u.x / v.x, u.y / v.y, u.z / v.z); }

	bool operator == (const Vec3f &v) { return isEqualf(x, v.x) && isEqualf(y, v.y) && isEqualf(z, v.z); }
	bool operator != (const Vec3f &v) { return !(isEqualf(x, v.x) && isEqualf(y, v.y) && isEqualf(z, v.z)); }

};

inline Vec3f cross(const Vec3f &u, const Vec3f &v)
{
	return Vec3f(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline float dot(const Vec3f &u, const Vec3f &v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline float length(const Vec3f &v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vec3f normalize(const Vec3f &v)
{
	return v * (1.0f / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}


#endif