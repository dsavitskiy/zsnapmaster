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

#include "zsm_options.h"

namespace zsm {

Options::Options(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i) {
        m_raw_args.push_back(argv[i]);
    }
}

bool Options::parse(const Meta &meta)
{
    size_t n = m_raw_args.size();
    size_t i;
    for (i = 0; i < n; ++i) {
        const std::string &arg = m_raw_args[i];

        if (arg[0] != '-' || arg.size() == 1)
            break;

        if (arg == "--") {
            ++i;
            break;
        }

        const Option *opt = nullptr;

        if (arg[1] == '-') {
            // Long options.
            auto s = arg.substr(2);
            for (const Option &j : meta.opts) {
                if (j.long_name == s) {
                    ++m_values[j.name].count;
                    opt = &j;
                    break;
                }
            }
        } else {
            // Short (one char) options.
            for (char c : arg.substr(1)) {
                opt = nullptr;

                for (const Option &j : meta.opts) {
                    if (j.short_name == c) {
                        ++m_values[j.name].count;
                        opt = &j;
                        break;
                    }
                }

                if (!opt) {
                    std::cerr << "Unknown option: -" << c << std::endl;
                    return false;
                }
            }
        }

        if (!opt) {
            std::cerr << "Unknown option: " << arg << std::endl;
            return false;
        }

        if (opt->type == Option::OneArg) {
            ++i;
            if (i == n) {
                std::cerr << "Expected argument after " << arg << std::endl;
                return false;
            }
            m_values[opt->name].arg = m_raw_args[i];
        }
    }

    // Copy remaining options as operands.
    if (i < n) {
        for (; i < n; ++i) {
            m_operands.push_back(m_raw_args[i]);
        }
    }

    // Set unspecified options to false;
    for (auto i : meta.opts) {
        if (m_values.find(i.name) == m_values.end())
            m_values[i.name] = Value();
    }

    return true;
}

} // namespace zsm
