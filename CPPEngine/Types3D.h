#pragma once

#include <vector>
#include "glide.h"
#include "Math/Vector3f.h"

#include <string>

namespace Types3D
{
	typedef struct
	{
		float x1, y1, x2, y2;
	} Rectangle;

	typedef struct 
	{
		std::string name;
		Vector3f Ka, Kd, Ks, Ke;
		float Ns, Ni;
		float D;
		int illum;
		std::string map_Kd; // texture, effectively
	} WFMaterial;

	typedef struct
	{
		GrVertex vertexes[3];
		WFMaterial *material; // if any
	} Triangle;

	typedef struct
	{
		/* Three indexes into a Points component vector. */
		int pointIdx[3];
		WFMaterial *material; // if any
	} FaceTriangle;
}