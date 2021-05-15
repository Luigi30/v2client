#include "Model/Model3D.h"
#include "Model/WavefrontObj.h"

Model3D::Model3D(std::string name, WavefrontObj *wavefront)
{
	/* Populate the Points and Faces with the .OBJ data */
	for (std::vector<GrVertex>::iterator it=wavefront->vertexesGeometry.begin(); it != wavefront->vertexesGeometry.end(); ++it)
	{
		points.geometry.push_back(*it);
	}
	for (std::vector<GrVertex>::iterator it=wavefront->vertexesNormal.begin(); it != wavefront->vertexesNormal.end(); ++it)
	{
		points.normal.push_back(*it);
	}
	for (std::vector<GrVertex>::iterator it=wavefront->vertexesTexture.begin(); it != wavefront->vertexesTexture.end(); ++it)
	{
		points.texture.push_back(*it);
	}

	/* Faces */
	for (std::vector<Types3D::FaceTriangle>::iterator it=wavefront->facesGeometry.begin(); it != wavefront->facesGeometry.end(); ++it)
	{
		faces.geometry.push_back(*it);
	}
	for (std::vector<Types3D::FaceTriangle>::iterator it=wavefront->facesNormal.begin(); it != wavefront->facesNormal.end(); ++it)
	{
		faces.normal.push_back(*it);
	}
	for (std::vector<Types3D::FaceTriangle>::iterator it=wavefront->facesTexture.begin(); it != wavefront->facesTexture.end(); ++it)
	{
		faces.texture.push_back(*it);
	}
}

unsigned int Model3D::GetFaceCount(VertexType faceType)
{
	assert(faceType == VTX_GEOMETRY || faceType == VTX_NORMAL || faceType == VTX_TEXTURE);

	switch(faceType)
	{
	case VTX_GEOMETRY:
		return (unsigned int)faces.geometry.size();
	case VTX_NORMAL:
		return (unsigned int)faces.normal.size();
	case VTX_TEXTURE:
		return (unsigned int)faces.texture.size();
	default:
		return 0;
	}
}

std::vector<GrVertex>* Model3D::GetPointsVector(VertexType pointType)
{
	switch(pointType)
	{
	case VTX_GEOMETRY:
		return &points.geometry;
	case VTX_NORMAL:
		return &points.normal;
	case VTX_TEXTURE:
		return &points.texture;
	default:
		return NULL; /* ??? */
	}
}

std::vector<Types3D::FaceTriangle>* Model3D::GetFacesVector(VertexType faceType)
{
	switch(faceType)
	{
	case VTX_GEOMETRY:
		return &faces.geometry;
	case VTX_NORMAL:
		return  &faces.normal;
	case VTX_TEXTURE:
		return &faces.texture;
	default:
		return NULL; /* ??? */
	}
}

size_t Model3D::AddTriangle(Types3D::Triangle tri, VertexType pointType)
{
	/* Add one triangle to the specified vector. */
	/* Returns the new size of the vector. */

	std::vector<GrVertex> *target = this->GetPointsVector(pointType);

	/* Add the three points to the specified Points vector */
	target->push_back(tri.vertexes[0]);
	target->push_back(tri.vertexes[1]);
	target->push_back(tri.vertexes[2]);

	return target->size();
}

size_t Model3D::AddFace(Types3D::FaceTriangle tri, VertexType faceType)
{
	std::vector<Types3D::FaceTriangle> *target = this->GetFacesVector(faceType);

	/* Add the three points to the specified Points vector */
	target->push_back(tri);

	return target->size();
}

Types3D::Triangle Model3D::GetVertexesOfFace(Types3D::FaceTriangle face,
											 VertexType faceType)
{
	std::vector<GrVertex> *pointsTarget = this->GetPointsVector(faceType);

	Types3D::Triangle tri;
	/* faces are 1 indexed :( */
	tri.vertexes[0] = pointsTarget->at(face.pointIdx[0]-1);
	tri.vertexes[1] = pointsTarget->at(face.pointIdx[1]-1);
	tri.vertexes[2] = pointsTarget->at(face.pointIdx[2]-1);
	return tri;
}

/* Transformation */
Types3D::FaceTriangle Model3D::GetFace(unsigned int faceIdx, VertexType faceType)
{
	std::vector<GrVertex> *pointsTarget = this->GetPointsVector(faceType);
	std::vector<Types3D::FaceTriangle> *facesTarget = this->GetFacesVector(faceType);

	return facesTarget->at(faceIdx);
}

