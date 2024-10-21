#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include "cpu_config.cpp"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

DWORD threads(int limit_percent);
char *threads_str();
char *cpu_affinity_str(DWORD cpus_to_use);
#endif // _WIN32 || _WIN64

#if defined(__linux__) || defined(__APPLE__)
int threads(int limit_percent);
char *threads_str();
char *cpu_affinity_str(int cpus_to_use);
#endif // __linux__ || __APPLE__

#endif // CPU_CONFIG_H