// -------------------------------------------------------------------------------------------------
// ----------------------------- IMPLEMENTATION :: Refinery Controller -----------------------------
// -------------------------------------------------------------------------------------------------
#include "../../include/RefineryControl.hpp"

Statement* RefineryControl::selector = nullptr;
Statement* RefineryControl::fallback = nullptr;
Statement* RefineryControl::getter   = nullptr;
Statement* RefineryControl::setter   = nullptr;
Statement* RefineryControl::deller   = nullptr;
