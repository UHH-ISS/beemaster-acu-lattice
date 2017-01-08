# CMakeLists.txt for tests

# Get source file list from src/
get_directory_property(_LIB_SOURCE DIRECTORY ../src DEFINITION SOURCE_FILES)
list(REMOVE_ITEM _LIB_SOURCE main.cc)
set(LIB_SOURCE)
foreach (_SRC ${_LIB_SOURCE})
    list(APPEND LIB_SOURCE ../src/${_SRC})
endforeach()

# Source files
set(SOURCE_FILES
        main.cc
        test_rocks_storage.cc)

# Add catch (testing library)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Define the executable
add_executable(tests ${LIB_SOURCE} ${SOURCE_FILES})
target_link_libraries(tests Catch ${LINK_LIBS})

# Add tests to CTest
enable_testing()
add_test(NAME acu-tests COMMAND tests)
