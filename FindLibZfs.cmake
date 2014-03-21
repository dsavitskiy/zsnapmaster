
include(FindPackageHandleStandardArgs)

find_path (SPL_INCLUDE_DIR zone.h HINTS /usr/include /usr/include/libspl)
find_path (ZFS_INCLUDE_DIR libzfs.h HINTS /usr/include /usr/include/libzfs)

find_package_handle_standard_args ("SPL include dir" DEFAULT_MSG SPL_INCLUDE_DIR)
find_package_handle_standard_args ("libzfs include dir" DEFAULT_MSG ZFS_INCLUDE_DIR)

set (LIBZFS_INCLUDE_DIRS
    ${ZFS_INCLUDE_DIR}
    ${SPL_INCLUDE_DIR})

find_library (LIBNVPAIR_LIBRARY libnvpair.so)
find_library (LIBZPOOL_LIBRARY libzpool.so)
find_library (LIBZFS_LIBRARY libzfs.so)

find_package_handle_standard_args ("libnvpair" DEFAULT_MSG LIBNVPAIR_LIBRARY)
find_package_handle_standard_args ("libzpool" DEFAULT_MSG LIBZPOOL_LIBRARY)
find_package_handle_standard_args ("libzfs" DEFAULT_MSG LIBZFS_LIBRARY)

set (LIBZFS_LIBRARIES 
    ${LIBZFS_LIBRARY}
    ${LIBZPOOL_LIBRARY}
    ${LIBNVPAIR_LIBRARY})
