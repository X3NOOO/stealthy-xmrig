#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "watchdog.cpp"

#include <memory>
#include "../App.h"

void watchdog(std::shared_ptr<xmrig::App> app, std::atomic<bool> &running);

#endif // WATCHDOG_H