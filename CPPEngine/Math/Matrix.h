// Matrix44.h: interface for the Matrix44 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Matrix44_H__513CD7B8_3453_416D_888A_B818B5C673BA__INCLUDED_)
#define AFX_Matrix44_H__513CD7B8_3453_416D_888A_B818B5C673BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <glide.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "Math3D.h"

class Matrix44  
{
private:
	int rowCount;
	int colCount;

public:
	float data[4][4];

	Matrix44()
	{
		rowCount = 4;
		colCount = 4;

		for(int i=0; i<rowCount; i++)
		{
			for(int j=0; j<colCount; j++)
			{
				data[i][j] = 0;
			}
		}
	}

	void Identity();
	void TransformNormal(const GrVertex *in, GrVertex *out);
	void TransformPoint(const GrVertex *in, GrVertex *out);

	~Matrix44() {}

	static Matrix44 Translation(Vector3f v);
	static Matrix44 Translation(float x, float y, float z);

	static Matrix44 Scale(Vector3f v);
	static Matrix44 Scale(float x, float y, float z);

	static Matrix44 Rotation(Vector3f v);
	static Matrix44 Rotation(float yaw, float pitch, float roll);
	
	static Matrix44 RotationX(float theta);
	static Matrix44 RotationY(float theta);
	static Matrix44 RotationZ(float theta);

	Matrix44 Inverse();
	void Print();
	Matrix44 Transpose();
	Matrix44 Multiply(Matrix44 *b);
};

#endif // !defined(AFX_MATRIX_H__513CD7B8_3453_416D_888A_B818B5C673BA__INCLUDED_)
