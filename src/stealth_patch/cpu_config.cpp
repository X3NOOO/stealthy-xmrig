#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

DWORD threads(int limit_percent)
{
    if (!limit_percent)
    {
        if (sscanf_s(LIMIT_CPU_PERCENT, "%d", &limit_percent) != 1)
        {
            return 1; // Return a safe default value
        }
    }

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DWORD cpus = sysinfo.dwNumberOfProcessors;

    double limited_cpus = static_cast<double>(cpus) * (static_cast<double>(limit_percent) / 100.0);
    DWORD result = static_cast<DWORD>(limited_cpus);
    return (result > 0) ? result : 1;
}

char *threads_str()
{
    DWORD cpus = threads(0);
    char *cpus_str = new char[16];
    if (cpus_str)
    {
        sprintf_s(cpus_str, 16, "%lu", cpus);
    }
    return cpus_str;
}

char *cpu_affinity_str(DWORD cpus_to_use)
{
    DWORD_PTR affinity = 0;

    for (DWORD i = 0; i < cpus_to_use && i < sizeof(DWORD_PTR) * 8; i++)
    {
        affinity |= static_cast<DWORD_PTR>(1) << i;
    }

    char *affinity_str = new char[17];
    if (affinity_str)
    {
        sprintf_s(affinity_str, 17, "%016llX", affinity);
    }
    return affinity_str;
}
#endif // _WIN32 || _WIN64

#if defined(__linux__) || defined(__APPLE__)
#include <sys/sysinfo.h>

int threads(int limit_percent)
{
    if (!limit_percent)
    {
        if (sscanf(LIMIT_CPU_PERCENT, "%d", &limit_percent) != 1)
        {
            return 1; // Return a safe default value
        }
    }

    float result = float(get_nprocs_conf()) * (float(limit_percent) / 100.0f);
    return (int)result;
}

char *threads_str()
{
    int cpus = threads(0);
    char *cpus_str = new char[16];
    snprintf(cpus_str, 16, "%d", cpus);
    return cpus_str;
}

char *cpu_affinity_str(int cpus_to_use)
{
    unsigned int affinity = 0;

    for (int i = 0; i < cpus_to_use && i < 32; i++)
    {
        affinity |= 1u << i;
    }

    char *affinity_str = new char[9];
    snprintf(affinity_str, 9, "%08X", affinity);
    return affinity_str;
}
#endif // __linux__ || __APPLE__