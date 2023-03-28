// -------------------------------------------------------------------------------------------------
// ------------------------------- IMPLEMENTATION :: Time-Controller -------------------------------
// -------------------------------------------------------------------------------------------------
#include "../../include/TimeController.hpp"

Statement* TimeController::createRefinedOre = nullptr;
Statement* TimeController::updateRefinedOre = nullptr;
Statement* TimeController::updateRefineries = nullptr;
Statement* TimeController::createOre        = nullptr;
Statement* TimeController::updateAsteroids  = nullptr;
Statement* TimeController::updateOre        = nullptr;
Statement* TimeController::updateDrones     = nullptr;