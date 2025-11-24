cmake_minimum_required(VERSION 3.16)
project(TanksGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)

add_executable(TanksGame
        src/main.cpp
        src/GameScane.cpp
        src/GameModel.cpp
        include/GameScane.h
        include/GameModel.h
        src/Bonus.cpp
        src/Tank.cpp
        src/EnemyTank.cpp
        src/Wall.cpp
        src/BrickWall.cpp
        src/Bullet.cpp
)

target_link_libraries(TanksGame PRIVATE
        Qt5::Core
        Qt5::Widgets
)

target_link_libraries(TanksGame PRIVATE Qt5::Multimedia)
target_include_directories(TanksGame PUBLIC ${CMAKE_SOURCE_DIR}/include)