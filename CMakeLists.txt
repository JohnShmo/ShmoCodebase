cmake_minimum_required(VERSION 3.26)
project(ShmoCodebase C)

set(CMAKE_C_STANDARD 23)

add_executable(ShmoCodebase src/main.c
        src/base.c
        src/arena.c
        src/allocator.c
        src/darray.c
        src/table.c
        src/linear_algebra.c
        src/memory.c
)
target_include_directories(ShmoCodebase PUBLIC include)
target_compile_definitions(ShmoCodebase PUBLIC ENABLE_ASSERT=1)
