// Matrix44.cpp: implementation of the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#include "Matrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Building translation/rotation/scale matrix functions.
Matrix44 Matrix44::Translation(Vector3f v)
{
	return Matrix44::Translation(v.x, v.y, v.z);
}

Matrix44 Matrix44::Translation(float x, float y, float z)
{
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[3][0] = x;
	m.data[3][1] = y;
	m.data[3][2] = z;

	return m;
}

Matrix44 Matrix44::Scale(Vector3f v)
{
	return Matrix44::Scale(v.x, v.y, v.z);
}

Matrix44 Matrix44::Scale(float x, float y, float z)
{
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[0][0] = x;
	m.data[1][1] = y;
	m.data[2][2] = z;

	return m;
}

// Roll
Matrix44 Matrix44::RotationX(float theta)
{
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[1][1] = cosf(theta);
	m.data[1][2] = -sinf(theta);
	m.data[2][1] = sinf(theta);
	m.data[2][2] = cosf(theta);

	return m;
}

// Pitch
Matrix44 Matrix44::RotationY(float theta)
{
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[0][0] = cosf(theta);
	m.data[0][2] = -sinf(theta);
	m.data[2][0] = sinf(theta);
	m.data[2][2] = cosf(theta);

	return m;
}

// Yaw
Matrix44 Matrix44::RotationZ(float theta)
{
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[0][0] = cosf(theta);
	m.data[1][0] = sinf(theta);
	m.data[0][1] = -sinf(theta);
	m.data[1][1] = cosf(theta);

	return m;
}

Matrix44 Matrix44::Rotation(Vector3f v)
{
	return Matrix44::Rotation(v.x, v.y, v.z);
}

Matrix44 Matrix44::Rotation(float yaw, float pitch, float roll)
{
#define ALPHA yaw
#define BETA pitch
#define GAMMA roll

	yaw = DEGtoRAD(yaw);
	pitch = DEGtoRAD(pitch);
	roll = DEGtoRAD(roll);

	// Compose a rotation matrix from Z, Y, and X rotations.
	Matrix44 m = Matrix44();
	m.Identity();

	m.data[0][0] = cosf(ALPHA) * cosf(BETA);
	m.data[0][1] = (cosf(ALPHA) * sinf(BETA) * sinf(GAMMA)) - (sinf(ALPHA) * cosf(GAMMA));
	m.data[0][2] = (cosf(ALPHA) * sinf(BETA) * cosf(GAMMA)) + (sinf(ALPHA) * sinf(GAMMA));

	m.data[1][0] = sinf(ALPHA) * cosf(BETA);
	m.data[1][1] = (sinf(ALPHA) * sinf(BETA) * sinf(GAMMA)) + (cosf(ALPHA) * cosf(GAMMA));
	m.data[1][2] = (sinf(ALPHA) * sinf(BETA) * cosf(GAMMA)) - (cosf(ALPHA) * sinf(GAMMA));

	m.data[2][0] = -sinf(BETA);
	m.data[2][1] = cosf(BETA) * sinf(GAMMA);
	m.data[2][2] = cosf(BETA) * cosf(GAMMA);

#undef ALPHA
#undef BETA
#undef GAMMA

	return m;
}

void Matrix44::Identity()
{
	data[0][0] = 1;	data[0][1] = 0;	data[0][2] = 0;	data[0][3] = 0;
	data[1][1] = 0;	data[1][1] = 1;	data[1][2] = 0;	data[1][3] = 0;
	data[2][2] = 0;	data[2][1] = 0;	data[2][2] = 1;	data[2][3] = 0;
	data[3][3] = 0;	data[3][1] = 0;	data[3][2] = 0;	data[3][3] = 1;
}

void Matrix44::TransformNormal(const GrVertex *in, GrVertex *out)
{
	out->x = (in->x * data[0][0]) + (in->y * data[1][0]) + (in->z * data[2][0]) + data[3][0];
	out->y = (in->x * data[0][1]) + (in->y * data[1][1]) + (in->z * data[2][1]) + data[3][1];
	out->z = (in->x * data[0][2]) + (in->y * data[1][2]) + (in->z * data[2][2]) + data[3][2];
}

