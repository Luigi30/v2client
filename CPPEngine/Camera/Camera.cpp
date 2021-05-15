#include "Camera.h"

Camera::Camera(std::string name) : SimulationObject(name, NULL)
{
}

Camera::~Camera(void)
{
}

void Camera::OnFrame()
{
	transformation.position = track_object->transformation.position + track_offset;
	transformation.rotation = track_object->transformation.rotation;
}