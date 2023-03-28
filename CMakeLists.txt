cmake_minimum_required(VERSION 3.24)
project(CSCI370FinalProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(CSCI370FinalProject src/main.cpp src/Controllers/AsteroidControl.cpp src/Controllers/AsteroidSelect.cpp src/Controllers/DroneControl.cpp src/Controllers/DroneSelect.cpp src/Controllers/RefineryControl.cpp src/Controllers/RefinerySelect.cpp src/Controllers/StockpileControl.cpp src/Controllers/StockpileSelect.cpp src/Utilities/IO.cpp src/Queries/Facility.cpp src/Queries/OreType.cpp src/Queries/Refinery.cpp src/Queries/Asteroid.cpp src/Queries/Stockpile.cpp src/Queries/Drone.cpp src/Queries/Ore.cpp src/Controllers/DroneMove.cpp src/Controllers/DroneHaul.cpp src/Controllers/DestinationSelect.cpp src/Controllers/TimeController.cpp)
include_directories(~/Downloads/instantclient_19_8/sdk/include)