void Matrix44::TransformPoint(const GrVertex *in, GrVertex *out)
{
	out->x = (in->x * data[0][0]) + (in->y * data[1][0]) + (in->z * data[2][0]) + data[3][0];
	out->y = (in->x * data[0][1]) + (in->y * data[1][1]) + (in->z * data[2][1]) + data[3][1];
	out->z = (in->x * data[0][2]) + (in->y * data[1][2]) + (in->z * data[2][2]) + data[3][2];
	float w =(in->x * data[0][3]) + (in->y * data[1][3]) + (in->z * data[2][3]) + data[3][3];

	if(w != 1)
	{
		out->x /= w;
		out->y /= w;
		out->z /= w;
	}

	out->r = in->r;
	out->g = in->g;
	out->b = in->b;
	out->a = in->a;

	if(out->z != 0)
	{
		// glide.h
		out->ooz = (float)(65535.0 / (out->z - 1));
	}
	if(w != 0)
	{
		out->oow = out->tmuvtx[0].oow = (float)(1.0 / w);
		out->tmuvtx[0].sow = (float)(in->tmuvtx[0].sow / w);
		out->tmuvtx[0].tow = (float)(in->tmuvtx[0].tow / w);
	}
}

Matrix44 Matrix44::Inverse()
{
	// Transpose the matrix.
	Matrix44 inv = Matrix44();
	double det;

	inv.data[0][0] = data[1][1]  * data[2][2] * data[3][3] - 
			 data[1][1]  * data[2][3] * data[3][2] - 
			 data[2][1]  * data[1][2]  * data[3][3] + 
			 data[2][1]  * data[1][3]  * data[3][2] +
			 data[3][1] * data[1][2]  * data[2][3] - 
			 data[3][1] * data[1][3]  * data[2][2];

	inv.data[1][0] = -data[1][0]  * data[2][2] * data[3][3] + 
			  data[1][0]  * data[2][3] * data[3][2] + 
			  data[2][0]  * data[1][2]  * data[3][3] - 
			  data[2][0]  * data[1][3]  * data[3][2] - 
			  data[3][0] * data[1][2]  * data[2][3] + 
			  data[3][0] * data[1][3]  * data[2][2];

	inv.data[2][0] = data[1][0]  * data[2][1] * data[3][3] - 
			 data[1][0]  * data[2][3] * data[3][1] - 
			 data[2][0]  * data[1][1] * data[3][3] + 
			 data[2][0]  * data[1][3] * data[3][1] + 
			 data[3][0] * data[1][1] * data[2][3] - 
			 data[3][0] * data[1][3] * data[2][1];

	inv.data[3][0] = -data[1][0]  * data[2][1] * data[3][2] + 
			   data[1][0]  * data[2][2] * data[3][1] +
			   data[2][0]  * data[1][1] * data[3][2] - 
			   data[2][0]  * data[1][2] * data[3][1] - 
			   data[3][0] * data[1][1] * data[2][2] + 
			   data[3][0] * data[1][2] * data[2][1];

	inv.data[0][1] = -data[0][1]  * data[2][2] * data[3][3] + 
			  data[0][1]  * data[2][3] * data[3][2] + 
			  data[2][1]  * data[0][2] * data[3][3] - 
			  data[2][1]  * data[0][3] * data[3][2] - 
			  data[3][1] * data[0][2] * data[2][3] + 
			  data[3][1] * data[0][3] * data[2][2];

	inv.data[1][1] = data[0][0]  * data[2][2] * data[3][3] - 
			 data[0][0]  * data[2][3] * data[3][2] - 
			 data[2][0]  * data[0][2] * data[3][3] + 
			 data[2][0]  * data[0][3] * data[3][2] + 
			 data[3][0] * data[0][2] * data[2][3] - 
			 data[3][0] * data[0][3] * data[2][2];

	inv.data[2][1] = -data[0][0]  * data[2][1] * data[3][3] + 
			  data[0][0]  * data[2][3] * data[3][1] + 
			  data[2][0]  * data[0][1] * data[3][3] - 
			  data[2][0]  * data[0][3] * data[3][1] - 
			  data[3][0] * data[0][1] * data[2][3] + 
			  data[3][0] * data[0][3] * data[2][1];

	inv.data[3][1] = data[0][0]  * data[2][1] * data[3][2] - 
			  data[0][0]  * data[2][2] * data[3][1] - 
			  data[2][0]  * data[0][1] * data[3][2] + 
			  data[2][0]  * data[0][2] * data[3][1] + 
			  data[3][0] * data[0][1] * data[2][2] - 
			  data[3][0] * data[0][2] * data[2][1];

	inv.data[0][2] = data[0][1]  * data[1][2] * data[3][3] - 
			 data[0][1]  * data[1][3] * data[3][2] - 
			 data[1][1]  * data[0][2] * data[3][3] + 
			 data[1][1]  * data[0][3] * data[3][2] + 
			 data[3][1] * data[0][2] * data[1][3] - 
			 data[3][1] * data[0][3] * data[1][2];

	inv.data[1][2] = -data[0][0]  * data[1][2] * data[3][3] + 
			  data[0][0]  * data[1][3] * data[3][2] + 
			  data[1][0]  * data[0][2] * data[3][3] - 
			  data[1][0]  * data[0][3] * data[3][2] - 
			  data[3][0] * data[0][2] * data[1][3] + 
			  data[3][0] * data[0][3] * data[1][2];

	inv.data[2][2] = data[0][0]  * data[1][1] * data[3][3] - 
			  data[0][0]  * data[1][3] * data[3][1] - 
			  data[1][0]  * data[0][1] * data[3][3] + 
			  data[1][0]  * data[0][3] * data[3][1] + 
			  data[3][0] * data[0][1] * data[1][3] - 
			  data[3][0] * data[0][3] * data[1][1];

	inv.data[3][2] = -data[0][0]  * data[1][1] * data[3][2] + 
			   data[0][0]  * data[1][2] * data[3][1] + 
			   data[1][0]  * data[0][1] * data[3][2] - 
			   data[1][0]  * data[0][2] * data[3][1] - 
			   data[3][0] * data[0][1] * data[1][2] + 
			   data[3][0] * data[0][2] * data[1][1];

	inv.data[0][3] = -data[0][1] * data[1][2] * data[2][3] + 
			  data[0][1] * data[1][3] * data[2][2] + 
			  data[1][1] * data[0][2] * data[2][3] - 
			  data[1][1] * data[0][3] * data[2][2] - 
			  data[2][1] * data[0][2] * data[1][3] + 
			  data[2][1] * data[0][3] * data[1][2];

	inv.data[1][3] = data[0][0] * data[1][2] * data[2][3] - 
			 data[0][0] * data[1][3] * data[2][2] - 
			 data[1][0] * data[0][2] * data[2][3] + 
			 data[1][0] * data[0][3] * data[2][2] + 
			 data[2][0] * data[0][2] * data[1][3] - 
			 data[2][0] * data[0][3] * data[1][2];

	inv.data[2][3] = -data[0][0] * data[1][1] * data[2][3] + 
			   data[0][0] * data[1][3] * data[2][1] + 
			   data[1][0] * data[0][1] * data[2][3] - 
			   data[1][0] * data[0][3] * data[2][1] - 
			   data[2][0] * data[0][1] * data[1][3] + 
			   data[2][0] * data[0][3] * data[1][1];

	inv.data[3][3] = data[0][0] * data[1][1] * data[2][2] - 
			  data[0][0] * data[1][2] * data[2][1] - 
			  data[1][0] * data[0][1] * data[2][2] + 
			  data[1][0] * data[0][2] * data[2][1] + 
			  data[2][0] * data[0][1] * data[1][2] - 
			  data[2][0] * data[0][2] * data[1][1];

	det = data[0][0] * inv.data[0][0] + data[0][1] * inv.data[1][0] + data[0][2] * inv.data[2][0] + data[0][3] * inv.data[3][0];
	det = 1.0 / det;

	return inv;
}

void Matrix44::Print()
{

}

Matrix44 Matrix44::Transpose()
{
	Matrix44 result = Matrix44();

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			result.data[j][i] = data[i][j];
		}
	}

	return result;
}

Matrix44 Matrix44::Multiply(Matrix44 *b)
{
	Matrix44 result = Matrix44();

	for(int rightRow=0; rightRow<4; rightRow++)
	{
		for(int r=0; r<4; r++)
		{
			for(int c=0; c<4; c++)
			{
				result.data[r][rightRow] += this->data[r][c] * b->data[c][rightRow];
			}
		}
	}

	return result;
}