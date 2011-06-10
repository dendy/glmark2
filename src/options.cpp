/*
 * Copyright © 2011 Linaro Limited
 *
 * This file is part of glcompbench.
 *
 * glcompbench is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glcompbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glcompbench.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Alexandros Frantzis <alexandros.frantzis@linaro.org>
 *  Jesse Barker <jesse.barker@linaro.org>
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

#include "options.h"

std::vector<std::string> Options::benchmarks;
bool Options::swap_buffers = true;
bool Options::list_scenes = false;
bool Options::show_debug = false;
bool Options::show_help = false;

static struct option long_options[] = {
    {"benchmark", 1, 0, 0},
    {"no-swap-buffers", 0, 0, 0},
    {"list-scenes", 0, 0, 0},
    {"debug", 0, 0, 0},
    {"help", 0, 0, 0},
    {0, 0, 0, 0}
};

void
Options::print_help()
{
    printf("A benchmark for Open GL (ES) 2.0\n"
           "\n"
           "Options:\n"
           "  -b, --benchmark BENCH  A benchmark to run: 'scene(:opt1=val1)*'\n"
           "                         (the option can be used multiple times)\n"
           "      --no-swap-buffers  Don't update the screen by swapping the front and\n"
           "                         back buffer, use glFinish() instead\n"
           "  -l, --list-scenes      Display information about the available scenes\n"
           "                         and their options\n"
           "  -d, --debug            Display debug messages\n"
           "  -h, --help             Display help\n");
}

bool
Options::parse_args(int argc, char **argv)
{
    while (1) {
        int option_index = -1;
        int c;
        const char *optname = "";

        c = getopt_long(argc, argv, "b:ldh",
                        long_options, &option_index);
        if (c == -1)
            break;
        if (c == ':' || c == '?')
            return false;

        if (option_index != -1)
            optname = long_options[option_index].name;

        if (c == 'b' || !strcmp(optname, "benchmark"))
            Options::benchmarks.push_back(optarg);
        else if (!strcmp(optname, "no-swap-buffers"))
            Options::swap_buffers = false;
        else if (c == 'l' || !strcmp(optname, "list-scenes"))
            Options::list_scenes = true;
        else if (c == 'd' || !strcmp(optname, "debug"))
            Options::show_debug = true;
        else if (c == 'h' || !strcmp(optname, "help"))
            Options::show_help = true;
    }

    return true;
}