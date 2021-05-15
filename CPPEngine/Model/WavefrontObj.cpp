#include "WavefrontObj.h"

#ifdef __GNUC__
#include <libgen.h>
#endif

#ifdef _MSC_VER
#include <shlwapi.h>
#endif


WavefrontObj::WFObjLineToken WavefrontObj::GetLineToken(std::string line)
{
	std::istringstream iss(line);
	std::string token;
	iss >> token;

	if(token == "G")		return WF_GROUP;
	if(token == "V")		return WF_VERTEXGEOMETRY;
	if(token == "VN")		return WF_VERTEXNORMAL;
	if(token == "VT")		return WF_VERTEXTEXTURE;
	if(token == "F")		return WF_FACE;
	if(token == "#")		return WF_COMMENT; 
	if(token == "O")		return WF_OBJECT;
	if(token == "USEMTL")	return WF_USEMTL;
	if(token == "MTLLIB")	return WF_MTLLIB;
	if(token == "S")		return WF_S;

	fputs("Encountered unknown .obj token!", stderr);
	abort();
	return WF_ERROR;
}

WavefrontObj::WFMaterialLineToken WavefrontObj::GetMaterialToken(std::string line)
{
	std::istringstream iss(line);
	std::string token;
	iss >> token;

	if(token == "NEWMTL")	return WFM_NEWMTL;
	if(token == "NS")		return WFM_Ns;
	if(token == "KA")		return WFM_Ka;
	if(token == "KD")		return WFM_Kd;
	if(token == "KS")		return WFM_Ks;
	if(token == "KE")		return WFM_Ke;
	if(token == "NI")		return WFM_Ni;
	if(token == "D")		return WFM_D;
	if(token == "ILLUM")	return WFM_ILLUM;
	if(token == "#")		return WFM_COMMENT;
	if(token == "MAP_KD")	return WFM_MAP_Kd;
	fputs("Encountered unknown .mtl token!", stderr);
	abort();
	return WFM_ERROR;
}


WavefrontObj::WavefrontObj(const char *path)
{
	// Working with .obj files so ASCII mode, not binary
	fileStream.open(path, std::fstream::in);


	/* Parse it into the std::map */
	std::string line;

	while(std::getline(fileStream, line))
	{
		// Ignore empty lines.
		if(line == "") { continue; }
		boost::to_upper(line);
		WavefrontObj::WFObjLineToken token = GetLineToken(line);

		switch(token)
		{
			case WF_VERTEXGEOMETRY:
				vertexesGeometry.push_back(ParseLineVertex(line));
				break;
			case WF_VERTEXNORMAL:
				vertexesNormal.push_back(ParseLineVertex(line));
				break;
			case WF_VERTEXTEXTURE:
				vertexesTexture.push_back(ParseLineVertex(line));
				break;
			case WF_FACE:
				ParseLineFace(line);
				break;
			case WF_MTLLIB:
				LoadMaterialLibrary(path, line);
				break;
			case WF_USEMTL:
				SetActiveMaterial(line);
				break;
			default:
				// ignore
				break;
		}
	}
}

WavefrontObj::~WavefrontObj(void)
{
}

void WavefrontObj::SetActiveMaterial(std::string line)
{
	// Get the material name from the line.
	// Find the material in the materials vector.
	// Retrieve it and make it the active material.

	std::istringstream iss(line);
	std::string token;
	iss >> token;	// discard one token
	iss >> token;	// the material name

	for (std::vector<Types3D::WFMaterial *>::iterator it=materials.begin(); it != materials.end(); ++it)
	{
		if((*it)->name == token)
		{
			activeMaterial = (Types3D::WFMaterial *)(*it);
		}
	}
}

