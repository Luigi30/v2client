#pragma once

#include "SimulationObject.h"
#include <vector>

typedef std::vector<SimulationObject *>::iterator SimulationObjectIterator;

class SimulationObjectList
{
	std::vector<SimulationObject *> simobjects;

public:
	SimulationObjectIterator GetBeginning() { return simobjects.begin(); }
	SimulationObjectIterator GetEnding() { return simobjects.end(); }

	void Add(SimulationObject *);
	SimulationObject *GetSimObject(std::string name);

	SimulationObjectList(void);
	~SimulationObjectList(void);
};

extern SimulationObjectList simObjectList;