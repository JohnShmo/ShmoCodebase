cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 17)

project(ShmoCodebase C)
project(ShmoCodebaseSandbox C)

add_executable(ShmoCodebaseSandbox src/sandbox/main.c)

add_library(ShmoCodebase STATIC
        src/base.c
        src/allocator.c
        src/darray.c
        src/table.c
        src/linear_algebra.c
        src/strings.c
        src/user_input.c
        src/set.c
        src/arena.c
        src/stack.c
        src/queue.c
)

target_include_directories(ShmoCodebase PUBLIC include)
target_link_libraries(ShmoCodebaseSandbox PUBLIC ShmoCodebase)
