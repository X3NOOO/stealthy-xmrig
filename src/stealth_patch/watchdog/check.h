#ifndef WATCHDOG_CHECK_H
#define WATCHDOG_CHECK_H

#include "detection_state.h"

#if defined(_WIN32) || defined(_WIN64)
#include "windows.cpp"
#else
#include "others.cpp"
#endif

detection_state check();

#endif // WATCHDOG_CHECK_H