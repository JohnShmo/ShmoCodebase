cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 23)

project(ShmoCodebase C)

add_executable(ShmoCodebase src/main.c
        src/base.c
        src/free_list_arena.c
        src/allocator.c
        src/darray.c
        src/table.c
        src/linear_algebra.c
        src/strings.c
        src/user_input.c
        src/set.c
        src/linear_arena.c
)

target_include_directories(ShmoCodebase PUBLIC include)
