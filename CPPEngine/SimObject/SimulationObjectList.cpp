#include "SimulationObjectList.h"

void SimulationObjectList::Add(SimulationObject *obj)
{
	assert(!GetSimObject(obj->name)); // names must be unique
	simobjects.push_back(obj);
}

SimulationObject *SimulationObjectList::GetSimObject(std::string name)
{
	SimulationObjectIterator it = this->GetBeginning();

	for (it; it != this->GetEnding(); ++it) {
		if((*it)->name == name) return *it;
	}

	return NULL;
}

SimulationObjectList::SimulationObjectList(void)
{
}

SimulationObjectList::~SimulationObjectList(void)
{
}
