// -------------------------------------------------------------------------------------------------
// ---------------------------- HEADER :: Asteroid-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
#ifndef CSCI370FINALPROJECT_ASTEROIDSELECT_HPP
#define CSCI370FINALPROJECT_ASTEROIDSELECT_HPP

#include "AsteroidControl.hpp"
#include "Asteroid.hpp"
#include "IO.hpp"

#include <iomanip>
#include <sstream>
#include <regex>

#include <occi.h>
using namespace oracle::occi;


// -------------------------------------------------------------------------------------------------
// ---------------------------- STRUCT :: Asteroid-Selection Controller ----------------------------
// -------------------------------------------------------------------------------------------------
struct AsteroidSelect {

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
            "SELECT * FROM Asteroids WHERE fid = :1                    "
        );

        getter = connection->createStatement(
            "SELECT * FROM Asteroids                                   "
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
    // ----------------------------- METHOD :: Select an Asteroid ------------------------------
    // -----------------------------------------------------------------------------------------
    Asteroid* select(const std::string& command) {

        selector->setString(1, command);
        ResultSet* asteroid = selector->executeQuery();

        if ( asteroid->next() ) { return new Asteroid(asteroid); }

        IO::print("asteroid not found");
        return nullptr;

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- METHOD :: View all Asteroids ------------------------------
    // -----------------------------------------------------------------------------------------
    void view() {

        ResultSet* asteroids = getter->executeQuery();

        if ( asteroids->next() ) {

            std::stringstream output;
            Asteroid::stringify_head(output);

            do { Asteroid(asteroids).stringify_body(output); }
            while ( asteroids->next() );

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
        output << std::setw(18) << std::left << "Operation" << "   ";
        output << std::endl;

        output << std::setfill('-');
        output << std::setw(9)  << "" << "   ";
        output << std::setw(18) << "" << "   ";
        output << std::setfill(' ');

        output << std::endl;
        output << std::setw(9)  << "[0-9] × 9"          << "   ";
        output << std::setw(18) << "select an asteroid" << "   ";

        output << std::endl;
        output << std::setw(9)  << "V"                   << "   ";
        output << std::setw(18) << "view all asteroids"  << "   ";

        output << std::endl;
        output << std::setw(9)  << "O"                   << "   ";
        output << std::setw(18) << "print this menu"     << "   ";

        output << std::endl;
        output << std::setw(9)  << "Q"                   << "   ";
        output << std::setw(18) << "quit to main menu"   << "   ";

        output << std::endl;
        IO::print(output.str());

    }


    // -----------------------------------------------------------------------------------------
    // ----------------------------- CONTROL :: Main Control Loop ------------------------------
    // -----------------------------------------------------------------------------------------
    void control() {

        static AsteroidControl asteroid_control;

        while ( true ) {

            auto command = IO::read(
                    "[ASTEROID SELECT] >>> enter asteroid id (O for Options): ");

            if ( std::regex_match(command, std::regex("[0-9]{9}")) ) {

                if ( Asteroid* asteroid = select(command) ) {
                    asteroid_control.control(asteroid);
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


#endif // CSCI370FINALPROJECT_ASTEROIDSELECT_HPP
