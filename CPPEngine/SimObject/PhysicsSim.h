#pragma once

#include "Inputs/InputManager.h"
#include "Math/Math3D.h"

// Data for keeping track of a SimObject's physics state.
struct PhysicsData
{
	Vector3f max_velocity;				// Max linear velocity in all three axes.
	Vector3f max_rotational_velocity;	// Max rotational velocity in the pitch/bank/yaw axes.

	PhysicsData()
	{
		max_velocity = Vector3f(0,0,0);
		max_rotational_velocity = Vector3f(0,0,0);
	}
};

class PhysicsSim
{
public:
	static void ApplyFlightControls(PhysicsData *data, Transformation *t, FlightControls *flightControls);
};
