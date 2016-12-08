# - Try to find libbroker include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(Broker)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  BROKER_ROOT_DIR           Set this variable to the root installation of
#                            libbroker if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  BROKER_FOUND            System has libbroker, include and lib dirs found
#  BROKER_INCLUDE_DIR      The libbroker include directories.
#  BROKER_LIBRARY          The libbroker library.

find_path(BROKER_ROOT_DIR
    NAMES include/broker/broker.hh
    # As the broker path might not be added to the default lib search path, we
    # need to hint to the possible location. [0] states that the use of PATHS is
    # the way to go.
    # [0]: https://cmake.org/pipermail/cmake/2010-October/040460.html
    PATHS /usr/local/bro
    # /opt/bro/ would also be a valid possibility
)

find_path(BROKER_INCLUDE_DIR
    NAMES broker/broker.hh
    PATHS ${BROKER_ROOT_DIR}/include
)

find_library(BROKER_LIBRARY
    NAMES broker
    PATHS ${BROKER_ROOT_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BROKER DEFAULT_MSG
    BROKER_LIBRARY
    BROKER_INCLUDE_DIR
)

mark_as_advanced(
    BROKER_ROOT_DIR
    BROKER_INCLUDE_DIR
    BROKER_LIBRARY
)
