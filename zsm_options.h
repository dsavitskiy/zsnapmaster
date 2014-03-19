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

#ifndef ZSM_OPTIONS_H
#define ZSM_OPTIONS_H

#include "zsm.h"

namespace zsm {

///
struct Option {
    enum Type {
        Flag,
        OneArg
    };

    const char *name;
    char short_name;
    const char *long_name;
    Type type;
    const char *help;
};


///
struct Meta {
    const char* help;
    std::list<Option> opts;
};


///
class Options {
public:
    Options(int argc, char *argv[]);

    bool parse(const Meta &meta);

    bool get(const std::string &name) const {
        return find(name).count > 0;
    }

    const std::string &get_arg(const std::string &name) const {
        return find(name).arg;
    }

    size_t get_count(const std::string &name) const {
        return find(name).count;
    }

    const std::vector<std::string> &ops() const {
        return m_operands;
    }

    std::string op(size_t n = 0) const {
        if (n >= m_operands.size())
            return std::string();
        return m_operands[n];
    }

private:
    struct Value {
        Value() : count(0) {}

        size_t count;
        std::string arg;
    };

    const Value &find(const std::string &name) const
    {
        auto i = m_values.find(name);
        assert(i != m_values.end());
        return i->second;
    }

private:
    std::vector<std::string> m_raw_args;
    std::map<std::string, Value> m_values;
    std::vector<std::string> m_operands;
};

} // namespace zsm

#endif // ZSM_OPTIONS_H
