# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        main.cc
        rocks_storage.cc
        config_parser.cc batch_aggregation.cc batch_aggregation.h batch_logging_correlation.h batch_logging_correlation.cc)

# Executable definition
add_executable(acu-impl ${SOURCE_FILES})
target_link_libraries(acu-impl ${LINK_LIBS})
