// -------------------------------------------------------------------------------------------------
// ----------------------------------- HEADER :: Facility Object -----------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_FACILITY_HPP
#define CSCI370FINALPROJECT_FACILITY_HPP

#include <iomanip>
#include <sstream>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ----------------------------------- STRUCT :: Facility Object -----------------------------------
// -------------------------------------------------------------------------------------------------
struct Facility {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    std::string refining;
    std::string miningFor;
    std::string category;
    std::string fid;


    // -----------------------------------------------------------------------------------------
    // ------------------------------ CONSTRUCTOR :: Constructor -------------------------------
    // -----------------------------------------------------------------------------------------
    Facility(ResultSet* data) {

        refining  = !data->isNull(4) ? data->getString(4) : "";
        miningFor = !data->isNull(3) ? data->getString(3) : "";

        category  = data->getString(2);
        fid       = data->getString(1);

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ STRINGIFICATION :: Stringify Table Head ------------------------
    // -----------------------------------------------------------------------------------------
    static void stringify_head(std::stringstream& output) {

        output << std::endl;

        output << std::setw(11) << std::left << "Facility ID" << "   ";
        output << std::setw(9)  << std::left << "Category"    << "   ";
        output << std::setw(10) << std::left << "Mining For"  << "   ";
        output << std::setw(9)  << std::left << "Refining"    << "   ";

        output << std::endl;

        output << std::setfill('-');
        output << std::setw(11) << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setw(10) << "" << "   ";
        output << std::setw(9)  << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------- STRINGIFICATION :: Stringify Object Body ------------------------
    // -----------------------------------------------------------------------------------------
    void stringify_body(std::stringstream& output) {

        output << std::setw(11) << fid       << "   ";
        output << std::setw(9)  << category  << "   ";
        output << std::setw(10) << miningFor << "   ";
        output << std::setw(9)  << refining  << "   ";

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


#endif // CSCI370FINALPROJECT_FACILITY_HPP
