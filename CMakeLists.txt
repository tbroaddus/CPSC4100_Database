cmake_minimum_required(VERSION 3.1)

project(CPSC4100_Database)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories("${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/lib/json11"
  "${PROJECT_SOURCE_DIR}/lib/fast-cpp-csv-parser")

add_executable(runDB
  "${PROJECT_SOURCE_DIR}/src/main.cpp"
  "${PROJECT_SOURCE_DIR}/lib/json11/json11.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(runDB Threads::Threads)
