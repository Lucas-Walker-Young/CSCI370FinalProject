// ------------------------------------------------------------------------------------------------
// ---------------- IMPLEMENTATION :: Drone-Move Destination-Selection Controller -----------------
// ------------------------------------------------------------------------------------------------
#include "../../include/DroneMove.hpp"

Statement* DroneMove::selector = nullptr;
Statement* DroneMove::getter   = nullptr;
Statement* DroneMove::setter   = nullptr;
Statement* DroneMove::deller   = nullptr;