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

#include "zsm_destroy.h"

namespace zsm {

Meta Destroy::get_meta()
{
    return {
        "destroy snapshot(s) previously created by zsnapmaster",
        {
            { "tag",       't', "tag",          Option::OneArg, "snapshot tag" },
            { "age",       'a', "age",          Option::OneArg, "destroy snapshots older than the given age" },
            { "recursive", 'r', "recursive",    Option::Flag,   "recursive" },
            { "defer",     'd', "defer",        Option::Flag,   "defer snapshot deletion" },
            { "dry_run",   'n', "dry-run",      Option::Flag,   "dry run" },
            { "verbose",   'V', "verbose",      Option::Flag,   "verbose output" },
        }
    };
}


int Destroy::exec(const Options &opts)
{
    m_tag = opts.get_arg("tag");
    if (m_tag.empty()) {
        std::cerr << "Tag must be specified" << std::endl;
        return 1;
    }

    std::string age_str = opts.get_arg("age");
    if (age_str.empty()) {
        std::cerr << "Age must be specified" << std::endl;
        return 1;
    }

    system_clock::duration age_dur;
    if (!parse_age(age_dur, age_str)) {
        std::cerr << "Invalid age format: " << age_str << std::endl;
        return 1;
    }
    m_age = duration_cast<seconds>(age_dur).count();

    m_recursive = opts.get("recursive");
    m_defer = opts.get("defer");
    m_dry_run = opts.get("dry_run");
    m_verbose = opts.get_count("verbose");

    system_clock::time_point now = system_clock::now();
    m_now = duration_cast<seconds>(now.time_since_epoch()).count();

    if (m_verbose) {
        std::cout << format_time(now, "%Y.%m.%d %H:%M:%S", false)
            << " cleaning up\n"
            << "    tag           : " << m_tag << "\n"
            << "    age           : " << age_str << "\n";
    }

    m_datasets.clear();
    for (auto i : opts.ops()) {
        find(i);
    }

    for (auto i : m_datasets) {
        destroy(i);
    }

    if (m_verbose) {
        std::cout << std::endl;
    }

    return 0;
}


void Destroy::find(const std::string &root)
{
    zfs_handle_t *hzfs = zfs_open(m_hlib, root.c_str(),
        ZFS_TYPE_FILESYSTEM | ZFS_TYPE_VOLUME);

    if (!hzfs) {
        std::cerr << root << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    zfs_iter_snapshots(hzfs, B_FALSE, iter_dataset, this);

    if (m_recursive) {
        zfs_iter_filesystems(hzfs, iter_dataset, this);
    }

    zfs_close(hzfs);
}


void Destroy::destroy(const std::string &name)
{
    if (m_verbose)
        std::cout << "    "
            << (m_dry_run ? "would destroy : " : "destroying    : ") << name << "\n";

    if (m_dry_run)
        return;

    zfs_handle_t *hzfs = zfs_open(m_hlib, name.c_str(), ZFS_TYPE_SNAPSHOT);

    if (!hzfs) {
        std::cerr << name << ": " << libzfs_error_description(m_hlib) << std::endl;
        return;
    }

    zfs_destroy(hzfs, m_defer ? B_TRUE : B_FALSE);

    zfs_close(hzfs);
}


int Destroy::iter_dataset(zfs_handle_t *hzfs, void *ptr)
{
    Destroy *self = (Destroy*)ptr;

    if (zfs_get_type(hzfs) == ZFS_TYPE_SNAPSHOT) {
        if (self->check_tag(hzfs) && self->check_age(hzfs)) {
            self->m_datasets.push_back(zfs_get_name(hzfs));
        }
    }

    if (self->m_recursive) {
        zfs_iter_filesystems(hzfs, iter_dataset, self);
        zfs_iter_snapshots(hzfs, B_FALSE, iter_dataset, self);
    }

    return 0;
}


bool Destroy::check_tag(zfs_handle_t *hzfs) const
{
    auto tag_prop = get_zfs_property<std::string>(hzfs, ZSM_TAG_PROP);
    if (tag_prop == m_tag)
        return true;

    if (m_verbose >= 2) {
        std::cout << "    skipping      : " << zfs_get_name(hzfs) << ", ";
        if (tag_prop.empty())
            std::cout << "empty tag\n";
        else
            std::cout << "different tag: " << tag_prop << "\n";
    }

    return false;
}


bool Destroy::check_age(zfs_handle_t *hzfs) const
{
    int64_t timestamp = get_zfs_property<int64_t>(hzfs, ZSM_TIMESTAMP_PROP);
    int64_t delta = m_now - timestamp;

    if (delta > m_age)
        return true;

    if (m_verbose >= 2) {
        std::cout << "    skipping      : " << zfs_get_name(hzfs) << ", "
                  << "not old enough\n";
    }

    return false;
}

} // namespace zsm
