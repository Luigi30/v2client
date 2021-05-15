#pragma once

#include "Types3D.h"
#include "Math/Matrix.h"

class Renderer
{
public:
	// TODO: Matrix stacks.
								// MODEL matrix is simobj->transformation.
	Matrix44 mtxWorld;			// Current VIEW matrix.
	Matrix44 mtxProjection;		// Current PROJECTION matrix.

	Renderer(void);
	~Renderer(void);

	// Matrix setup
	void CreateWorldMatrix(Matrix44* m, Vector3f localEye, Vector3f localTarget);
	void CreateProjectionMatrix(Matrix44* m);

	void LightTriangle(Types3D::Triangle *tri);
	void TextureTriangle(Types3D::Triangle *geometry_tri, Types3D::Triangle *texture_tri);
	void ProjectTriangleToCameraSpace(Types3D::Triangle *tri);
	void ProjectPolygonToCameraSpace(GrVertex *verts, int numVerts);
};

extern Renderer g_Renderer;