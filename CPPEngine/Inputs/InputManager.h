#pragma once

#include "Math/Vector3f.h"
#include "Types3D.h"
#include "globals.h"

class SimulationObject;
extern SimulationObject *playerSimobj;

typedef struct
{
	Vector3f axis_3;	// X, Y, and Z axes. (-1 to 1) range for setting in all three directions.
						// X and Y are the joystick axes, Z is the throttle axis.
} FlightControls;

class InputManager
{
public:
	bool *keyPressTable;
	POINT mousePosition;

	FlightControls flightControls;

	InputManager(void);
	~InputManager(void);

	void UpdateGameInputs();

	void DrawMouseCursor();

	void HandlePressedKeys();
};

extern InputManager g_InputManager;