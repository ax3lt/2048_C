cmake_minimum_required(VERSION 3.24)
project(2048_mac C)

set(CMAKE_C_STANDARD 99)

add_executable(2048_mac main.c src/Global_Declarations.h src/Database/DB_logic.c src/Database/DB_logic.h src/Game/Game_logic.h src/Game/Game_logic.c)
