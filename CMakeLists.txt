cmake_minimum_required(VERSION 3.0)
project(COMP345-A1-Part1)

include_directories(".")

file(GLOB dom_map_files
    "DominationMaps/*.map"
)

foreach(mf ${dom_map_files})
    file(COPY ${mf} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/DominationMaps/)
endforeach()

file(GLOB con_map_files
    "ConquestMaps/*.map"
)

foreach(mf ${con_map_files})
    file(COPY ${mf} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ConquestMaps/)
endforeach()


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

add_executable(COMP345-A1-Part1 Map/Map.cpp MapLoader/MapLoader.cpp Cards/Cards.cpp Orders/OrdersDriver.cpp Orders/Orders.cpp Player/Player.cpp GameEngine/GameEngine.cpp GameObservers/GameObservers.cpp PlayerStrategies/PlayerStrategies.cpp)
