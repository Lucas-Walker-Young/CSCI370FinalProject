// ------------------------------------------------------------------------------------------------
// -------------------- HEADER :: Drone-Haul Destination-Selection Controller ---------------------
// ------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DESTINATIONSELECT_HPP
#define CSCI370FINALPROJECT_DESTINATIONSELECT_HPP

#include "Drone.hpp"
#include "Ore.hpp"
#include "IO.hpp"
#include "Facility.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// ------------------------------------------------------------------------------------------------
// -------------------- HEADER :: Drone-Haul Destination-Selection Controller ---------------------
// ------------------------------------------------------------------------------------------------
struct DestinationSelect {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* fallback;
    static Statement* selector;
    static Statement* getter;
    static Statement* setter;
    static Statement* deller;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        fallback = connection->createStatement(
            "SELECT * FROM Facilities WHERE fid = :1                                    "
        );

        selector = connection->createStatement(
            "SELECT A.fid, 'Asteroid', A.miningFor, CAST (NULL AS CHAR(9))              "
            "  FROM Asteroids A                                                         "
            "  JOIN CanCreate X                                                         "
            "    ON A.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
            "   AND A.fid = :2                                                          "
            " UNION                                                                     "
            "SELECT R.fid, 'Refinery', CAST (NULL AS CHAR(9)), R.refining               "
            "  FROM Refineries R                                                        "
            "  JOIN CanRefine  X                                                        "
            "    ON R.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
            "   AND R.fid = :2                                                          "
            " UNION                                                                     "
            "SELECT R.fid, 'Refinery', CAST (NULL AS CHAR(9)), R.refining               "
            "  FROM Refineries R                                                        "
            "  JOIN CanRefine  X                                                        "
            "    ON R.fid = X.fid                                                       "
            "  JOIN CanBecome  Y                                                        "
            "    ON X.tid = Y.tidOut                                                    "
            " WHERE Y.tidIn = :1                                                        "
            "   AND R.fid   = :2                                                        "
            " UNION                                                                     "
            "SELECT S.fid, 'Stockpile', CAST (NULL AS CHAR(9)), CAST (NULL AS CHAR(9))  "
            "  FROM Stockpiles S                                                        "
            "  JOIN CanStore   X                                                        "
            "    ON S.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
            "   AND S.fid = :2                                                          "
        );

        getter = connection->createStatement(
            "SELECT A.fid, 'Asteroid', A.miningFor, CAST (NULL AS CHAR(9))              "
            "  FROM Asteroids A                                                         "
            "  JOIN CanCreate X                                                         "
            "    ON A.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
            " UNION                                                                     "
            "SELECT R.fid, 'Refinery', CAST (NULL AS CHAR(9)), R.refining               "
            "  FROM Refineries R                                                        "
            "  JOIN CanRefine  X                                                        "
            "    ON R.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
            " UNION                                                                     "
            "SELECT R.fid, 'Refinery', CAST (NULL AS CHAR(9)), R.refining               "
            "  FROM Refineries R                                                        "
            "  JOIN CanRefine  X                                                        "
            "    ON R.fid = X.fid                                                       "
            "  JOIN CanBecome  Y                                                        "
            "    ON X.tid = Y.tidOut                                                    "
            " WHERE Y.tidIn = :1                                                        "
            " UNION                                                                     "
            "SELECT S.fid, 'Stockpile', CAST (NULL AS CHAR(9)), CAST (NULL AS CHAR(9))  "
            "  FROM Stockpiles S                                                        "
            "  JOIN CanStore   X                                                        "
            "    ON S.fid = X.fid                                                       "
            " WHERE X.tid = :1                                                          "
        );

        setter = connection->createStatement(
            "UPDATE Drones                                                              "
            "   SET destination = :1,                                                   "
            "       haul        = :2                                                    "
            " WHERE did         = :3                                                    "
        );

