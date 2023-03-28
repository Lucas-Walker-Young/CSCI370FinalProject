// -------------------------------------------------------------------------------------------------
// ---------------------------- IMPLEMENTATION :: Stockpile Controller -----------------------------
// -------------------------------------------------------------------------------------------------
#include "../../include/StockpileControl.hpp"

Statement* StockpileControl::selector = nullptr;
Statement* StockpileControl::fallback = nullptr;
Statement* StockpileControl::getter   = nullptr;
Statement* StockpileControl::deller_a = nullptr;
Statement* StockpileControl::deller_b = nullptr;