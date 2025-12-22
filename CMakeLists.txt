cmake_minimum_required(VERSION 4.0)
project(Project_7)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_7 main.cpp
        head.h
        choose_and_upgrade_hero.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Data/Weapons.txt
        ${CMAKE_CURRENT_BINARY_DIR}/Weapons.txt
        COPYONLY)