#pragma once

#include <glide.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <list>

#pragma message disable REFTEMPORARY

#include <boost/algorithm/string.hpp>

#include "Types3D.h"

class WavefrontObj
{
	typedef enum {
		WF_ERROR,
		WF_GROUP,
		WF_VERTEXGEOMETRY,
		WF_VERTEXNORMAL,
		WF_VERTEXTEXTURE,
		WF_FACE,
		WF_COMMENT,
		WF_USEMTL,
		WF_OBJECT,
		WF_MTLLIB,
		WF_S,
	} WFObjLineToken;

	typedef enum {
		WFM_ERROR,
		WFM_NEWMTL,
		WFM_Ns,
		WFM_Ka,
		WFM_Kd,
		WFM_Ks,
		WFM_Ke,
		WFM_Ni,
		WFM_D,
		WFM_ILLUM,
		WFM_COMMENT,
		WFM_MAP_Kd,
	} WFMaterialLineToken;

	std::vector<Types3D::WFMaterial *> materials;
	Types3D::WFMaterial *activeMaterial;

	std::fstream fileStream;
	std::fstream materialStream;

	std::map<WFObjLineToken, std::string> lines;
	WFObjLineToken GetLineToken(std::string line);

	/* Parse line functions */
	GrVertex ParseLineVertex(std::string line);
	void ParseLineFace(std::string line);

	/* Material functions */
	void LoadMaterialLibrary(std::string basePath, std::string line);
	WFMaterialLineToken GetMaterialToken(std::string line);
	void SetActiveMaterial(std::string line);

public:
	/* Vertexes vectors */
	std::vector<GrVertex> vertexesGeometry;
	std::vector<GrVertex> vertexesTexture;
	std::vector<GrVertex> vertexesNormal;

	/* Faces vectors */
	std::vector<Types3D::FaceTriangle> facesGeometry;
	std::vector<Types3D::FaceTriangle> facesTexture;
	std::vector<Types3D::FaceTriangle> facesNormal;

	WavefrontObj(const char *path);
	~WavefrontObj(void);
};
