# - Try to find libacu include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(rocksdb)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  RDB_ROOT_DIR              Set this variable to the root installation of
#                            libacu if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  RDB_FOUND            System has libacu, include and lib dirs found
#  RDB_INCLUDE_DIR      The libacu include directories.
#  RDB_LIBRARY          The libacu library.

find_path(RDB_ROOT_DIR
    NAMES include/rocksdb/db.h
    # As the acu path might not be added to the default lib search path, we
    # need to hint to the possible location. [0] states that the use of PATHS is
    # the way to go.
    # [0]: https://cmake.org/pipermail/cmake/2010-October/040460.html
    # TODO: ADD YOUR CUSTOM PATH HERE!
    PATHS ../rocksdb
)

find_path(RDB_INCLUDE_DIR
    NAMES rocksdb/db.h
		PATHS ${RDB_ROOT_DIR}/include
)

find_library(RDB_LIBRARY
    NAMES rocksdb
    PATHS
				${RDB_ROOT_DIR}
				#${RDB_ROOT_DIR}/cmake-build-debug/src
				#${RDB_ROOT_DIR}/build/src
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RDB DEFAULT_MSG
		RDB_LIBRARY
		RDB_INCLUDE_DIR
)

mark_as_advanced(
		RDB_ROOT_DIR
		RDB_INCLUDE_DIR
		RDB_LIBRARY
)