        deller = connection->createStatement(
                "UPDATE Drones                                                          "
                "   SET destination = NULL,                                             "
                "       haul        = NULL                                              "
                " WHERE did         = :1                                                "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
        connection->terminateStatement(selector);
        connection->terminateStatement(fallback);
        connection->terminateStatement(getter);
        connection->terminateStatement(setter);
        connection->terminateStatement(deller);
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: Abort Current Operation ---------------------------
    // -----------------------------------------------------------------------------------------
    void abort(Drone* drone) {

        if ( !drone->destination.empty() ) {

            if ( !drone->haul.empty() ) {
                IO::print("haul operation aborted");
            }

            else {
                IO::print("move operation aborted");
            }

            deller->setString(1, drone->did);
            deller->executeUpdate();

            drone->haul        = "";
            drone->destination = "";

        }

        else { IO::print("cannot abort nonexistent drone operation"); }

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- METHOD :: Print Current Drone -----------------------------
    // -----------------------------------------------------------------------------------------
    void print(Drone* drone) {
        IO::print(drone->stringify());
    }


    // -----------------------------------------------------------------------------------------
    // --------------------------- METHOD :: View Haul-Destinations ----------------------------
    // -----------------------------------------------------------------------------------------
    void view(Drone* drone) {

        getter->setString(1, drone->location);
        ResultSet* destinations = getter->executeQuery();

        if ( destinations->next() ) {

            std::stringstream output;
            Facility::stringify_head(output);

            do { Facility(destinations).stringify_body(output); }
            while ( destinations->next() );

            IO::print(output.str());

        }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ METHOD :: (Attempt to) Assign Operation ------------------------
    // -----------------------------------------------------------------------------------------
    bool assign(Drone* drone, Ore* ore, const std::string& command) {

        if ( drone->haul != ore->oid || drone->destination != command ) {

            if ( ore->location != command ) {

                if (!drone->haul.empty()) {
                    IO::print("previous drone-haul operation aborted");
                } else if (!drone->destination.empty()) {
                    IO::print("previous drone-move operation aborted");
                }

                setter->setString(1, command);
                setter->setString(2, ore->oid);
                setter->setString(3, drone->did);
                setter->executeUpdate();

                drone->haul = ore->oid;
                drone->destination = command;

                IO::print("drone-haul operation assigned");
                return true;

            }

            IO::print("the selected ore is already at this facility");
            return true;

        }

        IO::print("haul operation already underway");
        return false;

    }


    // -----------------------------------------------------------------------------------------
    // ------------ METHOD :: (Attempt to) Select Destination and Assign Operation -------------
    // -----------------------------------------------------------------------------------------
    bool select(Drone* drone, Ore* ore, const std::string& command) {

        selector->setString(1, ore->tid);
        selector->setString(2, command);
        fallback->setString(1, command);

        ResultSet* destination = selector->executeQuery();

        if ( destination->next() ) {
            return assign(drone, ore, command);
        }

        destination = fallback->executeQuery();

        if ( !destination->next() ) { IO::print("destination id not recognized"); }
        else { IO::print("the selected ore cannot be shipped to the specified facility"); }

        return false;

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Drone* drone) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(30) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(30) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"                      << "   ";
        output << std::setw(30) << "select haul destination"        << "   ";

        if ( !drone->destination.empty() ) {
            output << std::endl;
            output << std::setw(9)  << "A"                          << "   ";
            output << std::setw(30) << "abort current operation"    << "   ";
        }

        output << std::endl;
        output << std::setw(9)  << "P"                              << "   ";
        output << std::setw(30) << "print current drone"            << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                              << "   ";
        output << std::setw(30) << "view all possible destinations" << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                              << "   ";
        output << std::setw(30) << "print this menu"                << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                              << "   ";
        output << std::setw(30) << "quit to drone control"          << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Drone* drone, Ore* ore) {

        while ( true ) {

            auto command = IO::read("[HAUL SELECT] >>> enter destination id (O for options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( select(drone, ore, command) ) {
                    return;
                }

                continue;

            }

            if ( std::regex_match(command, std::regex("A|a")) ) {
                abort(drone); return;
            }

            if ( std::regex_match(command, std::regex("P|p")) ) {
                print(drone); continue;
            }

            if ( std::regex_match(command, std::regex("V|v")) ) {
                view(drone); continue;
            }

            if ( std::regex_match(command, std::regex("O|o")) ) {
                options(drone); continue;
            }

            if ( std::regex_match(command, std::regex("Q|q")) ) {
                return;
            }

            if ( command.empty() ) {
                continue;
            }

            IO::print("unrecognized command");

        }

    }

};


#endif //CSCI370FINALPROJECT_DESTINATIONSELECT_HPP
