// Vector3f.h: interface for the Vector3f class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3F_H__D045F2C3_77F6_4D3C_9793_597BBC7BEABB__INCLUDED_)
#define AFX_VECTOR3F_H__D045F2C3_77F6_4D3C_9793_597BBC7BEABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include <glide.h>
#include "globals.h"

inline float DEGtoRAD(float x)
{
	return (float)(x * (M_PI / 180.0));
}

class Vector3f  
{
public:
	float x, y, z;

	// Constructors
	Vector3f() { x = y = z = 0; }
	Vector3f(float x, float y, float z);
	Vector3f(const Vector3f &original) { x = original.x; y = original.y; z = original.z; }
	virtual ~Vector3f();

	GrVertex ToGrVertex()
	{
		GrVertex v;
		
		v.x = x;
		v.y = y;
		v.z = z;
		v.ooz = 1/v.z;

		return v;
	}

	Vector3f operator+(const Vector3f &second)
	{ 
		return Vector3f(this->x + second.x, this->y + second.y, this->z + second.z); 
	}
	
	Vector3f operator-(const Vector3f &second)
	{ 
		return Vector3f(this->x - second.x, this->y - second.y, this->z - second.z); 
	}

	Vector3f operator*(const float scalar)
	{
		return Vector3f(x*scalar, y*scalar, z*scalar);
	}

	Vector3f operator/(const float scalar)
	{
		return Vector3f(x/scalar, y/scalar, z/scalar);
	}

	float Length()
	{
		return (float)(sqrt(x*x + y*y + z*z));
	}

	static float DotProduct(Vector3f a, Vector3f b)
	{
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	static float Distance(Vector3f a, Vector3f b)
	{
		return (float)sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)) + ((b.z - a.z) * (b.z - a.z)));
	}

	Vector3f Negate()
	{
		return Vector3f(-x, -y, -z);
	}

	Vector3f Normalize()
	{
		float mag = Length();

		if(mag != 0)
		{
			return Vector3f(x/mag, y/mag, z/mag);
		}
		else
		{
			return Vector3f(0,0,0);
		}
	}

	static Vector3f CrossProduct(const Vector3f a, const Vector3f b)
	{
		return Vector3f((a.y * b.z) - (a.z * b.y),
						(a.z * b.x) - (a.x * b.z),
						(a.x * b.y) - (a.y * b.x));
	}

	Vector3f RotateAroundX(float degrees)
	{
		float s = sinf(DEGtoRAD(degrees));
		float c = cosf(DEGtoRAD(degrees));

		return Vector3f(x,
						y*c - z*s,
						y*s + z*c);
	}

	Vector3f RotateAroundY(float degrees)
	{
		float s = sinf(DEGtoRAD(degrees));
		float c = cosf(DEGtoRAD(degrees));

		return Vector3f(x*c + z*s,
						y,
						-x*s + z*c);
	}

	Vector3f RotateAroundZ(float degrees)
	{
		float s = sinf(DEGtoRAD(degrees));
		float c = cosf(DEGtoRAD(degrees));

		return Vector3f(x*c - y*s,
						x*s + y*c,
						z);
	}

	/* Static definitions for Vector3f constants */
	static Vector3f Right() { return Vector3f(1, 0, 0); }
	static Vector3f Up() { return Vector3f(0, 1, 0); }
	static Vector3f Forward() { return Vector3f(0, 0, 1); }
};

#endif // !defined(AFX_VECTOR3F_H__D045F2C3_77F6_4D3C_9793_597BBC7BEABB__INCLUDED_)
