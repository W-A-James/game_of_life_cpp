cmake_minimum_required(VERSION 3.10)

project(Life
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wpedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wpedantic -g -O2")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_THREAD_PREFER_PTHREAD True)
set(THREADS_PREFER_PTHREAD_FLAG True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Parallel neighbour refs
add_library(neighbour_refs_par neighbour_refs_par.cpp)
target_link_libraries(neighbour_refs_par pthread)

# TODO: add opt flags
add_library(neighbour_refs_par_opt neighbour_refs_par.cpp)
target_compile_options(neighbour_refs_par_opt PRIVATE "-O2")
target_link_libraries(neighbour_refs_par_opt pthread)

# Neighbour refs
add_library(neighbour_refs neighbour_refs.cpp)

# TODO: add opt flags
add_library(neighbour_refs_opt neighbour_refs.cpp)
target_compile_options(neighbour_refs_opt PRIVATE "-O2")

# Naive impl
add_library(naive naive.cpp)
# TODO: add opt flags
add_library(naive_opt naive.cpp)
target_compile_options(naive_opt PRIVATE "-O2")

# Main
add_executable(main main.cpp)
target_compile_options(main PRIVATE "-O2")
target_link_libraries(main
  neighbour_refs_par
  ncurses
  )

# Create list of implementation files
list(APPEND Libs naive neighbour_refs neighbour_refs_par)

# Test executables
foreach(lib IN LISTS Libs)
  add_executable("test_${lib}" test_main.cpp)
  target_link_libraries("test_${lib}" "${lib}")

  add_executable("test_${lib}_opt" test_main.cpp)
  target_compile_options("test_${lib}_opt" PRIVATE "-O2")
  target_link_libraries("test_${lib}_opt" "${lib}_opt")
endforeach()
