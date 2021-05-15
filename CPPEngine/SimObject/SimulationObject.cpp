#include "SimulationObject.h"

SimulationObject::SimulationObject(std::string name, Model3D *model)
{
	this->name = name;
	this->model = model;
	visible = true;
	is_player = false;
	can_be_targeted = true;
	is_physics_object = false;

	current_target = NULL;

	team = TEAM_NULL;
}

SimulationObject::~SimulationObject(void)
{
}

void SimulationObject::Draw(Matrix44 *world, Matrix44 *projection)
{
	if(model != NULL)
	{
		model->Draw(&this->transformation, world, projection);
	}
}

void SimulationObject::ApplyInputs()
{
/*
	if(g_InputManager.keyPressTable[VK_LEFT])
	{
		transformation.position.x -= 0.5;
	}
	if(g_InputManager.keyPressTable[VK_RIGHT])
	{
		transformation.position.x += 0.5;
	}
	if(g_InputManager.keyPressTable[VK_UP])
	{
		transformation.position.y -= 0.5;
	}
	if(g_InputManager.keyPressTable[VK_DOWN])
	{
		transformation.position.y += 0.5;
	}
*/
}