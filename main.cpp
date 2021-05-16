#include <INTS.H>
#include <SSDEF.H>
#include <STARLET.H>

#include <stdio.h>

#include "Glide/GlideManager.h"
#include "Simulation/SimulationCore.h"

#include <fstream>

int main()
{
    g_GlideManager.StartGlide();
    g_SimulationCore.SetupSimulation();

    g_SimulationCore.SimOneFrame();

    return SS$_NORMAL;    
}