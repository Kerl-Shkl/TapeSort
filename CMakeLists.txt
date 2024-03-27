cmake_minimum_required(VERSION 3.22)

project(tape_sort VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build tests" ON)
IF(BUILD_TESTS)
    add_compile_definitions(TESTS_ENABLED)
ENDIF()
add_subdirectory(libs)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_executable(exec ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(exec PRIVATE include)
add_compile_options(-Wall -Wextra -Wno-missing-field-initializers -Wpedantic -Wconversion)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
