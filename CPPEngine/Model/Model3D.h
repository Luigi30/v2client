#pragma once

#include <cassert>
#include <vector>
#include <limits>

#include <glide.h>

#include "Math/Matrix.h"
#include "Render/Renderer.h"
#include "globals.h"

class WavefrontObj;

class Model3D
{
public:
	typedef enum
	{
		VTX_GEOMETRY,
		VTX_NORMAL,
		VTX_TEXTURE
	} VertexType;

private:
	typedef struct
	{
		std::vector<GrVertex> geometry;
		std::vector<GrVertex> normal;
		std::vector<GrVertex> texture;
	} Points;

	typedef struct
	{
		std::vector<Types3D::FaceTriangle> geometry;
		std::vector<Types3D::FaceTriangle> normal;
		std::vector<Types3D::FaceTriangle> texture;
	} Faces;

	std::vector<GrVertex>* GetPointsVector(VertexType pointType);
	std::vector<Types3D::FaceTriangle>* GetFacesVector(VertexType faceType);

public:
	Model3D(std::string name, WavefrontObj *wavefront);

	Points			points;
	Faces			faces;

	unsigned int GetFaceCount(VertexType faceType);

	size_t AddTriangle(Types3D::Triangle tri, VertexType pointType);
	size_t AddFace(Types3D::FaceTriangle tri, VertexType pointType);

	Types3D::FaceTriangle GetFace(unsigned int faceIdx, VertexType faceType);
	Types3D::Triangle GetVertexesOfFace(Types3D::FaceTriangle face, VertexType faceType);
	
	Types3D::Triangle GetTransformedFace(unsigned int faceIdx,
						Transformation *tLocal,		// M
						Matrix44 *tWorld,			// V
						Matrix44 *tProjection);		// P
	
	void ProjectTriangleToCameraSpace(Types3D::Triangle *tri);
	void TextureTriangle(Types3D::Triangle *tri, unsigned int faceIdx);

	Types3D::Rectangle Get2DBoundingBox(Transformation *localTransform);

	void Draw(Transformation *localTransform, Matrix44 *world, Matrix44 *projection);
};