void WavefrontObj::LoadMaterialLibrary(std::string basePath, std::string line)
{
	std::istringstream iss(line);
	std::string token;
	iss >> token; // discard one token
	iss >> token;

	// Construct the path from the basePath's directory.
	char *path_to_material_directory = (char *)basePath.c_str();
	char constructed_path[1024];
	char material_file[512];
	
	boost::to_lower(token);
#ifdef _MSC_VER
	PathRemoveFileSpecA(path_to_material_directory);
	PathCombineA(constructed_path, path_to_material_directory, token.c_str());
#else
	strncpy(material_file, token.c_str(), 511);
	strncpy(constructed_path, dirname(path_to_material_directory), 511);
	strncat(constructed_path, "/", 2);
	strncat(constructed_path, material_file, 511);
#endif

	materialStream.open(constructed_path, std::fstream::in);

	// TODO: a different function
	// Read the materials from the library.
	std::string material_line;

	Types3D::WFMaterial *material = NULL;

	while(std::getline(materialStream, material_line))
	{
		// Ignore empty lines.
		if(material_line == "") { continue; }
		boost::to_upper(material_line);
		WavefrontObj::WFMaterialLineToken token = GetMaterialToken(material_line);

		std::istringstream iss(material_line);
		std::string value;
		float f1, f2, f3;
		int illum;

		switch(token)
		{
			case WFM_NEWMTL:
				// All materials start with NEWMTL.
				// Insert current material into list, create a new material.
				if(material != NULL)
				{
					materials.push_back(material);
				}
				material = new Types3D::WFMaterial;
				iss >> value >> value;
				material->map_Kd = "";
				material->name = value;
				break;
			case WFM_Ka:
				iss >> value >> f1 >> f2 >> f3;
				material->Ka = Vector3f(f1, f2, f3);
				break;
			case WFM_Kd:
				iss >> value >> f1 >> f2 >> f3;
				material->Kd = Vector3f(f1, f2, f3);
				break;
			case WFM_Ks:
				iss >> value >> f1 >> f2 >> f3;
				material->Ks = Vector3f(f1, f2, f3);
				break;
			case WFM_Ke:
				iss >> value >> f1 >> f2 >> f3;
				material->Ke = Vector3f(f1, f2, f3);
				break;
			case WFM_Ns:
				iss >> value >> f1;
				material->Ns = f1;
				break;
			case WFM_Ni:
				iss >> value >> f1;
				material->Ni = f1;
				break;
			case WFM_D:
				iss >> value >> f1;
				material->D = f1;
				break;
			case WFM_MAP_Kd:
				iss >> value >> value;
				material->map_Kd = value;
				break;
			case WFM_ILLUM:
				iss >> value >> illum;
				material->illum = illum;
				break;
			default:
				// ignore
				break;
		}
	}

	if(material != NULL)
	{
		materials.push_back(material);
	}
}

/* Line parsing */
GrVertex WavefrontObj::ParseLineVertex(std::string line)
{
	std::istringstream iss(line);
	std::string discard;

	GrVertex v;
	iss >> discard >> v.x >> v.y >> v.z;
	return v;
}

void WavefrontObj::ParseLineFace(std::string line)
{
	std::istringstream iss(line);
	std::string discard;
	std::string faces[3];
	std::vector<std::string> splitStrings;

	Types3D::FaceTriangle geometry, texture, normal;

	iss >> discard >> faces[0] >> faces[1] >> faces[2];

	for(int i=0; i<3; i++)
	{
		boost::algorithm::split(splitStrings, faces[i], boost::algorithm::is_any_of("/"));
		
		geometry.pointIdx[i] = std::atoi(splitStrings[0].c_str());
		texture.pointIdx[i] = std::atoi(splitStrings[1].c_str());
		normal.pointIdx[i] = std::atoi(splitStrings[2].c_str());

		geometry.material = activeMaterial;
		texture.material = activeMaterial;
		normal.material = activeMaterial;
	}

	facesGeometry.push_back(geometry);
	facesTexture.push_back(texture);
	facesNormal.push_back(normal);
}