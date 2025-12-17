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
        src/GameScene.cpp
        src/GameModel.cpp
        src/InputController.cpp
        include/GameScene.h
        include/GameModel.h
        include/InputController.h
        src/Bonus.cpp
        src/Tank.cpp
        src/TankView.cpp
        src/EnemyTank.cpp
        src/KamikazeEnemy.cpp
        src/LightEnemy.cpp
        src/HeavyEnemy.cpp
        src/TwinShooterEnemy.cpp
        src/Wall.cpp
        src/BrickWall.cpp
        src/Bullet.cpp
        include/TankView.h
)

target_link_libraries(TanksGame PRIVATE
        Qt5::Core
        Qt5::Widgets
)

target_link_libraries(TanksGame PRIVATE Qt5::Multimedia)
target_include_directories(TanksGame PUBLIC ${CMAKE_SOURCE_DIR}/include)