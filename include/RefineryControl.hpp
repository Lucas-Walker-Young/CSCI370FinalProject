// -------------------------------------------------------------------------------------------------
// --------------------------------- HEADER :: Refinery Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_REFINERYCONTROL_HPP
#define CSCI370FINALPROJECT_REFINERYCONTROL_HPP

#include "Refinery.hpp"
#include "IO.hpp"
#include "Ore.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// --------------------------------- STRUCT :: Refinery Controller ---------------------------------
// -------------------------------------------------------------------------------------------------
struct RefineryControl {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* selector;
    static Statement* fallback;
    static Statement* getter;
    static Statement* setter;
    static Statement* deller;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        fallback = connection->createStatement(
                "SELECT * FROM Ore O WHERE oid = :1                             "
        );

        selector = connection->createStatement(
            "   SELECT O.oid, O.tid, O.location, O.createdAt, O.createdDate,    "
            "                                    R.refinedAt, R.refinedDate     "
            "     FROM Ore        O                                             "
            "LEFT JOIN RefinedOre R                                             "
            "       ON O.oid    = R.oid                                         "
            "    WHERE O.tid IN (                                               "
            "       SELECT Y.tidIn                                              "
            "         FROM CanRefine X                                          "
            "         JOIN CanBecome Y                                          "
            "           ON X.tid     =  Y.tidOut                                "
            "        WHERE X.fid     = :1                                       "
            " )                                                                 "
            "      AND O.location = :3                                          "
            "      AND O.oid      = :4                                          "
        );

        getter = connection->createStatement(
            "   SELECT O.oid, O.tid, O.location, O.createdAt, O.createdDate,    "
            "                                    R.refinedAt, R.refinedDate     "
            "     FROM Ore        O                                             "
            "LEFT JOIN RefinedOre R                                             "
            "       ON O.oid    = R.oid                                         "
            "    WHERE O.tid IN (                                               "
            "       SELECT Y.tidIn                                              "
            "         FROM CanRefine X                                          "
            "         JOIN CanBecome Y                                          "
            "           ON X.tid     =  Y.tidOut                                "
            "        WHERE X.fid     = :1                                       "
            " )                                                                 "
            "      AND O.location = :1                                          "
            " ORDER BY O.oid                                                    "
        );

        setter = connection->createStatement(
            "UPDATE Refineries                                                  "
            "   SET refining = :1                                               "
            " WHERE fid      = :2                                               "
        );

        deller = connection->createStatement(
            "UPDATE Refineries                                                  "
            "   SET refining = NULL                                             "
            " WHERE fid      = :1                                               "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(selector);
        connection->terminateStatement(fallback);
        connection->terminateStatement(getter);
        connection->terminateStatement(deller);
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Abort Current Operation ---------------------------
    // -----------------------------------------------------------------------------------------
    void abort(Refinery* refinery) {

        if ( !refinery->refining.empty() ) {

            deller->setString(1, refinery->fid);
            deller->executeUpdate();

            refinery->refining = "";

            IO::print("refinery operation aborted");

        } else { IO::print("cannot abort nonexistent refinery operation"); }

    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Print Current Refinery ----------------------------
    // -----------------------------------------------------------------------------------------
    void print(Refinery* refinery) {
        IO::print(refinery->stringify());
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: View all Refinable Ore ----------------------------
    // -----------------------------------------------------------------------------------------
    void view(Refinery* refinery) {

        getter->setString(1, refinery->fid);
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
    // ------------------------ METHOD :: (Attempt to) Assign Operation ------------------------
    // -----------------------------------------------------------------------------------------
    void assign(Refinery* refinery, const std::string& command) {

        if ( refinery->refining != command ) {

            if ( !refinery->refining.empty() ) {
                IO::print("previous refinery operation aborted");
            }

            setter->setString(1, command);
            setter->setString(2, refinery->fid);
            setter->executeUpdate();

            refinery->refining = command;

            IO::print("refinery operation assigned");

        }

        else { IO::print("operation is already underway"); }

    }


    // -----------------------------------------------------------------------------------------
    // ---------------- METHOD :: (Attempt to) Select Ore and Assign Operation -----------------
    // -----------------------------------------------------------------------------------------
    void select(Refinery* refinery, const std::string& command) {

        selector->setString(1, refinery->fid);
        selector->setString(2, refinery->fid);
        selector->setString(3, command);
        fallback->setString(1, command);

        ResultSet* ore = selector->executeQuery();

        if ( ore->next() ) {
            assign(refinery, command);
            return;
        }

        ore = fallback->executeQuery();

        if ( ore->next() ) {
            IO::print("the selected is located elsewhere or else cannot be "
                      "refined at this facility");
        }

        else { IO::print("unrecognized ore-id"); }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Refinery* refinery) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(23) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(23) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"               << "   ";
        output << std::setw(23) << "select an ore to refine" << "   ";

        if ( !refinery->refining.empty() ) {
            output << std::endl;
            output << std::setw(9)  << "A"                       << "   ";
            output << std::setw(23) << "abort current operation" << "   ";
        }

        output << std::endl;
        output << std::setw(9)  << "P"                       << "   ";
        output << std::setw(23) << "print current refinery"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                       << "   ";
        output << std::setw(23) << "view available ore"      << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                       << "   ";
        output << std::setw(23) << "print this menu"         << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                       << "   ";
        output << std::setw(23) << "quit to refinery select" << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Refinery* refinery) {

        while ( true ) {

            auto command = IO::read("[REFINERY CONTROL] >>> enter ore-id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {
                select(refinery, command); continue;
            }

            if ( std::regex_match(command, std::regex("A|a")) ) {
                abort(refinery); continue;
            }

            if ( std::regex_match(command, std::regex("P|p")) ) {
                print(refinery); continue;
            }

            if ( std::regex_match(command, std::regex("V|v")) ) {
                view(refinery); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(refinery); continue;
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


#endif // CSCI370FINALPROJECT_REFINERYCONTROL_HPP
