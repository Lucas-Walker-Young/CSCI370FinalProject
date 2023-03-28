// -------------------------------------------------------------------------------------------------
// --------------------------- HEADER :: Stockpile-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_STOCKPILESELECT_HPP
#define CSCI370FINALPROJECT_STOCKPILESELECT_HPP

#include "StockpileControl.hpp"
#include "IO.hpp"
#include "Stockpile.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// --------------------------- STRUCT :: Stockpile-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
struct StockpileSelect {

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
                "SELECT * FROM Stockpiles WHERE fid = :1                    "
        );

        getter = connection->createStatement(
                "SELECT * FROM Stockpiles                                  "
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
    // ----------------------------- METHOD :: Select a Stockpile ------------------------------
    // -----------------------------------------------------------------------------------------
    Stockpile* select(const std::string& command) {

        selector->setString(1, command);
        ResultSet* stockpile = selector->executeQuery();

        if ( stockpile->next() ) { return new Stockpile(stockpile); }

        IO::print("stockpile not found");
        return nullptr;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- METHOD :: View all Stockpiles -----------------------------
    // -----------------------------------------------------------------------------------------
    void view() {

        ResultSet* stockpiles = getter->executeQuery();

        if ( stockpiles->next() ) {

            std::stringstream output;
            Stockpile::stringify_head(output);

            do { Stockpile(stockpiles).stringify_body(output); }
            while ( stockpiles->next() );

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
        output << std::setw(19) << "select a stockpile" << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                   << "   ";
        output << std::setw(19) << "view all stockpiles" << "   ";

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

        static StockpileControl stockpile_control;

        while ( true ) {

            auto command = IO::read(
                    "[STOCKPILE SELECT] >>> enter stockpile id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( Stockpile* stockpile = select(command) ) {
                    stockpile_control.control(stockpile);
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


#endif // CSCI370FINALPROJECT_STOCKPILESELECT_HPP
