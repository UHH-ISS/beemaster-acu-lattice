# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        main.cc
        rocks_storage.cc
        vector_storage.cc
        config_parser.cc
		latticeCorrelation.cc
				lattice_threshold.cc
				lattice_incoming_alert.cc
				lattice_outgoing_alert.cc)

# Executable definition
add_executable(acu-impl ${SOURCE_FILES})
target_link_libraries(acu-impl ${LINK_LIBS})
