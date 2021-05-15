#pragma once

#include "Vector3f.h"
#include <stdio.h>
#include <glide.h>

const int screen_width  = 640;
const int screen_height = 480;

typedef struct transformation_t {
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	transformation_t()
	{
		position = Vector3f(0,0,0);
		rotation = Vector3f(0,0,0);
		scale = Vector3f(1,1,1);
	};

	char dbg[200];
	Vector3f ForwardVec()
	{
		Vector3f v = Vector3f(0,0,1);
		
		v = v.RotateAroundZ(rotation.z);
		v = v.RotateAroundY(rotation.y);
		v = v.RotateAroundX(rotation.x);

		return v;
	}

	Vector3f UpVec()
	{
		Vector3f v = Vector3f(0,1,0);
		
		v = v.RotateAroundZ(rotation.z);
		v = v.RotateAroundY(rotation.y);
		v = v.RotateAroundX(rotation.x);

		return v;
	}

	Vector3f RightVec()
	{
		Vector3f v = Vector3f(1,0,0);
		
		v = v.RotateAroundZ(rotation.z);
		v = v.RotateAroundY(rotation.y);
		v = v.RotateAroundX(rotation.x);

		return v;
	}
} Transformation;


/*
inline float DEGtoRAD(float x)
{
	return (float)(x * (M_PI / 180.0));
}
*/