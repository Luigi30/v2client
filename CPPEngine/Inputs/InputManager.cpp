#include "InputManager.h"
#include "SimObject/SimulationObjectList.h"
#include "Glide/GlidePrimitives.h"

extern SimulationObject *playerSimobj;

InputManager::InputManager(void)
{
	keyPressTable = new bool[256];
	for (int i = 0; i < 255; i++)
	{
		keyPressTable[i] = false;
	}

	mousePosition.x = 400;
	mousePosition.y = 300;
}

InputManager::~InputManager(void)
{
}

void InputManager::UpdateGameInputs()
{
	// Process X/Y axes.
	flightControls.axis_3.x = -(mousePosition.x - 400.0f) / 400.0f;
	flightControls.axis_3.y = (mousePosition.y - 300.0f) / 300.0f;

	flightControls.axis_3.z = 0;
	if(keyPressTable['a'])
	{
		flightControls.axis_3.z = 1.0f;
	}
	else if(keyPressTable['z'])
	{
		flightControls.axis_3.z = -1.0f;
	}

	HandlePressedKeys();
}

void InputManager::HandlePressedKeys()
{
	// All the game controls and junk that's separate from movement axes.
	if(keyPressTable['t'])
	{
		// Change the player's current_target to the next valid target.
		if(playerSimobj->current_target == NULL)
		{
			SimulationObjectIterator it = simObjectList.GetBeginning();
			for (it; it != simObjectList.GetEnding(); ++it) {
				if((*it)->can_be_targeted)
				{
					playerSimobj->current_target = *it;
				}
			}
		}
		
		keyPressTable['t'] = false;
	}
	
}

void InputManager::DrawMouseCursor()
{
	GlidePrimitives::DrawLine(mousePosition.x - 10,
							  600 - mousePosition.y,
							  mousePosition.x + 10,
							  600 - mousePosition.y,
							  0xFFFFFFFF);

	GlidePrimitives::DrawLine(mousePosition.x,
							  600 - mousePosition.y - 10,
							  mousePosition.x,
							  600 - mousePosition.y + 10,
							  0xFFFFFFFF);
}

InputManager g_InputManager;