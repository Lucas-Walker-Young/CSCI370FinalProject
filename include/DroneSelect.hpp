// -------------------------------------------------------------------------------------------------
// ----------------------------- HEADER :: Drone-Selection Controller ------------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DRONESELECT_HPP
#define CSCI370FINALPROJECT_DRONESELECT_HPP

#include "DroneControl.hpp"
#include "Drone.hpp"
#include "IO.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ----------------------------- HEADER :: Drone-Selection Controller ------------------------------
// -------------------------------------------------------------------------------------------------
struct DroneSelect {

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
            "SELECT * FROM Drones WHERE did = :1                    "
        );

        getter = connection->createStatement(
            "SELECT * FROM Drones                                   "
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
    // ------------------------------- METHOD :: Select a Drone --------------------------------
    // -----------------------------------------------------------------------------------------
    Drone* select(const std::string& command) {

        selector->setString(1, command);
        ResultSet* drone = selector->executeQuery();

        if ( drone->next() ) { return new Drone(drone); }

        IO::print("drone not found");
        return nullptr;

    }


    // -----------------------------------------------------------------------------------------
    // ------------------------------- METHOD :: View all Drones -------------------------------
    // -----------------------------------------------------------------------------------------
    void view() {

        ResultSet* drones = getter->executeQuery();

        if ( drones->next() ) {

            std::stringstream output;
            Drone::stringify_head(output);

            do { Drone(drones).stringify_body(output); }
            while ( drones->next() );

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
        output << std::setw(17) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(17) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"         << "   ";
        output << std::setw(17) << "select a drone"    << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                 << "   ";
        output << std::setw(17) << "view all drones"   << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                 << "   ";
        output << std::setw(17) << "print this menu"   << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                 << "   ";
        output << std::setw(17) << "quit to main menu" << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control() {

        static DroneControl drone_control;

        while ( true ) {

            auto command = IO::read("[DRONE SELECT] >>> enter drone id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( Drone* drone = select(command) ) {
                    drone_control.control(drone);
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


#endif // CSCI370FINALPROJECT_DRONESELECT_HPP
