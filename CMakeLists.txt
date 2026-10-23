cmake_minimum_required(VERSION 3.20)
project(ModernCExercises C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enforce strict ISO C Standard
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Werror
    )
elseif(MSVC)
    add_compile_options(
        /W4
        /wx
    )
endif()

add_subdirectory(programs)