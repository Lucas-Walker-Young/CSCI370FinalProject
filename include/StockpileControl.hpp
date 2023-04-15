// -------------------------------------------------------------------------------------------------
// -------------------------------- HEADER :: Stockpile Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_STOCKPILECONTROL_HPP
#define CSCI370FINALPROJECT_STOCKPILECONTROL_HPP

#include "Stockpile.hpp"
#include "IO.hpp"
#include "Ore.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// -------------------------------- STRUCT :: Stockpile Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
struct StockpileControl {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* selector;
    static Statement* fallback;
    static Statement* getter;
    static Statement* deller_a;
    static Statement* deller_b;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        fallback = connection->createStatement(
            "SELECT * FROM Ore WHERE oid = :1                                   "
        );

        selector = connection->createStatement(
            "   SELECT O.oid, O.tid, O.location, O.createdAt, O.createdDate,    "
            "                                    R.refinedAt, R.refinedDate     "
            "     FROM Ore O                                                    "
            "LEFT JOIN RefinedOre R                                             "
            "       ON O.oid = R.oid                                            "
            "    WHERE O.location = :1                                          "
            "      AND O.oid      = :2                                          "
        );

        getter = connection->createStatement(
            "   SELECT O.oid, O.tid, O.location, O.createdAt, O.createdDate,    "
            "                                    R.refinedAt, R.refinedDate     "
            "     FROM Ore        O                                             "
            "LEFT JOIN RefinedOre R                                             "
            "       ON O.oid = R.oid                                            "
            "    WHERE O.location = :1                                          "
        );

        deller_a = connection->createStatement(
            "DELETE FROM Ore WHERE oid = :1                                     "
        );

        deller_b = connection->createStatement(
            "DELETE FROM RefinedOre WHERE oid = :1                              "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(selector);
        connection->terminateStatement(fallback);
        connection->terminateStatement(getter);
        connection->terminateStatement(deller_a);
        connection->terminateStatement(deller_b);
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Print Current Stockpile ---------------------------
    // -----------------------------------------------------------------------------------------
    void print(Stockpile* stockpile) {
        IO::print(stockpile->stringify());
    }


    // -----------------------------------------------------------------------------------------
    // ---------------------------- METHOD :: View all Sellable Ore ----------------------------
    // -----------------------------------------------------------------------------------------
    void view(Stockpile* stockpile) {

        getter->setString(1, stockpile->fid);
        ResultSet* ore = getter->executeQuery();

        if ( ore->next() ) {

            std::stringstream output;
            Ore::stringify_head(output);

            do { Ore(ore).stringify_body(output); }
            while ( ore->next() );

            IO::print(output.str());

        }

    }


    // -----------------------------------------------------------------------------------------
    // ---------------------- METHOD :: (Attempt to) Select and Sell Ore -----------------------
    // -----------------------------------------------------------------------------------------
    void select(Stockpile* stockpile, const std::string& command) {

        selector->setString(1, stockpile->fid);
        selector->setString(2, command);
        fallback->setString(1, command);

        ResultSet* ore = selector->executeQuery();

        if ( ore->next() ) {

            if ( !ore->isNull(6) ) {
                deller_b->setString(1, command);
                deller_b->executeUpdate();
            }

            deller_a->setString(1, command);
            deller_a->executeUpdate();

	    IO::print("ore sold successfully");

            return;

        }

        ore = fallback->executeQuery();

        if ( !ore->next() ) { IO::print("unrecognized ore-id"); }
        else { IO::print("the selected ore is not located at this facility"); }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Stockpile* stockpile) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(24) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(24) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"                << "   ";
        output << std::setw(24) << "select an ore to sell"    << "   ";

        output << std::endl;
        output << std::setw(9)  << "P"                        << "   ";
        output << std::setw(24) << "print current stockpile"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                        << "   ";
        output << std::setw(24) << "view available ore"       << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                        << "   ";
        output << std::setw(24) << "print this menu"          << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                        << "   ";
        output << std::setw(24) << "quit to stockpile select" << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Stockpile* stockpile) {

        while ( true ) {

            auto command = IO::read("[STOCKPILE CONTROL] >>> enter ore-id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {
                select(stockpile, command); continue;
            }

            if ( std::regex_match(command, std::regex("P|p")) ) {
                print(stockpile); continue;
            }

            if ( std::regex_match(command, std::regex("V|v")) ) {
                view(stockpile); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(stockpile); continue;
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


#endif // CSCI370FINALPROJECT_STOCKPILECONTROL_HPP
