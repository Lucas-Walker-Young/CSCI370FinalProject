// -------------------------------------------------------------------------------------------------
// ---------------------------------- HEADER :: Drone Controller -----------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DRONECONTROL_HPP
#define CSCI370FINALPROJECT_DRONECONTROL_HPP

#include "DroneHaul.hpp"
#include "DroneMove.hpp"
#include "IO.hpp"
#include "Drone.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ---------------------------------- STRUCT :: Drone Controller -----------------------------------
// -------------------------------------------------------------------------------------------------
struct DroneControl {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* deller;


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Configure Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void configure(Connection* connection) {

        deller = connection->createStatement(
            "UPDATE Drones                                                      "
            "   SET destination = NULL,                                         "
            "       haul        = NULL                                          "
            " WHERE did         = :1                                            "
        );

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------- CONFIGURATION :: Terminate Statements -------------------------
    // -----------------------------------------------------------------------------------------
    static void terminate(Connection* connection) {
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
    // ------------------------------- OPTIONS :: Print Options --------------------------------
    // -----------------------------------------------------------------------------------------
    void options(Drone* drone) {

        std::stringstream output;
        output << std::endl;

        output << std::setw(9)  << std::left << "Command"   << "   ";
        output << std::setw(23) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(23) << "" << "   ";
        output << std::setfill(' ');

        if ( !drone->destination.empty() ) {
            output << std::endl;
            output << std::setw(9)  << "A"                       << "   ";
            output << std::setw(23) << "abort current operation" << "   ";
        }

        output << std::endl;
        output << std::setw(9)  << "H"                     << "   ";
        output << std::setw(23) << "haul ore"              << "   ";

        output << std::endl;
        output << std::setw(9)  << "M"                     << "   ";
        output << std::setw(23) << "move drone elsewhere"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "P"                     << "   ";
        output << std::setw(23) << "print current drone"   << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                     << "   ";
        output << std::setw(23) << "print this menu"       << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                     << "   ";
        output << std::setw(23) << "quit to drone select"  << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control(Drone* drone) {

        static DroneHaul drone_haul;
        static DroneMove drone_move;

        while ( true ) {

            auto command = IO::read("[DRONE CONTROL] >>> enter command (O for Options): ");

            if ( std::regex_match(command, std::regex("A|a")) ) {
                abort(drone); continue;
            }

            if ( std::regex_match(command, std::regex("H|h")) ) {
                drone_haul.control(drone); continue;
            }

            if ( std::regex_match(command, std::regex("M|m")) ) {
                drone_move.control(drone); continue;
            }

            if ( std::regex_match(command, std::regex("P|p")) ) {
                print(drone); continue;
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


#endif // CSCI370FINALPROJECT_DRONECONTROL_HPP