#pragma once

#include "Math/Matrix.h"
#include "Math/Vector3f.h"
#include "Types3D.h"
#include "Model/WavefrontObj.h"
#include "Model/Model3D.h"
#include "Inputs/InputManager.h"
#include "SimObject/PhysicsSim.h"

typedef enum
{
	TEAM_NULL,
	TEAM_GOOD,
	TEAM_BAD,
	TEAM_NEUTRAL,
} SOTeam;

class SimulationObject
{
private:
	Model3D *model;

public:
	std::string name;
	Transformation transformation;

	/* flags */
	bool visible;
	bool is_player;
	bool can_be_targeted;
	bool is_physics_object;
	
	SOTeam team;
	PhysicsData physics;
	SimulationObject *current_target;

	//////////////////////////////
	void ApplyInputs();

	// event functions implemented by child classes
	virtual void OnFrame() {};
	virtual void Draw(Matrix44 *, Matrix44 *);

	Model3D *GetModel() { return model; } 

	SimulationObject(std::string name, Model3D *model);
	virtual ~SimulationObject(void);
};
