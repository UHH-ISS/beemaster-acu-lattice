# CMakeLists.txt for tests

# Source files
set(SOURCE_FILES
        main.cc)

# Add catch (testing library)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Define the executable
add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests Catch)

# Add tests to CTest
enable_testing()
add_test(NAME acu-tests COMMAND tests)
