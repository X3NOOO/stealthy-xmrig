#include "detection_state.h"
#include "windows/exit_processes.h"
#include "windows/pause_processes.h"
#include <iostream>

detection_state check()
{
    for (int i = 0; i < PAUSE_PROCESSES_COUNT; i++)
    {
        std::cout << "Checking for " << pause_processes[i] << std::endl;
    }

    return RUN;
}