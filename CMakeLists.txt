project(StringTemplate)
cmake_minimum_required(VERSION 2.8)

add_definitions(-Wall)

include_directories(library)

file(GLOB_RECURSE library_headers library/*.hpp)
file(GLOB_RECURSE library_sources library/*.cpp)
add_library(stringtemplate ${library_sources} ${library_headers})

file(GLOB_RECURSE example_headers example/*.hpp)
file(GLOB_RECURSE example_sources example/*.cpp)
add_executable(example ${example_sources} ${example_headers})
target_link_libraries(example stringtemplate)

