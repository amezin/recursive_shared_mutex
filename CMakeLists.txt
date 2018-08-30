cmake_minimum_required(VERSION 3.1)

project(shared_recursive_mutex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME} shared_recursive_mutex.cpp shared_recursive_mutex.h)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

enable_testing()

function(lib_test name)
add_executable(${name} ${name}.cpp)
target_link_libraries(${name} ${PROJECT_NAME})
add_test(NAME ${name} COMMAND ${name})
endfunction()

lib_test(test_lock_recursive)
lib_test(test_shared_lock)
lib_test(test_exclusive_lock)
lib_test(test_shared_and_exclusive)
