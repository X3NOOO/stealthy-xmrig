#include "detection_state.h"
#include "windows/exit_processes.h"
#include "windows/pause_processes.h"
#include <windows.h>
#include <algorithm>
#include <cctype>

detection_state check()
{
    detection_state state = RUN;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == NULL)
        return ERROR;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    Process32First(hSnapshot, &pe);
    do
    {
        char *process_name = new char[strlen(pe.szExeFile) + 1];
        std::transform(pe.szExeFile, pe.szExeFile + strlen(pe.szExeFile), process_name, ::tolower);

        for (const char *exit_process : exit_processes)
        {
            if (strcmp(process_name, exit_process) == 0)
            {
                state = EXIT;
                goto exit;
            }
        }

        for (const char *pause_process : pause_processes)
        {
            if (strcmp(process_name, pause_process) == 0)
            {
                state = PAUSE;
                goto exit;
            }
        }
    } while (Process32Next(hSnapshot, &pe));

exit:
    CloseHandle(hSnapshot);
    return state;
}