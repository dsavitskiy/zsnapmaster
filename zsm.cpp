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

#include "zsm.h"

namespace zsm {

bool get_zfs_property(std::string &dst, zfs_handle_t *hzfs, const std::string &name)
{
    dst.clear();

    zfs_prop_t zprop = zfs_name_to_prop(name.c_str());
    if ((int)zprop == ZPROP_INVAL) {
        char *str;
        nvlist_t *prop_val;
        nvlist_t *user_props = zfs_get_user_props(hzfs);

        if (!user_props)
            return false;

        if (nvlist_lookup_nvlist(user_props, name.c_str(), &prop_val) == 0) {
            if (nvlist_lookup_string(prop_val, ZPROP_VALUE, &str) == 0) {
                dst = str;
                return true;
            }
        }

        return false;
    } else {
        char buff[ZFS_MAXPROPLEN];
        zprop_source_t source_type;
        char source[ZFS_MAXNAMELEN];

        memset(buff, 0, sizeof (buff));

        zfs_prop_get(hzfs, zprop, buff, sizeof (buff),
            &source_type, source, sizeof (source), B_TRUE);

        dst = buff;
        return true;
    }
}


std::string format_time(std::time_t t, const std::string &fmt, bool local)
{
    std::tm *tm = local ? std::localtime(&t) : std::gmtime(&t);
    char buff[128];
    std::strftime(buff, 128, fmt.c_str(), tm);
    return buff;
}


std::string format_time(const system_clock::time_point &t, const std::string &fmt, bool local)
{
    std::time_t tt = system_clock::to_time_t(t);
    return format_time(tt, fmt, local);
}


std::string format_time(const std::string &time_str, const std::string &fmt, bool local)
{
    std::time_t t;
    std::istringstream iss(time_str);
    iss >> t;
    return format_time(t, fmt, local);
}


std::string pad_right(const std::string &s, size_t n, char fill)
{
    size_t sz = s.length();
    if (sz < n)
        return s + std::string(n - sz, fill);
    else
        return s;
}


bool parse_age(system_clock::duration &dst, const std::string &src)
{
    // Parse format: 1Y2M3D4h5m6s

    system_clock::duration age(0);

    std::string num;
    for (auto i : src) {
        if (i >= '0' && i <= '9') {
            num += i;
        } else {
            if (num.empty())
                return false;
            int val = std::stoi(num);
            if (val == 0)
                return false;
            num.clear();
            switch (i) {
            case 'd': age += 24 * hours(val);    break;
            case 'h': age += hours(val);   break;
            case 'm': age += minutes(val); break;
            case 's': age += seconds(val); break;
            default:
                return false;
            }
        }
    }

    if (!num.empty())
        return false;

    dst = age;

    return true;
}


std::string nice_bytes(uint64_t sz)
{
    static struct {
        double size;
        const char *suffix;
    } units[] = {
        { 1024l * 1024l * 1024l * 1024l, "T" },
        { 1024l * 1024l * 1024l, "G" },
        { 1024l * 1024l, "M" },
        { 1024l, "K" }
    };

    const char *suffix = "";
    double x = (double)sz;
    for (size_t i = 0; i < sizeof (units) / sizeof (units[0]); ++i) {
        if (x >= units[i].size) {
            x /= units[i].size;
            suffix = units[i].suffix;
            break;
        }
    }

    char buff[128];
    snprintf(buff, 128, "%.2f%s", x, suffix);
    return buff;
}

} // namespace zsm
