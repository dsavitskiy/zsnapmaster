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

#ifndef ZSM_LIST_H
#define ZSM_LIST_H

#include "zsm_command.h"
#include "zsm_dataset.h"

namespace zsm {

/**
 * Implements dataset listing.
 */
class List : public Command {
public:
    virtual Meta get_meta() override;
    virtual void exec(const Options &opts) override;

private:
    struct Column {
        Column(const std::string &key, const std::string &txt) : key(key),
            txt(txt), width(txt.size())
        {
        }

        std::string key;
        std::string txt;
        size_t width;
    };

private:
    void make_props(Dataset &d);
    void find(const std::string &root);
    void iter_dataset(zfs_handle_t *hzfs);
    void add_dataset(zfs_handle_t *hzfs);
    static int iter_pool(zpool_handle_t *hpool, void *self);
    static int iter_dataset(zfs_handle_t *hzfs, void *self);

private:
    std::string m_name;
    bool m_recursive;
    bool m_show_all;
    int m_filter_flags;
    bool m_verbose;
    std::string m_tag;

    std::list<Dataset> m_datasets;
    std::list<Column> m_cols;
};

} // namespace zsm

#endif // ZSM_LIST_H
