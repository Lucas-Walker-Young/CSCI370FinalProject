// ------------------------------------------------------------------------------------------------
// -------------------- HEADER :: Drone-Move Destination-Selection Controller ---------------------
// ------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DRONEMOVE_HPP
#define CSCI370FINALPROJECT_DRONEMOVE_HPP

#include "Drone.hpp"
#include "IO.hpp"
#include "Facility.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// ------------------------------------------------------------------------------------------------
// -------------------- STRUCT :: Drone-Move Destination-Selection Controller ---------------------
// ------------------------------------------------------------------------------------------------
struct DroneMove {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* selector;
    static Statement* getter;
    static Statement* setter;
    static Statement* deller;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        selector = connection->createStatement(
            "SELECT * FROM Facilities WHERE fid = :1                                "
        );

        getter = connection->createStatement(
            "SELECT fid, 'Asteroid', miningFor, CAST(NULL as CHAR(9))               "
            "  FROM Asteroids                                                       "
            " UNION                                                                 "
            "SELECT fid, 'Refinery', CAST(NULL as CHAR(9)), refining                "
            "  FROM Refineries                                                      "
            " UNION                                                                 "
            "SELECT fid, 'Stockpile', CAST(NULL as CHAR(9)), CAST(NULL as CHAR(9))  "
            "  FROM Stockpiles                                                      "
        );

        setter = connection->createStatement(
            "UPDATE Drones                                                          "
            "   SET destination = :1                                                "
            " WHERE did         = :2                                                "
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
    // ------------------------------ METHOD :: View Moveable Ore ------------------------------
    // -----------------------------------------------------------------------------------------
    void view(Drone* drone) {

        ResultSet* facilities = getter->executeQuery();

        if ( facilities->next() ) {

            std::stringstream output;
            Facility::stringify_head(output);

            do { Facility(facilities).stringify_body(output); }
            while ( facilities->next() );

            IO::print(output.str());

        }

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------ METHOD :: (Attempt to) Assign Operation ------------------------
    // -----------------------------------------------------------------------------------------
    bool assign(Drone* drone, const std::string& command) {

        if ( !drone->haul.empty() || drone->destination != command ) {

            if ( drone->location != command ) {

                if (!drone->haul.empty()) {
                    IO::print("previous drone-haul operation aborted");
                }

                else if (!drone->destination.empty()) {
                    IO::print("previous drone-move operation aborted");
                }

                setter->setString(1, command);
                setter->setString(2, drone->did);
                setter->executeUpdate();

                drone->haul = "";
                drone->destination = command;

                IO::print("drone-move operation assigned");
                return true;

            }

            if (!drone->haul.empty()) {
                IO::print("previous drone-haul operation aborted");
            }

            IO::print("drone is already present at this location");
            return false;

        }

        else { IO::print("operation is already underway"); }
        return false;

    }


    // -----------------------------------------------------------------------------------------
    // ------------ METHOD :: (Attempt to) Select Destination and Assign Operation -------------
    // -----------------------------------------------------------------------------------------
    bool select(Drone* drone, const std::string& command) {

        selector->setString(1, command);
        ResultSet* destination = selector->executeQuery();

        if ( destination->next() ) {
            return assign(drone, command);
        }

        IO::print("unrecognized destination-id");

        return false;

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Drone* drone) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(21) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(21) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"              << "   ";
        output << std::setw(21) << "select destination"     << "   ";

        if ( !drone->destination.empty() ) {
            output << std::endl;
            output << std::setw(9)  << "A"                       << "   ";
            output << std::setw(21) << "abort current operation" << "   ";
        }

        output << std::endl;
        output << std::setw(9)  << "P"                      << "   ";
        output << std::setw(21) << "print current drone"    << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                      << "   ";
        output << std::setw(21) << "view all destinations"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                      << "   ";
        output << std::setw(21) << "print this menu"        << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                      << "   ";
        output << std::setw(21) << "quit to drone control"  << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Drone* drone) {

        while ( true ) {

            auto command = IO::read("[MOVE SELECT] >>> enter destination id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( select(drone, command) ) {
                    return;
                }

                continue;

            }

            if ( std::regex_match(command, std::regex("A|a")) ) {
                abort(drone); continue;
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

            if (command.empty() ) {
                continue;
            }

            IO::print("unrecognized command");

        }

    }

};


#endif // CSCI370FINALPROJECT_DRONEMOVE_HPP
