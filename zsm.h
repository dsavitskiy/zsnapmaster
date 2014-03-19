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

#ifndef ZSM_H
#define ZSM_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <assert.h>
#include <string.h>
#include <libzfs.h>

using namespace std::chrono;

namespace zsm {

#define ZSM_TIMESTAMP_PROP      "zsm:timestamp"
#define ZSM_TAG_PROP            "zsm:tag"

//
bool get_zfs_property(std::string &dst, zfs_handle_t *hzfs, const std::string &name);

//
template<class T>
T get_zfs_property(zfs_handle_t *hzfs, const std::string &name)
{
    T t = T();
    std::string s;
    if (!get_zfs_property(s, hzfs, name))
        return t;

    std::istringstream iss(s);
    iss >> t;
    return t;
}

//
std::string format_time(std::time_t t, const std::string &fmt, bool local);

//
std::string format_time(const system_clock::time_point &t, const std::string &fmt, bool local);

//
std::string format_time(const std::string &time_str, const std::string &fmt, bool local);

//
std::string pad_right(const std::string &s, size_t n, char fill = ' ');

//
bool parse_age(system_clock::duration &dst, const std::string &src);

//
std::string nice_bytes(uint64_t sz);

} // namespace zsm

#endif // ZSM_H
