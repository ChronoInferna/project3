cmake_minimum_required(VERSION 3.22)
project(project3)

set(CMAKE_CXX_STANDARD 17)

# Compile flags to match Gradescope test environment
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
# Gradescope does use -Werror, so if you remove it here you may run into issues when trying to submit
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(src)

add_library(compass_lib
        src/Compass.cpp
        src/Compass.hpp
        src/CSVReader.cpp
        src/CSVReader.hpp
        src/Graph.cpp
        src/Graph.hpp
        src/StudentDirectory.cpp
        src/StudentDirectory.hpp
        src/ClassDirectory.cpp
        src/ClassDirectory.hpp
)

target_include_directories(compass_lib PUBLIC include)

add_executable(Main src/main.cpp)
target_link_libraries(Main PRIVATE compass_lib)

# These tests can use the Catch2-provided main
add_executable(Tests test/test.cpp)
target_link_libraries(Tests PRIVATE compass_lib)
        
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

# find_package(Catch2 3.10 REQUIRED)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)
catch_discover_tests(Tests
    ADD_TAGS_AS_LABELS
)

# Copy `data/` into the build directory so runtime can open files with `data/...`
add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/data
                        ${CMAKE_BINARY_DIR}/data
        COMMENT "Copying data directory to build directory"
)

# Ensure both executables are built after the data is copied
add_dependencies(Main copy_data)
add_dependencies(Tests copy_data)
