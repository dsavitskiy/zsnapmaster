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

#ifndef ZSM_COMMAND_H
#define ZSM_COMMAND_H

#include "zsm_options.h"

namespace zsm {

/**
 *
 */
class Command {
public:
    Command() : m_hlib(nullptr)
    {
    }

    virtual ~Command()
    {
    }

    virtual Meta get_meta() = 0;

    virtual void exec(const Options &opts) = 0;

    void set_lib(libzfs_handle_t *hlib)
    {
        m_hlib = hlib;
    }

protected:
    libzfs_handle_t *m_hlib;
};

} // namespace zsm

#endif // ZSM_COMMAND_H
