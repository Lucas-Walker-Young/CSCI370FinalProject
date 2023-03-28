// -------------------------------------------------------------------------------------------------
// ----------------------------- IMPLEMENTATION :: Asteroid Controller -----------------------------
// -------------------------------------------------------------------------------------------------
#include "../../include/AsteroidControl.hpp"

Statement* AsteroidControl::selector = nullptr;
Statement* AsteroidControl::fallback = nullptr;
Statement* AsteroidControl::getter   = nullptr;
Statement* AsteroidControl::setter   = nullptr;
Statement* AsteroidControl::deller   = nullptr;