Types3D::Triangle Model3D::GetTransformedFace(
											unsigned int faceIdx,
											Transformation *tLocal,	// M
											Matrix44 *tWorld,		// V
											Matrix44 *tProjection)	// P
{
	Matrix44 mFinal = Matrix44();
	mFinal.Identity();

	// Construct a transform from our model's transform.
	Matrix44 modelT = Matrix44();
	modelT.Identity();

	/* Perform the model's transformation... */
	Matrix44 mScale = Matrix44::Scale(tLocal->scale);
	Matrix44 mRotation = Matrix44::Rotation(tLocal->rotation);
	Matrix44 mTranslation = Matrix44::Translation(tLocal->position);

	modelT = modelT.Multiply(&mScale);
	modelT = modelT.Multiply(&mRotation);
	modelT = modelT.Multiply(&mTranslation);

	Types3D::FaceTriangle face = GetFace(faceIdx, VTX_GEOMETRY);
	Types3D::Triangle triangle = GetVertexesOfFace(face, VTX_GEOMETRY);
	Types3D::Triangle output = GetVertexesOfFace(face, VTX_GEOMETRY);

	mFinal = mFinal.Multiply(&modelT);
	mFinal = mFinal.Multiply(tWorld);

	// If we have a projection, use it.
	if(tProjection != NULL) mFinal = mFinal.Multiply(tProjection);
	output.material = face.material;

	// Transform each of the points and return the transformed points.
	for(int i=0; i<3; i++)
	{
		mFinal.TransformPoint(&triangle.vertexes[i], &output.vertexes[i]);
	}
	return output;
}

Types3D::Rectangle Model3D::Get2DBoundingBox(Transformation *localTransform)
{
	// Determine the bounding box of this model with the current MVP matrix set.
	Types3D::Triangle tri;
	unsigned int faces = GetFaceCount(Model3D::VTX_GEOMETRY);

	Types3D::Rectangle r;
	r.x1 = 10000000.0f;
	r.y1 = 10000000.0f;
	r.x2 = -10000000.0f;
	r.y2 = -10000000.0f;

	for(unsigned int i=0; i<faces; i++)
	{
		tri = GetTransformedFace(i, localTransform, &g_Renderer.mtxWorld, &g_Renderer.mtxProjection);
		g_Renderer.ProjectTriangleToCameraSpace(&tri);

		for(int v=0; v<3; v++)
		{
			if(tri.vertexes[v].x < r.x1) r.x1 = tri.vertexes[v].x;
			if(tri.vertexes[v].y < r.y1) r.y1 = tri.vertexes[v].y;

			if(tri.vertexes[v].x > r.x2) r.x2 = tri.vertexes[v].x;
			if(tri.vertexes[v].y > r.y2) r.y2 = tri.vertexes[v].y;
		}
	}

	return r;
}

/* Draw routines */
void Model3D::Draw(Transformation *localTransform, Matrix44 *world, Matrix44 *projection)
{
	/* Draw the polygons that make up this model to the framebuffer. */
	/* Returns the number of rendered polygons. */

	Types3D::Triangle tri;
	unsigned int faces = GetFaceCount(Model3D::VTX_GEOMETRY);
	for(unsigned int i=0; i<faces; i++)
	{
		Types3D::FaceTriangle face = GetFace(i, VTX_GEOMETRY);
		// TODO: Dynamically set grColorCombine depending on whether
		// the model has a texture or just solid-color materials
		{
			
			if(face.material->map_Kd != "")
			{
				grColorCombine( 
					GR_COMBINE_FUNCTION_SCALE_OTHER,
					GR_COMBINE_FACTOR_ONE,
					GR_COMBINE_LOCAL_NONE,
					GR_COMBINE_OTHER_TEXTURE,
					FXFALSE
				);
			}
			else
			{
				grColorCombine(
					GR_COMBINE_FUNCTION_LOCAL,
					GR_COMBINE_FACTOR_NONE,
					GR_COMBINE_LOCAL_ITERATED, 
					GR_COMBINE_OTHER_NONE, 
					FXFALSE
				);
			}
		}

		tri = GetTransformedFace(i, localTransform, world, projection);
		g_Renderer.LightTriangle(&tri);
		{
			Types3D::FaceTriangle texture_face	= GetFace(i, Model3D::VTX_TEXTURE);
			Types3D::Triangle texture_tri		= GetVertexesOfFace(texture_face, Model3D::VTX_TEXTURE);
			g_Renderer.TextureTriangle(&tri, &texture_tri);
		}
		g_Renderer.ProjectTriangleToCameraSpace(&tri);
		grDrawTriangle(&tri.vertexes[0], &tri.vertexes[1], &tri.vertexes[2]);
	}
}