#pragma once
#include "Math/Vector3f.h"
#include "SimObject/SimulationObject.h"

class Camera : public SimulationObject
{
private:


public:
	SimulationObject *track_object; // The SimObject that the camera is tracking.
	Vector3f track_offset; // Where, in relation to track_object, is the camera?

	Camera(std::string);
	~Camera(void);

	void OnFrame();
};
