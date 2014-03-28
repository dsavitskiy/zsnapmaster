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

#ifndef ZSM_DATASET_H
#define ZSM_DATASET_H

#include "zsm.h"

namespace zsm {

/**
 * Dataset structure.
 */
struct Dataset {
    Dataset() : type(ZFS_TYPE_FILESYSTEM), timestamp(0),
        is_zsm(false), skip_snap(false),
        used(0), avail(0), refer(0), depth(0)
    {
    }

    explicit Dataset(zfs_handle_t *hzfs);

    std::string name;
    zfs_type_t type;
    std::string tag;
    int64_t timestamp;
    uint64_t used;
    uint64_t avail;
    uint64_t refer;
    bool is_zsm;
    bool skip_snap;
    size_t depth;
    std::map<std::string, std::string> props;
};

} // namespace zsm

#endif // ZSM_DATASET_H
