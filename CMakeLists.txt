cmake_minimum_required(VERSION 3.0)
project(COMP345-A1-Part1)

include_directories(".")

file(GLOB map_files
    "MapDirectory/*.map"
)

foreach(mf ${map_files})
    file(COPY ${mf} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/MapDirectory/)
endforeach()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_executable(COMP345-A1-Part1 Map/Map.cpp MapLoader/MapLoader.cpp cards/Cards.cpp Orders/OrdersDriver.cpp Orders/Orders.cpp Player/Player.cpp GameEngine/GameEngine.cpp)
