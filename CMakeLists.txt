cmake_minimum_required(VERSION 4.2)
project(Voidfall)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(Voidfall src/main.cpp
        src/game.cpp
        src/game.h
        src/Player.h
        src/Player.cpp
        src/Enemy.h
        src/Enemy.cpp
        src/config.h
        src/config.cpp
        src/Projectile.h
        src/Projectile.cpp)

target_link_libraries(Voidfall PRIVATE raylib)
#target_link_options(Voidfall PRIVATE -static-libgcc -static-libstdc++ -static)

