cmake_minimum_required (VERSION 2.8)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")
set (CMAKE_C_COMPILER "clang")
set (CMAKE_CXX_COMPILER "clang++")

add_definitions ("-std=c++11")                      #"-stdlib=libc++"
add_definitions ("-DHAVE_IOCTL_IN_SYS_IOCTL_H")

project ("zsnapmaster")

file(GLOB ZSM_SRC "src/*.cpp")
add_executable (zsnapmaster ${ZSM_SRC})

find_package (LibZfs REQUIRED)

include_directories (${LIBZFS_INCLUDE_DIRS})
target_link_libraries (zsnapmaster ${LIBZFS_LIBRARIES})
