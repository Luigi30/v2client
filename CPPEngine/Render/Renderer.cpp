#include "Renderer.h"
#include "Model/Model3D.h"

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::LightTriangle(Types3D::Triangle *tri)
{
	// Use the diffuse color from the material associated with the face.

	// TODO: hook up actual lighting
	// TODO: read wikipedia article on computer graphics lighting and
	//		 type a bunch of stuff into snack overflow????
	 
	for(int i=0; i<3; i++)
	{
		tri->vertexes[i].r = tri->material->Kd.x * 255;
		tri->vertexes[i].g = tri->material->Kd.y * 255;
		tri->vertexes[i].b = tri->material->Kd.z * 255;
		tri->vertexes[i].a = tri->material->D * 255;
	}
}

void Renderer::TextureTriangle(Types3D::Triangle *geometry_tri, Types3D::Triangle *texture_tri)
{
	// Copy the TMUVTX values to our vertexes */
	for(int i=0; i<3; i++)
	{
		geometry_tri->vertexes[i].tmuvtx[0].sow = texture_tri->vertexes[i].x * 256.0;
		geometry_tri->vertexes[i].tmuvtx[0].tow = texture_tri->vertexes[i].y * 256.0;
		geometry_tri->vertexes[i].tmuvtx[0].oow = 1;
	}
}

void Renderer::ProjectTriangleToCameraSpace(Types3D::Triangle *tri)
{
	for(int i=0; i<3; i++)
	{
		tri->vertexes[i];
		tri->vertexes[i].x = (tri->vertexes[i].x * 800) + (800 / 2);
		tri->vertexes[i].y = (tri->vertexes[i].y * 600) + (600 / 2);
		tri->vertexes[i].oow = 1;
		tri->vertexes[i].ooz = 1 / tri->vertexes[i].z;
	}
}

void Renderer::ProjectPolygonToCameraSpace(GrVertex *verts, int numVerts)
{
	for(int i=0; i<numVerts; i++)
	{
		verts[i].x = (verts[i].x * 800) + (800 / 2);
		verts[i].y = (verts[i].y * 600) + (600 / 2);
		verts[i].oow = 1;
	}
}

void Renderer::CreateWorldMatrix(Matrix44* m, Vector3f localEye, Vector3f localTarget)
{
	Vector3f up = Vector3f::Up();

	Vector3f Z = (localEye - localTarget).Normalize();
	Vector3f X = Vector3f::CrossProduct(up, Z).Normalize();
	Vector3f Y = Vector3f::CrossProduct(Z, X);

	Matrix44 mWorldOrientation = Matrix44();
	mWorldOrientation.Identity();
	mWorldOrientation.data[0][0] = X.x;
	mWorldOrientation.data[0][1] = Y.x;
	mWorldOrientation.data[0][2] = Z.x;
	mWorldOrientation.data[1][0] = X.y;
	mWorldOrientation.data[1][1] = Y.y;
	mWorldOrientation.data[1][2] = Z.y;
	mWorldOrientation.data[2][0] = X.z;
	mWorldOrientation.data[2][1] = Y.z;
	mWorldOrientation.data[2][2] = Z.z;

	Matrix44 mWorldTranslation = Matrix44();
	mWorldTranslation.Identity();
	mWorldTranslation.data[3][0] = localEye.x;
	mWorldTranslation.data[3][1] = localEye.y;
	mWorldTranslation.data[3][2] = localEye.z;

	*m = mWorldTranslation.Multiply(&mWorldOrientation);
}

void Renderer::CreateProjectionMatrix(Matrix44* m)
{
	const float clipNear = 1.0f;
	const float clipFar = 500.0f;

	const float right = 1.33;
	const float left = -1.33;
	const float top = 1;
	const float bottom = -1;

	// Projection matrix: Perspective projection.
	m->Identity();

	m->data[0][0] = (2*clipNear)/(right - left);
	m->data[1][1] = (2*clipNear)/(top - bottom);
	m->data[2][0] = (right+left)/(right-left);
	m->data[2][1] = (top+bottom)/(top-bottom);
	m->data[2][2] = -((clipFar+clipNear) / (clipFar-clipNear));
	m->data[2][3] = -1;
	m->data[3][2] = -((2*clipFar*clipNear)/(clipFar-clipNear));
	m->data[3][3] = 0;
}

Renderer g_Renderer;