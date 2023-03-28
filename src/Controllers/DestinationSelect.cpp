// ------------------------------------------------------------------------------------------------
// ---------------- IMPLEMENTATION :: Drone-Haul Destination-Selection Controller -----------------
// ------------------------------------------------------------------------------------------------
#include "../../include/DestinationSelect.hpp"

Statement* DestinationSelect::selector = nullptr;
Statement* DestinationSelect::fallback = nullptr;
Statement* DestinationSelect::getter   = nullptr;
Statement* DestinationSelect::setter   = nullptr;
Statement* DestinationSelect::deller   = nullptr;