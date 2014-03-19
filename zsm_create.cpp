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

namespace zsm {

Meta Create::get_meta()
{
    return {
        "create snapshot(s) of the given dataset(s)",
        {
            { "tag",       't', "tag",          Option::OneArg, "snapshot tag" },
            { "recursive", 'r', "recursive",    Option::Flag,   "create snapshots recursively" },
            { "dry_run",   'n', "dry-run",      Option::Flag,   "skip snapshot creation" },
            { "verbose",   'V', "verbose",      Option::Flag,   "verbose output" },
        }
    };
}


int Create::exec(const Options &opts)
{
    m_tag = opts.get_arg("tag");
    if (m_tag.empty()) {
        std::cerr << "Schedule tag must be specified" << std::endl;
        return 1;
    }

    m_recursive = opts.get("recursive");
    m_dry_run = opts.get("dry_run");
    m_verbose = opts.get("verbose");

    m_datasets.clear();
    for (auto i : opts.ops()) {
        find(i);
    }

    if (m_datasets.empty()) {
        std::cerr << "At least one dataset must be specified" << std::endl;
        return 1;
    }

    //
    system_clock::time_point now = system_clock::now();
    system_clock::duration epoch = now.time_since_epoch();
    m_timestamp = std::to_string(duration_cast<seconds>(epoch).count());
    m_snap_suffix = format_time(now, "%Y.%m.%d_%H.%M.%S", true);

    if (m_verbose) {
        std::cout << format_time(now, "%Y.%m.%d %H:%M:%S", false)
            << " creating snapshots\n"
            << "    tag          : " << m_tag << "\n"
            << "    timestamp    : " << m_timestamp << "\n"
            << "    suffix       : " << m_snap_suffix << "\n";
    }

    for (auto i : m_datasets) {
        snap(i);
    }

    if (m_verbose) {
        std::cout << std::endl;
    }

    return 0;
}


void Create::find(const std::string &root)
{
    zfs_handle_t *hzfs = zfs_open(m_hlib, root.c_str(),
        ZFS_TYPE_FILESYSTEM | ZFS_TYPE_VOLUME);

    if (!hzfs) {
        std::cerr << root << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    m_datasets.push_back(root);

    if (m_recursive) {
        zfs_iter_filesystems(hzfs, iter_dataset, this);
    }

    zfs_close(hzfs);
}


void Create::snap(const std::string &name)
{
    std::string snap_name = name + "@" + m_snap_suffix + "_" + m_tag;

    if (m_verbose)
        std::cout << "    "
            << (m_dry_run ? "would create : " : "creating     : ")
            << snap_name << "\n";

    if (m_dry_run)
        return;

    if (zfs_snapshot(m_hlib, snap_name.c_str(), B_FALSE, NULL) != 0) {
        std::cerr << snap_name << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    zfs_handle_t *hsnap = zfs_open(m_hlib, snap_name.c_str(), ZFS_TYPE_SNAPSHOT);
    if (!hsnap) {
        std::cerr << snap_name << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    if (zfs_prop_set(hsnap, ZSM_TIMESTAMP_PROP, m_timestamp.c_str()) != 0 ||
        zfs_prop_set(hsnap, ZSM_TAG_PROP, m_tag.c_str()) != 0) {
        std::cerr << snap_name << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    zfs_close(hsnap);
}


int Create::iter_dataset(zfs_handle_t *hzfs, void *ptr)
{
    Create *self = (Create*)ptr;

    self->m_datasets.push_back(zfs_get_name(hzfs));

    if (self->m_recursive) {
        zfs_iter_filesystems(hzfs, iter_dataset, self);
    }

    return 0;
}

} // namespace zsm
