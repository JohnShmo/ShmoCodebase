cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 23)

project(ShmoCodebase C)

add_executable(ShmoCodebase src/main.c
        src/base.c
        src/arena.c
        src/allocator.c
        src/darray.c
        src/table.c
        src/linear_algebra.c
        src/strings.c
        src/user_input.c
        src/window.c
        src/app.c
)
find_package(SDL2 REQUIRED)
target_link_libraries(ShmoCodebase SDL2::SDL2)

target_include_directories(ShmoCodebase PUBLIC include)
target_compile_definitions(ShmoCodebase PUBLIC ENABLE_ASSERT=1)
target_compile_definitions(ShmoCodebase PUBLIC SDL_MAIN_HANDLED)