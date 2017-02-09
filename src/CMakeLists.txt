# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        main.cc
        config_parser.cc
        lattice_alert_mapper.cc
        lattice_aggregation.cc
        lattice_incoming_alert.cc
        lattice_correlation.cc
        lattice_threshold.cc
        vector_storage.cc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

# Executable definition
add_executable(acu-impl ${SOURCE_FILES})
target_link_libraries(acu-impl ${LINK_LIBS})
