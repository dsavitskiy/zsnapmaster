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

#ifndef ZSM_DESTROY_H
#define ZSM_DESTROY_H

#include "zsm_command.h"

namespace zsm {

/**
 *
 */
class Destroy : public Command {
public:
    virtual Meta get_meta() override;
    virtual void exec(const Options &opts) override;

private:
    void find(const std::string &root);
    void destroy(const std::string &name);
    static int iter_dataset(zfs_handle_t *hzfs, void *self);
    bool check_tag(zfs_handle_t *hzfs) const;
    bool check_age(zfs_handle_t *hzfs) const;

private:
    std::string m_tag;
    int64_t m_age;
    bool m_recursive;
    bool m_defer;
    bool m_dry_run;
    size_t m_verbose;

    std::vector<std::string> m_datasets;
    int64_t m_now;
};

} // namespace zsm

#endif // ZSM_DESTROY_H
