// #include <windows.h>
#include <memory>
#include <iostream>
#include <atomic>
#include <thread>
#include <algorithm>
#include "../App.h"
#include "watchdog/detection_state.h"
#include "watchdog/check.h"

void watchdog(std::shared_ptr<xmrig::App> app, std::atomic<bool> &running)
{
    while (running.load())
    {
        if (!app->isReady())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
        }

        switch (check())
        {
        case RUN:
            app->resumeMining();
            break;
        case PAUSE:
            app->pauseMining();
            break;
        case EXIT:
            exit(0);
        case ERROR:
            exit(1);
        default:
            exit(2);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}