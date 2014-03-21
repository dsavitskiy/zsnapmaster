/******************************************************************************
 * Copyright (c) 2014 kona4kona (kona4kona@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#include "zsm_create.h"
#include "zsm_destroy.h"
#include "zsm_list.h"

using namespace zsm;

struct CommandInfo {
    const char *name;
    Command* (*func)();
};

static CommandInfo commands[] = {
    { "create",    []() -> Command* { return new Create; } },
    { "destroy",   []() -> Command* { return new Destroy; } },
    { "list",      []() -> Command* { return new List; } }
};

static void print_command_help(const std::string &name, std::shared_ptr<Command> cmd)
{
    Meta meta = cmd->get_meta();

    std::cout << "  " << pad_right(name, 24) << meta.help << "\n";

    for (auto j : meta.opts) {
        std::ostringstream oss;
        if (j.short_name)
            oss << "-" << j.short_name << ", ";
        oss << "--" << j.long_name;
        if (j.type == Option::OneArg)
            oss <<  " <arg>";

        std::cout << "    " << pad_right(oss.str(), 22) << j.help << "\n";
    }
}


static void print_help()
{
    std::cout << "\n"
        "zsnapmaster by kona4kona\n"
        "\n"
        "Usage:\n";

    for (size_t i = 0; i < sizeof (commands) / sizeof (commands[0]); ++i) {
        print_command_help(commands[i].name, std::shared_ptr<Command>(commands[i].func()));
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::string cmd = argv[1];

    if (cmd == "-h" || cmd == "--help") {
        print_help();
        return 0;
    }

    std::shared_ptr<Command> pcmd;
    for (size_t i = 0; i < sizeof (commands) / sizeof (commands[0]); ++i) {
        if (cmd == commands[i].name) {
            pcmd.reset(commands[i].func());
            break;
        }
    }

    if (!pcmd) {
        std::cerr << "Unknown command: " << cmd << std::endl;
        return 1;
    }

    Options opts(argc - 2, argv + 2);
    if (!opts.parse(pcmd->get_meta()))
        return 1;

    libzfs_handle_t *hlib = libzfs_init();
    if (!hlib) {
        std::cerr << "Cannot initalize libzfs" << std::endl;
        return 1;
    }

    pcmd->set_lib(hlib);
    int ret = pcmd->exec(opts);
    libzfs_fini(hlib);

    return ret;
}
