// -------------------------------------------------------------------------------------------------
// ---------------------------- HEADER :: Refinery-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_REFINERYSELECT_HPP
#define CSCI370FINALPROJECT_REFINERYSELECT_HPP

#include "RefineryControl.hpp"
#include "IO.hpp"
#include "Refinery.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ---------------------------- STRUCT :: Refinery-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
struct RefinerySelect {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* selector;
    static Statement* getter;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        selector = connection->createStatement(
            "SELECT * FROM Refineries WHERE fid = :1                    "
        );

        getter = connection->createStatement(
            "SELECT * FROM Refineries                                   "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(selector);
        connection->terminateStatement(getter);
    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------ METHOD :: Select a Refinery ------------------------------
    // -----------------------------------------------------------------------------------------
    Refinery* select(const std::string& command) {

        selector->setString(1, command);
        ResultSet* refinery = selector->executeQuery();

        if ( refinery->next() ) { return new Refinery(refinery); }

        IO::print("refinery not found");
        return nullptr;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- METHOD :: View all Refineries -----------------------------
    // -----------------------------------------------------------------------------------------
    void view() {

        ResultSet* refineries = getter->executeQuery();

        if ( refineries->next() ) {

            std::stringstream output;
            Refinery::stringify_head(output);

            do { Refinery(refineries).stringify_body(output); }
            while ( refineries->next() );

            IO::print(output.str());

        }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options() {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(19) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(19) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"          << "   ";
        output << std::setw(19) << "select a refinery"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                   << "   ";
        output << std::setw(19) << "view all refineries" << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                   << "   ";
        output << std::setw(19) << "print this menu"     << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                   << "   ";
        output << std::setw(19) << "quit to main menu"   << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control() {

        static RefineryControl refinery_control;

        while ( true ) {

            auto command = IO::read(
                    "[REFINERY SELECT] >>> enter refinery id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( Refinery* refinery = select(command) ) {
                    refinery_control.control(refinery);
                }

                continue;

            }

            if ( std::regex_match(command, std::regex("V|v")) ) {
                view(); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(); continue;
            }

            if ( std::regex_match(command, std::regex("Q|q")) ) {
                return;
            }

            if (command.empty() ) {
                continue;
            }

            IO::print("unrecognized command");

        }

    }

};


#endif // CSCI370FINALPROJECT_REFINERYSELECT_HPP
