// -------------------------------------------------------------------------------------------------
// ----------------------------------- HEADER :: Ore-Type Object -----------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_ORETYPE_HPP
#define CSCI370FINALPROJECT_ORETYPE_HPP

#include <iomanip>
#include <sstream>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ----------------------------------- STRUCT :: Ore-Type Object -----------------------------------
// -------------------------------------------------------------------------------------------------
struct OreType {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    std::string tid;
    std::string name;
    int         value;
    std::string from;


    // -----------------------------------------------------------------------------------------
    // ------------------------------ CONSTRUCTOR :: Constructor -------------------------------
    // -----------------------------------------------------------------------------------------
    OreType(ResultSet* data) {

        from  = !data->isNull(4) ? data->getString(4) : "";

        tid   = data->getString(1);
        name  = data->getString(2);
        value = data->getNumber(3);

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ STRINGIFICATION :: Stringify Table Head ------------------------
    // -----------------------------------------------------------------------------------------
    static void stringify_head(std::stringstream& output) {

        output << std::endl;

        output << std::setw(9)  << std::left << "Type-ID"      << "   ";
        output << std::setw(11) << std::left << "Name"         << "   ";
        output << std::setw(5)  << std::left << "Value"        << "   ";
        output << std::setw(12) << std::left << "Refined From" << "   ";

        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(11) << "" << "   ";
        output << std::setw(5)  << "" << "   ";
        output << std::setw(12) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------- STRINGIFICATION :: Stringify Object Body ------------------------
    // -----------------------------------------------------------------------------------------
    void stringify_body(std::stringstream& output) {

        output << std::setw(9)  << std::left << tid   << "   ";
        output << std::setw(11) << std::left << name  << "   ";
        output << std::setw(5)  << std::left << value << "   ";
        output << std::setw(12) << std::left << from  << "   ";
        output << std::endl;

    }


    // -----------------------------------------------------------------------------------------
    // -------------------------- STRINGIFICATION :: Stringify Object --------------------------
    // -----------------------------------------------------------------------------------------
    std::string stringify() {

        std::stringstream output;

        stringify_head(output);
        stringify_body(output);

        return output.str();

    }

};


#endif // CSCI370FINALPROJECT_ORETYPE_HPP
