cmake_minimum_required(VERSION 3.0)
project(COMP345-A1-Part1)

include_directories(".")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_executable(COMP345-A1-Part1 Map/Map.cpp MapLoader/MapLoader.cpp cards/Cards.cpp Orders/OrdersDriver.cpp Orders/Orders.cpp Player/Player.cpp GameEngine/GameEngine.cpp)
