// ------------------------------------------------------------------------------------------------
// ------------------------ HEADER :: Drone-Haul Ore-Selection Controller -------------------------
// ------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_DRONEHAUL_HPP
#define CSCI370FINALPROJECT_DRONEHAUL_HPP

#include "DestinationSelect.hpp"
#include "Drone.hpp"
#include "IO.hpp"
#include "Ore.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// ------------------------------------------------------------------------------------------------
// ------------------------ STRUCT :: Drone-Haul Ore-Selection Controller -------------------------
// ------------------------------------------------------------------------------------------------
struct DroneHaul {

    // -----------------------------------------------------------------------------------------
    // ------------------------------- ATTRIBUTES :: Attributes --------------------------------
    // -----------------------------------------------------------------------------------------
    static Statement* fallback;
    static Statement* selector;
    static Statement* getter;
    static Statement* deller;


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
            "     FROM Ore        O                                             "
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
        connection->terminateStatement(selector);
        connection->terminateStatement(fallback);
        connection->terminateStatement(getter);
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

        getter->setString(1, drone->location);
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
    // ---------------------------- METHOD :: Select a Ore to Hail -----------------------------
    // -----------------------------------------------------------------------------------------
    Ore* select(Drone* drone, const std::string& command) {

        selector->setString(1, drone->location);
        selector->setString(2, command);
        fallback->setString(1, command);

        ResultSet* ore = selector->executeQuery();

        if ( ore->next() ) {
            return new Ore(ore);
        }

        ore = fallback->executeQuery();

        if ( !ore->next() ) { IO::print("ore-id not recognized"); }
        else { IO::print("the selected ore is not present at this facility"); }

        return nullptr;

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
        output << std::setw(21) << "select an ore to haul"  << "   ";

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
        output << std::setw(21) << "view all moveable ore"  << "   ";

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

        static DestinationSelect destination_select;

        while ( true ) {

            auto command = IO::read("[HAUL SELECT] >>> enter ore-id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( Ore* ore = select(drone, command) ) {
                    destination_select.control(drone, ore);
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

            if (command.empty() ) {
                continue;
            }

            IO::print("unrecognized command");

        }

    }

};


#endif // CSCI370FINALPROJECT_DRONEHAUL_HPP
