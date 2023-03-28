// ------------------------------------------------------------------------------------------------
// -------------------- IMPLEMENTATION :: Drone-Haul Ore-Selection Controller ---------------------
// ------------------------------------------------------------------------------------------------
#include "../../include/DroneHaul.hpp"

Statement* DroneHaul::selector = nullptr;
Statement* DroneHaul::fallback = nullptr;
Statement* DroneHaul::getter   = nullptr;
Statement* DroneHaul::deller   = nullptr;
