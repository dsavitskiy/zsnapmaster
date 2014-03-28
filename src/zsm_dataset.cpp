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

#include "zsm_dataset.h"

namespace zsm {

Dataset::Dataset(zfs_handle_t *hzfs) : Dataset()
{
    name = zfs_get_name(hzfs);
    type = zfs_get_type(hzfs);

    tag = get_zfs_property<std::string>(hzfs, ZSM_TAG_PROP);
    if (!tag.empty()) {
        is_zsm = true;
    }

    std::string skip_prop = get_zfs_property<std::string>(hzfs, ZSM_SKIP_PROP);
    if (skip_prop == "on" && type != ZFS_TYPE_SNAPSHOT) {
        skip_snap = true;
    }

    timestamp = get_zfs_property<uint64_t>(hzfs, ZSM_TIMESTAMP_PROP);

    used = get_zfs_property<uint64_t>(hzfs, "used");
    avail = get_zfs_property<uint64_t>(hzfs, "avail");
    refer = get_zfs_property<uint64_t>(hzfs, "refer");
}

} // namespace zsm
