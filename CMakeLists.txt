cmake_minimum_required(VERSION 3.20)
project(MyLearnCPPThreadPool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

find_package(benchmark REQUIRED)

file(GLOB sourcefile *.cpp)
add_executable(main ${sourcefile})

target_link_libraries(main benchmark::benchmark)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

# for iouring test on Linux system
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detected Linux system: ${CMAKE_CXX_COMPILER_ID}")

    add_executable(io_uring_test iouring/io_uring_test.cpp)
    add_executable(echo_server iouring/echo_server.cpp)

    target_link_libraries(io_uring_test uring)
    target_link_libraries(echo_server uring)

    set_target_properties(io_uring_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/iouring"
    )
    set_target_properties(echo_server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/iouring"
    )
else()
    message(WARNING "Not on Linux. Skipping io_uring_test (io_uring is Linux-only).")
endif()