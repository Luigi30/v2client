#include <INTS.H>
#include <SSDEF.H>
#include <STARLET.H>
#include <UNISTD.H>
#include <stdio.h>

#include "Glide/GlideManager.h"
#include "Simulation/SimulationCore.h"

int main()
{
    g_GlideManager.StartGlide();
    g_SimulationCore.SetupSimulation();

    g_SimulationCore.SimOneFrame();

    usleep(5000000);

    for(int i=0; i<600; i++)
    {
        g_SimulationCore.SimOneFrame();
    }

    return SS$_NORMAL;
} 