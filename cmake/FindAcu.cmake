# - Try to find libacu include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(Acu)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  ACU_ROOT_DIR              Set this variable to the root installation of
#                            libacu if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  ACU_FOUND            System has libacu, include and lib dirs found
#  ACU_INCLUDE_DIR      The libacu include directories.
#  ACU_LIBRARY          The libacu library.

find_path(ACU_ROOT_DIR
    NAMES include/acu/acu.h
    # As the acu path might not be added to the default lib search path, we
    # need to hint to the possible location. [0] states that the use of PATHS is
    # the way to go.
    # [0]: https://cmake.org/pipermail/cmake/2010-October/040460.html
    # TODO: ADD YOUR CUSTOM PATH HERE!
    PATHS
        ../acu-fw
        ../mp-ids-acu-fw
)

find_path(ACU_INCLUDE_DIR
    NAMES acu/acu.h
    PATHS ${ACU_ROOT_DIR}/include
)

find_library(ACU_LIBRARY
    NAMES acu
    PATHS
        ${ACU_ROOT_DIR}/lib
        ${ACU_ROOT_DIR}/cmake-build-debug/src
        ${ACU_ROOT_DIR}/build/src
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ACU DEFAULT_MSG
    ACU_LIBRARY
    ACU_INCLUDE_DIR
)

mark_as_advanced(
    ACU_ROOT_DIR
    ACU_INCLUDE_DIR
    ACU_LIBRARY
)
