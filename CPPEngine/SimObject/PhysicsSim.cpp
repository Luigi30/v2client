#include "PhysicsSim.h"

void PhysicsSim::ApplyFlightControls(PhysicsData *physics, Transformation *t, FlightControls *flightControls)
{
	//const float rotationFactor = 0.5;

	t->rotation.y += flightControls->axis_3.x * physics->max_rotational_velocity.x; // X and Y are intentionally swapped here
	t->rotation.x += flightControls->axis_3.y * physics->max_rotational_velocity.y;

	// TODO: throttle instead of straight-up movement
	t->position = t->position - t->ForwardVec() * flightControls->axis_3.z;
}

// todo: everything else