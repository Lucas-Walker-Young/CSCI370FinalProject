// -------------------------------------------------------------------------------------------------
// ------------------------------------- HEADER :: Ore Object --------------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_ORE_HPP
#define CSCI370FINALPROJECT_ORE_HPP

#include <iomanip>
#include <sstream>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ------------------------------------- STRUCT :: Ore Object --------------------------------------
// -------------------------------------------------------------------------------------------------
struct Ore {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    std::string createdDate;
    std::string createdAt;
    std::string refinedDate;
    std::string refinedAt;
    std::string location;
    std::string tid;
    std::string oid;


    // -----------------------------------------------------------------------------------------
    // ------------------------------ CONSTRUCTOR :: Constructor -------------------------------
    // -----------------------------------------------------------------------------------------
    Ore(ResultSet* data) {

        refinedDate = !data->isNull(7) ? data->getDate(7).toText() : "";
        refinedAt   = !data->isNull(6) ? data->getString(6)        : "";

        oid         =  data->getString(1);
        tid         =  data->getString(2);
        location    =  data->getString(3);
        createdAt   =  data->getString(4);
        createdDate =  data->getDate(5).toText();

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ STRINGIFICATION :: Stringify Table Head ------------------------
    // -----------------------------------------------------------------------------------------
    static void stringify_head(std::stringstream& output) {

        output << std::endl;

        output << std::setw(9)  << std::left << "Ore ID"       << "   ";
        output << std::setw(9)  << std::left << "Ore Type"     << "   ";
        output << std::setw(9)  << std::left << "Location"     << "   ";
        output << std::setw(10) << std::left << "Created At"   << "   ";
        output << std::setw(12) << std::left << "Created Date" << "   ";
        output << std::setw(10) << std::left << "Refined At"   << "   ";
        output << std::setw(12) << std::left << "Refined Date" << "   ";

        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setw(10) << "" << "   ";
        output << std::setw(12) << "" << "   ";
        output << std::setw(10) << "" << "   ";
        output << std::setw(12) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------- STRINGIFICATION :: Stringify Object Body ------------------------
    // -----------------------------------------------------------------------------------------
    void stringify_body(std::stringstream& output) {

        output << std::setw(9)  << std::left << oid         << "   ";
        output << std::setw(9)  << std::left << tid         << "   ";
        output << std::setw(9)  << std::left << location    << "   ";
        output << std::setw(10) << std::left << createdAt   << "   ";
        output << std::setw(12) << std::left << createdDate << "   ";
        output << std::setw(10) << std::left << refinedAt   << "   ";
        output << std::setw(12) << std::left << refinedDate << "   ";
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



#endif // CSCI370FINALPROJECT_ORE_HPP
