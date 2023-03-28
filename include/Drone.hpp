// -------------------------------------------------------------------------------------------------
// ------------------------------------ HEADER :: Drone Object -------------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DRONE_HPP
#define CSCI370FINALPROJECT_DRONE_HPP

#include <iomanip>
#include <sstream>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ------------------------------------ STRUCT :: Drone Object -------------------------------------
// -------------------------------------------------------------------------------------------------
struct Drone {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    std::string did;
    std::string location;
    std::string destination;
    std::string haul;


    // -----------------------------------------------------------------------------------------
    // ------------------------------ CONSTRUCTOR :: Constructor -------------------------------
    // -----------------------------------------------------------------------------------------
    Drone(ResultSet* data) {

        destination = !data->isNull(3) ? data->getString(3) : "";
        haul        = !data->isNull(4) ? data->getString(4) : "";

        did         =  data->getString(1);
        location    =  data->getString(2);

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ STRINGIFICATION :: Stringify Table Head ------------------------
    // -----------------------------------------------------------------------------------------
    static void stringify_head(std::stringstream& output) {

        output << std::endl;

        output << std::setw(9)  << std::left << "Drone ID"    << "   ";
        output << std::setw(9)  << std::left << "Location"    << "   ";
        output << std::setw(9)  << std::left << "Haul ID"     << "   ";
        output << std::setw(11) << std::left << "Destination" << "   ";

        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setw(11) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------- STRINGIFICATION :: Stringify Object Body ------------------------
    // -----------------------------------------------------------------------------------------
    void stringify_body(std::stringstream& output) {

        output << std::setw(9)  << std::left << did   << "   ";
        output << std::setw(9)  << std::left << location    << "   ";
        output << std::setw(9)  << std::left << haul     << "   ";
        output << std::setw(11) << std::left << destination << "   ";

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


#endif // CSCI370FINALPROJECT_DRONE_HPP
