#pragma once

#include "SimObject/SimulationObject.h"
#include "Model/Model3D.h"

class JackCube : public SimulationObject
{
public:
	JackCube(std::string name, Model3D *model) : SimulationObject(name, model) {};
	~JackCube(void) {}

	void OnFrame()
	{
		//transformation.rotation.y++;
		if(transformation.rotation.y > 360) transformation.rotation.y -= 360;
	}
};
